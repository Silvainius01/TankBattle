#include <iostream>
#include <sstream>

#include "TankBattle-AI\brain.h"
#include "TankBattleNet.h"
#include "sfwdraw.h"
#undef NONE     // sfw defines NONE as one of its colors; we won't be needing that

using std::stringstream;

// declare controls for example client
const char TANK_FWRD = 'W';
const char TANK_BACK = 'S';
const char TANK_LEFT = 'A';
const char TANK_RIGT = 'D';

const char TANK_FIRE = 'F';

const char CANN_LEFT = 'Q';
const char CANN_RIGT = 'E';

const char GAME_QUIT = 'L';

const int WINDOW_HEIGHT = 800;
const int WINDOW_WIDTH = 400;

tankNet::TankBattleCommand clientData()
{
	tankNet::TankBattleCommand data;
	//tank actions
	data.tankMove = decisions.body_up ? tankNet::TankMovementOptions::FWRD :
		decisions.body_down ? tankNet::TankMovementOptions::BACK :
		decisions.body_left ? tankNet::TankMovementOptions::LEFT :
		decisions.body_right ? tankNet::TankMovementOptions::RIGHT :
		tankNet::TankMovementOptions::HALT;

	data.cannonMove = decisions.cannon_left ? tankNet::CannonMovementOptions::LEFT :
		decisions.cannon_right ? tankNet::CannonMovementOptions::RIGHT :
		tankNet::CannonMovementOptions::HALT;

	data.fireWish = decisions.fire;

	//game actions
	if (sfw::getKey(GAME_QUIT))
	{
		data.msg = tankNet::TankBattleMessage::QUIT;
	}

	return data;
}


enum ServerName
{
	HOME,
	TUNG
};
char *getIP(ServerName sn)
{
	switch (sn)
	{
	case TUNG: return "10.15.22.46";
	default: return "";
	}
}


int main(int argc, char** argv)
{
    char * serverIPAddress = getIP(HOME);

    // handle console arguments
    if (argc > 2)
    {
        std::cout << "Unsupported number of arguments." << std::endl;
        std::cout << "Specify the IP address of the target server. Ex: " << argv[0] << " 127.0.0.1" << std::endl;

        return EXIT_FAILURE;
    }
    else if (argc == 2)
    {
        serverIPAddress = argv[1];
    }

    // initialize networking
    if (serverIPAddress[0] == '\0')
    {
        tankNet::init();
    }
    else
    {
        tankNet::init(serverIPAddress);
    }

    while (!tankNet::isProvisioned())
    {
		// block further execution until the server responds
		// or until the client gives up on connecting
        tankNet::update();
    }

    // if a connection was successful...
    if (tankNet::isConnected() && tankNet::isProvisioned())
    {
        auto serverData = tankNet::recieve();

        // initialize SFW and assets
        sfw::initContext(WINDOW_WIDTH, WINDOW_HEIGHT, "TankController");
        unsigned font = sfw::loadTextureMap("./res/fontmap.png", 16, 16);

        while (sfw::stepContext() && tankNet::isConnected() && tankNet::isProvisioned())
        {
            // check TCP streams via dyad
            tankNet::update();
            if (tankNet::isConnected() == false)
            {
                break;
            }

            tankNet::TankBattleStateData * state = tankNet::recieve();

            // diagnostic report of current state
            stringstream debugStrings;
            debugStrings << *state;
            debugStrings << "Tacticool Report:\n";
            for (int i = 0; i < state->tacticoolCount; ++i)
            {
                debugStrings << state->tacticoolData[i].playerID << "\n    ";
                for (int j = 0; j < 3; ++j)
                {
                    debugStrings << state->tacticoolData[i].lastKnownPosition[j] << " , ";
                }
                debugStrings << "\n    ";
                for (int j = 0; j < 3; ++j)
                {
                    debugStrings << state->tacticoolData[i].lastKnownDirection[j] << " , ";
                }

                debugStrings << "\ninSight: " << (state->tacticoolData[i].inSight ? "true" : "false") << "\n";
            }

            sfw::drawString(font, debugStrings.str().c_str(), 0, WINDOW_HEIGHT, 15, 15);

            // prepare message
			leka.update(state);
			send(clientData());
        }
    }
    else
    {
        std::cout << "\nFailed to connect to server. Is the server online and not full?";
    }

    tankNet::term();
    sfw::termContext();

    return EXIT_SUCCESS;
}