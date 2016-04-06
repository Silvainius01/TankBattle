#include <iostream>
#include <random>
#include <time.h>

#include "dyad.h"
#include "sfwdraw.h"
#undef NONE

#include "TankBattleHeader.h"

int     myPlayerID = -1;

bool    isConnected = false;   
bool    isWaiting = false;
bool    isProvisioned = false;

void    onConnect(dyad_Event *e)
{
    printf("connected: %s\n", e->msg);
    isConnected = true;
}
void    onData(dyad_Event *e)
{
    printf("onData: ");

    auto msg = (TankBattleStateData*)e->data;
    
    if (myPlayerID == -1)
    {
        std::cout << "DIAGNOSTIC STATUS:\n";
        std::cout << (msg->playerID) << "\n";

        myPlayerID = msg->playerID;

        isProvisioned = true;
        isWaiting = false;
    }
    else
    {
        system("cls");

        std::cout << "position" << "\n";
        std::cout << "  x: " << msg->position[0] << "\n  y: " << msg->position[1] << "\n  z: " << msg->position[2] << "\n\n";
        
        std::cout << "direction" << "\n";
        std::cout << "  x: " << msg->forward[0] << "\n  y: " << msg->forward[1] << "\n  z: " << msg->forward[2] << "\n\n";

        std::cout << "cannon direction" << "\n";
        std::cout << "  x: " << msg->cannonForward[0] << "\n  y: " << msg->cannonForward[1] << "\n  z: " << msg->cannonForward[2] << "\n\n";

        std::cout << "canShoot\n  " << (msg->canFire ? "yes" : "no") << "\n";
        std::cout << "los to enemy\n  " << (msg->enemyInSight ? "yes" : "no") << "\n";
    }
}
void    onError(dyad_Event *e)
{
    printf("onError: ");
    printf("%s", e->msg);
}
void    onClose(dyad_Event *e)
{
    printf("onClose: ");
    printf("%s", e->msg);

    isConnected = false;
}
bool    inputPressed()
{
    for (unsigned int i = 0; i < 150; ++i)
    {
        if (sfw::getKey(i))
            return true;
    }

    return false;
}

#define TANK_FWRD 'W'
#define TANK_BACK 'S'
#define TANK_LEFT 'A'
#define TANK_RIGT 'D'

#define CANN_LEFT 'Q'
#define CANN_RIGT 'E'

#define TANK_FIRE 'F'

#define GAME_QUIT 'L'

#define CLIENT_MAJOR_VERSION 0
#define CLIENT_MINOR_VERSION 1

int main(int argc, char** argv)
{
    char * serverIPAddress = "";

    // handle console arguments
    if (argc > 2)
    {
        std::cout << "Unsupported number of arguments." << std::endl;
        std::cout << "Specify the IP address of the target server. Ex: tankbattle.exe 127.0.0.1" << std::endl;

        return EXIT_FAILURE;
    }
    else if (argc == 2)
    {
        serverIPAddress = argv[1];
    }

    // Boot-up sequence
    std::cout << "TankBattleClient v" << CLIENT_MAJOR_VERSION << "." << CLIENT_MINOR_VERSION << "\n";
    
    // acquire server IP if not provided in args
    if (serverIPAddress[0] == '\0')
    {
        std::cout << "Enter the server address. \n";

        char userInput[17];
        std::cin.getline(serverIPAddress, 16);
    }

    srand(time(NULL));

    // initialize dyad

    dyad_init();

    dyad_Stream *s = dyad_newStream();
    dyad_addListener(s, DYAD_EVENT_CONNECT, onConnect, NULL);
    dyad_addListener(s, DYAD_EVENT_CLOSE, onClose, NULL);
    dyad_addListener(s, DYAD_EVENT_DATA, onData, NULL);
    dyad_addListener(s, DYAD_EVENT_ERROR, onError, NULL);

    if (dyad_connect(s, serverIPAddress, 11000) == -1)
    {
        std::cerr << "Failed to connect to specified server at " << serverIPAddress << ".\n";
        return EXIT_FAILURE;
    }

    sfw::initContext(400, 400, "TankController");

    while (dyad_getStreamCount() > 0 && sfw::stepContext())
    {
        // check TCP streams via dyad
        dyad_setUpdateTimeout(0.0);
        dyad_update();

        if (isConnected && !isWaiting)
        {
            // prepare message
            const int msgSize = sizeof(tankBattleHeader);
            unsigned char msg[msgSize];

            tankBattleHeader ex;
            ex.msg = tankBattleMessage::NONE;
            ex.tankMove = TankMovementOptions::HALT;
            ex.cannonMove = CannonMovementOptions::HALT;
            ex.messageLength = msgSize;    // TODO: support for dynamic message length
            ex.playerID = myPlayerID;

            if (ex.playerID == -1)
                isWaiting = true;

            // poll for input
            if (inputPressed())
            {
                // tank actions
                if (sfw::getKey(TANK_FWRD))
                {
                    ex.tankMove = TankMovementOptions::FWRD;
                }
                else if (sfw::getKey(TANK_BACK))
                {
                    ex.tankMove = TankMovementOptions::BACK;
                }
                else if (sfw::getKey(TANK_LEFT))
                {
                    ex.tankMove = TankMovementOptions::LEFT;
                }
                else if (sfw::getKey(TANK_RIGT))
                {
                    ex.tankMove = TankMovementOptions::RIGHT;
                }
                else
                {
                    ex.tankMove = TankMovementOptions::HALT;
                }

                if (sfw::getKey(CANN_LEFT))
                {
                    ex.cannonMove = CannonMovementOptions::LEFT;
                }
                else if (sfw::getKey(CANN_RIGT))
                {
                    ex.cannonMove = CannonMovementOptions::RIGHT;
                }
                else
                {
                    ex.cannonMove = CannonMovementOptions::HALT;
                }

                if (sfw::getKey(TANK_FIRE))
                {
                    ex.fireWish = true;
                }

                // game actions
                if (sfw::getKey(GAME_QUIT))
                {
                    ex.msg = tankBattleMessage::QUIT;
                    break;
                }
            }

            if (isConnected)
            {
                // begin transmission
                memcpy_s(&msg, msgSize, &ex, sizeof(tankBattleHeader));
                dyad_write(s, &msg, msgSize);
            }
        }
    }

    dyad_close(s);
    dyad_shutdown();

    sfw::termContext();

    return EXIT_SUCCESS;
}