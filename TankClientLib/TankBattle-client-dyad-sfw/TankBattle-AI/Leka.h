#pragma once

#include "LekaEnvData.h"
#include "sfwdraw.h"

//Leka Constants
#define CS_NULL CommandSend{0,0,0,0,0,0,0}
#define vel_Normal 12.0f
#define vel_Goo 3.6f

namespace tankBot
{
	class Leka
	{
	private:
		//Enviorment Data

		friend struct e_Enemy;
		e_Enemy enemy[3];
		cml::Graph map;
		int weakestEnemy(bool returnHP = false);
		int closestEnemy();
		void initMap();
		void cleanMap();

		//States
		//These are the overall brain of Leka, they take the data from update()
		//	and decide what state is best. They pass down the useful information
		//	to the routines, who do the peasent job of deciding where to go.

		BodyState s_Body = WANDER;
		CannonState s_Cannon = SCAN;
		BodyState optimalState();
		void stateMachine();

		//Routines
		//These functions crunch data handed to them from the state machines, 
		//	and are responsible for handing the results (a point of interest) 
		//  to the movement functions, who do the dirty work of getting there.
		//They also have limited surveying capabilities, incase the statemachine
		//	missed something and the current state needs to change ASAP. 
		tankNet::TankBattleStateData *me;
		Vec2 pos;
		Vec2 b_dir;
		Vec2 c_dir;

		void wander();
		void pursue();
		void search();
		void hide();
		void aim();
		void scan();

		//Movement data and functions
		//These functions are in charge of getting to the spot the routines want
		//	to go. They have no thinking abilities.
		//locate() is the driver, he simply keeps track of the various sub-states
		//	invloved with getting somewhere. He is at the mercy of the routines
		//	bidding, meaning he if he gets told to a new place each frame, he has
		//	to; however he can deny the new target if he is pathing, unless it is
		//	a life vs death situation.

		int placeInPath = 0;
		int targetedEnemy = -1;
		bool onPath = false;
		bool needsPath = false;
		bool DRIVE = false;
		Vec2 target = { -100, 0 };
		void rotateCannon(Vec2 target);
		void rotateBody(Vec2 target);
		void drive(Vec2 target);
		void findPath(Vec2 target);
		void locate();

	public:
		cml::Graph LekaMap();

		Leka(void)
		{
			initMap();
		}
		TankBattleCommand update(TankBattleStateData *info);
	};
}

extern tankBot::Leka leka;
extern tankBot::CommandSend decisions;

const int WINDOW_HEIGHT = 800;
const int WINDOW_WIDTH = 1200;
const float HALF_WIDTH = 400;
const float HALF_HEIGHT = 400;

//Makes a click on the client a point on the server, and returns it.
extern Vec2 ccts(float mx, float my);

//Makes a y on the server a y on the client
extern float gth(float val);
//Makes an x on the server an x on the client
extern float gtw(float val);
