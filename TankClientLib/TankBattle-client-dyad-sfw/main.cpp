#include <iostream>
#include <sstream>

#include "AutoAgent.h"
#include "HumanAgent.h"
#include "TankBattle-AI\Leka.h"
#include "TankBattleNet.h"
#undef NONE     // sfw defines NONE as one of its colors; we won't be needing that

#define ORANGE 0xf89d28ff;
#define GRAV -9.81f
#define FIREPOINT 1.7f

using std::stringstream;
tankNet::TankBattleCommand ex;

// use O or P to switch between players
const char GAME_QUIT = 'L';
const char GAME_TOGGLE_AI    = 'O';
const char GAME_TOGGLE_HUMAN = 'P';

//const int WINDOW_HEIGHT = 800;
//const int WINDOW_WIDTH  = 400;


void printTacticalData(tankNet::TankBattleStateData * state);

float gth(float val)
{
	return ((val / 100) * WINDOW_HEIGHT) + HALF_HEIGHT;
}

float gtw(float val)
{
	return ((val / 100) * HALF_WIDTH * 2) + HALF_WIDTH;
}

void printTacticalData(tankNet::TankBattleStateData * state);

enum ServerName
{
	HOME,
	TUNG,
	JACOB,
	DEDICATED
};
char *getIPAdress(ServerName sn)
{
	switch (sn)
	{
	case TUNG: return "10.15.22.46";
	case JACOB: return "10.15.22.81";
	case DEDICATED: return "10.15.22.31";
	default: return "";
	}
}

unsigned g_FONT = 0;
unsigned g_DMAP = 1;
unsigned g_DOTS = 2;
unsigned g_BUCK = 3;
unsigned g_BACK = 4;

int main(int argc, char** argv)
{
	char * serverIPAddress = getIPAdress(HOME);

    // handle console arguments
    if (argc > 2)
    {
        std::cout << "Unsupported number of arguments." << std::endl;
        std::cout << "Specify the IP address of the target server. Ex: " << argv[0] << " 127.0.0.1" << std::endl;

        return EXIT_FAILURE;
    }
    else if (argc == 2)
        serverIPAddress = argv[1]; 

    // initialize networking
    if (serverIPAddress[0] == '\0') tankNet::init();
    else                            tankNet::init(serverIPAddress);


    // block further execution until the server responds
    // or until the client gives up on connecting
    while (!tankNet::isProvisioned()) tankNet::update();
    

    // if a connection was successful...
    if (tankNet::isConnected() && tankNet::isProvisioned())
    {
        auto serverData = tankNet::recieve();

        // initialize SFW and assets
		sfw::initContext(WINDOW_WIDTH, WINDOW_HEIGHT, "TankController");
		sfw::setBackgroundColor(BLACK);
		g_FONT = sfw::loadTextureMap("./res/fontmap.png", 16, 16);
		g_DMAP = sfw::loadTextureMap("./res/MapDiagramCrop.png");
		g_DOTS = sfw::loadTextureMap("./res/circArrow.png");
		g_BUCK = sfw::loadTextureMap("./res/Bucket.png");
		g_BACK = sfw::loadTextureMap("./res/black.jpg");

        AutoAgent  aiAgent;
        HumanAgent huAgent;
        bool isHuman = false;

        ////////////////////////////////////////////////////////////
        /////////// Game Loop!
        while (sfw::stepContext() && tankNet::isConnected() && tankNet::isProvisioned() && !sfw::getKey(GAME_QUIT))
        {
            // check TCP streams via dyad
            tankNet::update();
            if (tankNet::isConnected() == false) break;
			sfw::drawTexture(g_FONT, sfw::getMouseX(), sfw::getMouseY(), 20, 20, 0, true, 88, MAGENTA);

            tankNet::TankBattleStateData * state = tankNet::recieve();

            printTacticalData(state);
        
            // Toggle between human or computer player, for debug
            if (sfw::getKey(GAME_TOGGLE_AI))    isHuman = false;
            if (sfw::getKey(GAME_TOGGLE_HUMAN)) isHuman = true;

            // use human agent or AI agent to determine the TBC
            
            // begin transmission
			ex = leka.update(state);
			tankNet::send(ex);
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

unsigned getColor(int player)
{
	switch (player % 4)
	{
	case 0: return RED;
	case 1: return CYAN;
	case 2: return GREEN;
	case 3: return ORANGE;
	}

	return WHITE;
}
const float blastRad = gtw(5.0f) - HALF_WIDTH;

void drawArc(float radius, float angleStart, float angleStop, Vector2 pos, unsigned color = WHITE)
{
	angleStart *= DEG2RAD, angleStop *= DEG2RAD;
	float step = (10 * DEG2RAD);

	while (angleStop < angleStart) { angleStop += 2 * PI; }

	for (float a = angleStart; a < angleStop; a += step)
		if (a + step < angleStop)
			sfw::drawLine(gtw(pos.x + radius * cos(a)), gth(pos.y + radius * sin(a)), gtw(pos.x + radius * cos(a + step)), gth(pos.y + radius * sin(a + step)), color);
		else
			sfw::drawLine(gtw(pos.x + radius * cos(a)), gth(pos.y + radius * sin(a)), gtw(pos.x + radius * cos(angleStop)), gth(pos.y + radius * sin(angleStop)), color);

}

void drawMap()
{
	for (int a = 0; a < leka.LekaMap().getTotalNodes(); a++)
	{
		if (!leka.LekaMap().nodes[a].open)
		{
			sfw::drawCircle(gtw(leka.LekaMap().nodes[a].data.x), gth(leka.LekaMap().nodes[a].data.y), 10, 4, MAGENTA);

			if (leka.LekaMap().nodes[a].edge != nullptr)
				for (int b = 0, index = 0; b < leka.LekaMap().nodes[a].totalEdges; b++)
				{
					index = leka.LekaMap().nodes[a].edge[b].index;
					sfw::drawLine(gtw(leka.LekaMap().nodes[a].data.x), gth(leka.LekaMap().nodes[a].data.y), gtw(leka.LekaMap().nodes[index].data.x), gth(leka.LekaMap().nodes[index].data.y), MAGENTA);
				}
		}
	}
}

bool firing = false;
bool hasLanded = true;
float fly_time = 0.0f;
Vector2 bulletPath;
Vector2 bulletPos;
void drawPlayer(int id, Vector2 pos, Vector2 ang, Vector2 can, bool canFire = true)
{
	float blerg = 0.0f;
	float dist = gtw(15.0f) - HALF_WIDTH;
	float blah = angle(can) * RAD2DEG;
	float blah2 = blah - 45.0f; blah += 45.0f;
	float vlx = gtw((can.x * 19) + pos.x);
	float vly = gth((can.y * 19) + pos.y);
	float blx;
	float bly;
	unsigned color = getColor(id);
	Vector2 relPos = Vector2{ gtw(pos[0]), gth(pos[1]) };
	Vector2 view1 = Vector2{ std::cos((blah)* DEG2RAD) * 50, std::sin((blah)* DEG2RAD) * 50 };
	Vector2 view2 = Vector2{ std::cos((blah2)* DEG2RAD) * 50, std::sin((blah2)* DEG2RAD) * 50 };

	if (id == -5)
	{
		if (canFire && ex.fireWish)
		{
			firing = true;
			hasLanded = false;
			bulletPos = pos;
			bulletPath = can;
			fly_time = 0.0f;
		}

		if (firing)
		{
			if (!canFire) { fly_time += sfw::getDeltaTime(); }
			if ((GRAV * fly_time) + FIREPOINT < 0.0f) { hasLanded = true; }
			else { dist *= fly_time; }
		}

		//Bullet
		if (!hasLanded)
			sfw::drawTexture(g_BUCK, gtw((bulletPath.x * dist) + bulletPos.x), gth((bulletPath.y * dist) + bulletPos.y), 15, 15, (angle(bulletPath) * RAD2DEG) + 180);
	}


	//Cannon
	sfw::drawTexture(g_DOTS, relPos.x, relPos.y, 10.0f, 10.0f, blah2);
	//Tank
	sfw::drawTexture(g_DOTS, relPos.x, relPos.y, 20.0f, 20.0f, (angle(ang) * RAD2DEG) - 45, true, 0, color);
	//Cannon Range
	sfw::drawLine(relPos.x, relPos.y, vlx, vly, color);
	//Shell Blast Radius
	sfw::drawCircle(vlx, vly, blastRad, 12, color);
	//Tank Direction
	sfw::drawLine(relPos.x, relPos.y, relPos.x + ang.x * 30, relPos.y + ang.y * 30, color);
	//Tank LoS
	sfw::drawLine(relPos.x, relPos.y, gtw(view1.x + pos.x), gth(view1.y + pos.y), color);
	sfw::drawLine(relPos.x, relPos.y, gtw(view2.x + pos.x), gth(view2.y + pos.y), color);
	drawArc(50, blah2, blah, pos, color);

}

void printTacticalData(tankNet::TankBattleStateData * state)
{
	// diagnostic report of current statead
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

	drawMap();

		sfw::drawString(g_FONT, debugStrings.str().c_str(), 805, WINDOW_HEIGHT, 15, 15);

	Vector2 angle = Vector2::fromXZ(state->forward);
	Vector2 cnAng = Vector2::fromXZ(state->cannonForward);

	sfw::drawTexture(g_BACK, 1000, 400, 400, 800);

	drawPlayer(-5, Vector2::fromXZ(state->position), angle, cnAng, state->canFire);

	for (int a = 0; a < state->tacticoolCount; a++)
	{
		if (state->tacticoolData[a].isAlive)
		{
			angle = Vector2::fromXZ(state->tacticoolData[a].lastKnownDirection);
			cnAng = Vector2::fromXZ(state->tacticoolData[a].lastKnownCannonForward);
			drawPlayer(state->tacticoolData[a].playerID, Vector2::fromXZ(state->tacticoolData[a].lastKnownPosition), angle, cnAng);
		}
		else
			sfw::drawTexture(g_FONT, gtw(state->tacticoolData[a].lastKnownPosition[0]), gth(state->tacticoolData[a].lastKnownPosition[2]), 20, 20, 0, true, 88, getColor(state->tacticoolData[a].playerID));
	}

	sfw::drawTexture(g_DMAP, HALF_WIDTH, HALF_HEIGHT, HALF_WIDTH * 2, WINDOW_HEIGHT);
}