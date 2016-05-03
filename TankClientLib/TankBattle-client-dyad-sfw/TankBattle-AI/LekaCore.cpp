#include <iostream>
#include "Leka.h"
#include "sfwdraw.h"
using namespace tankBot;

Leka leka;
CommandSend decisions;
s_wander WanderData;
bool playerOverride = false;
char controls[7] = { 'W','A','S','D','Q','E','F' };

void Leka::update(TankBattleStateData *info)
{
	decisions = CS_NULL;

	if (playerOverride)
	{
		for (int a = 0; a < 7; a++)
			if (sfw::getKey(controls[a]))
				decisions.set(a, 1);
		return;
	}

	this->me = info;
	if (me->playerCount > 3) { me->playerCount = 3; }
	pos = Vec2{ me->position[0], me->position[2] };
	b_dir = Vec2{ me->forward[0], me->forward[2] }.getAngle();
	c_dir = Vec2{ me->cannonForward[0], me->cannonForward[2] }.getAngle();
	for (int a = 0; a < me->playerCount; enemy[a].setData(me, a), a++);
	brain();
}

void tankBot::Leka::brain()
{
	if (s_Body == WANDER || s_Body == HIDE) { s_Body = optimalState(); }

	//cout << stateName(s_Body) << " Node Status: ";

	switch (s_Body)
	{
	case WANDER:	wander(WanderData); break;
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