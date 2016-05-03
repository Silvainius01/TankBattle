#include <iostream>
#include "../Leka.h"
using namespace tankBot;

Leka leka;
CommandSend decisions;
Vec2 manualBodTarget;
Vec2 manualCanTarget;
bool playerOverride = true;
bool first = true;
bool clicked2 = false;
char controls[7] = { 'W','A','S','D','Q','E','F' };

TankBattleCommand Leka::update(TankBattleStateData *info)
{
	decisions = CS_NULL;
	this->me = info;
	if (me->playerCount > 3) { me->playerCount = 3; }
	pos = Vec2{ me->position[0], me->position[2] };
	b_dir = Vec2{ me->forward[0], me->forward[2] };
	c_dir = Vec2{ me->cannonForward[0], me->cannonForward[2] };
	for (int a = 0; a < me->playerCount; enemy[a].setData(me, a), a++);

	if (playerOverride)
	{
		for (int a = 0; a < 7; a++)
			if (sfw::getKey(controls[a]))
				decisions.set(a, 1);

		if (!decisions.body_left && !decisions.body_right && !decisions.body_up && !decisions.body_down)
		{

			if (!clicked2 && sfw::getMouseButton(0))
			{
				target = ccts(sfw::getMouseX(), sfw::getMouseY());
				clicked2 = true, bif = 0, needsPath = true;
			}
			else if (!sfw::getMouseButton(0))
				clicked2 = false;

			//rotateBody(manualBodTarget);
			if(target.x != -100)
				locate();
		}
		else
			bif = 0;

		if (!decisions.cannon_left && !decisions.cannon_right)
		{
			if (!clicked2 && sfw::getMouseButton(1))
			{
				manualCanTarget = ccts(sfw::getMouseX(), sfw::getMouseY());
				clicked2 = true;
			}
			else if (!sfw::getMouseButton(0))
				clicked2 = false;

			rotateCannon(manualCanTarget);
		}
	}
	else
	{
		stateMachine();
	}

	tankNet::TankBattleCommand ex;
	ex.msg = tankNet::TankBattleMessage::GAME;
	ex.tankMove = tankNet::TankMovementOptions::HALT;
	ex.cannonMove = tankNet::CannonMovementOptions::HALT;

		ex.tankMove = decisions.body_up ? tankNet::TankMovementOptions::FWRD :
			decisions.body_down ? tankNet::TankMovementOptions::BACK :
			decisions.body_left ? tankNet::TankMovementOptions::LEFT :
			decisions.body_right ? tankNet::TankMovementOptions::RIGHT :
			tankNet::TankMovementOptions::HALT;

		ex.cannonMove = decisions.cannon_left ? tankNet::CannonMovementOptions::LEFT :
			decisions.cannon_right ? tankNet::CannonMovementOptions::RIGHT :
			tankNet::CannonMovementOptions::HALT;

		ex.fireWish = decisions.fire;
	
	return ex;
}

void tankBot::Leka::stateMachine()
{
	if (s_Body == WANDER || s_Body == HIDE) { s_Body = optimalState(); }

	//cout << stateName(s_Body) << " Node Status: ";

	switch (s_Body)
	{
	case WANDER:	//wander(); break;
	case HIDE:		hide(); break;
	case SEARCH:	search(); break;
	case PURSUE:	pursue();
	}

	switch (s_Cannon)
	{
	case AIM:	aim(); break;
	case SCAN:	scan();
	}

	//cout << stateName(s_Cannon) << endl;

}

BodyState Leka::optimalState()
{
	float hp = weakestEnemy(true);

	if (hp < me->currentHealth) { return SEARCH; }
	//else if(hp < 65) { return HIDE; }

	return WANDER;
}

void Leka::initMap()
{
	//Rocks 0-1
	map.addNode(Vec2{ -5.0f, 15.0f });
	map.addNode(Vec2{ 5.0f, 5.0f });
	//BL cans 2-4
	map.addNode(Vec2{ -35.0f, -35.0f });
	map.addNode(Vec2{ -25.0f, -35.0f });
	map.addNode(Vec2{ -35.0f, -25.0f });
	//TR Cans 5-7
	map.addNode(Vec2{ 35.0f, 35.0f });
	map.addNode(Vec2{ 25.0f, 35.0f });
	map.addNode(Vec2{ 35.0f, 25.0f });

	//Rocks pathing 8-19
	map.addNode(Vec2{ -2.5f, -2.5f });
	map.addNode(map.nodes[1].data + Vec2{ 0.0f, -10.0f });
	map.addNode(Vec2{ 12.5f, -2.5f });
	map.addNode(map.nodes[1].data + Vec2{ 10.0f, 0.0f });
	map.addNode(Vec2{ 12.5f, 12.5f });
	map.addNode(map.nodes[0].data + Vec2{ 10.0f, 0.0f });
	map.addNode(Vec2{ 2.5f, 22.5f });
	map.addNode(map.nodes[0].data + Vec2{ 0.0f, 10.0f });
	map.addNode(Vec2{ -12.5f, 22.5f });
	map.addNode(map.nodes[0].data + Vec2{ -10.0f, 0.0f });
	map.addNode(Vec2{ -12.5f, 7.5f });
	map.addNode(map.nodes[0].data + Vec2{ 0.0f, -10.0f });
	map.connectNodes(&map.nodes[8], &map.nodes[19], 0.0f, false);
	for (int a = 8; a < 19; a++) map.connectNodes(&map.nodes[a], &map.nodes[a + 1], 0.0f, false);
	//BL Cans pathing 20-28
	map.addNode(Vec2{ -45.0f, -25.0f });
	map.addNode(Vec2{ -45.0f, -35.0f });
	map.addNode(Vec2{ -42.5f, -42.5f });
	map.addNode(Vec2{ -35.0f, -45.0f });
	map.addNode(Vec2{ -25.0f, -45.0f });
	map.addNode(Vec2{ -17.5f, -42.5f });
	map.addNode(Vec2{ -15.0f, -35.0f });
	map.addNode(Vec2{ -17.5f, -27.5f });
	map.addNode(Vec2{ -25.0f, -25.0f });
	for (int a = 20; a < 28; a++) map.connectNodes(&map.nodes[a], &map.nodes[a + 1], 0.0f, false);
	//TR Cans pathing 29-37
	map.addNode(Vec2{ 25.0f, 15.0f });
	map.addNode(Vec2{ 25.0f, 25.0f });
	map.addNode(Vec2{ 17.5f, 27.5f });
	map.addNode(Vec2{ 15.0f, 35.0f });
	map.addNode(Vec2{ 17.5f, 42.5f });
	map.addNode(Vec2{ 25.0f, 45.0f });
	map.addNode(Vec2{ 35.0f, 45.0f });
	map.addNode(Vec2{ 42.5f, 42.5f });
	map.addNode(Vec2{ 45.0f, 35.0f });
	for (int a = 29; a < 37; a++) map.connectNodes(&map.nodes[a], &map.nodes[a + 1], 0.0f, false);

	//Pos and Tar nodes
	map.addNode(Vec2()), map.addNode(Vec2());
	map.nodes[38].open = true, map.nodes[39].open = true;
}