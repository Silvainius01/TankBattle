#include <iostream>
#include "brain.h"
#include "../libs/sfw/sfwdraw.h"
using namespace tankBot;

Leka leka;
CommandSend decisions;
s_wander WanderData;

bool playerOverride = false;
char controls[7] = { 'W','A','S','D','Q','E','F' };

//-1: right
//0: Not turning
//1: left
int isTurning = 0;

void Enemy::setData(TankBattleStateData *info, int plyr)
{
	this->player = info->tacticoolData[plyr];
	Vec2 comp = Vec2{ player.lastKnownPosition[0], player.lastKnownPosition[2] };
	if (comp != this->lastPos[0])
	{
		for (int a = 2; a > 0; a--)
			this->lastPos[a] = this->lastPos[a - 1];

		this->lastPos[0] = comp;
	}
	lastDir = Vec2{ player.lastKnownDirection[0], player.lastKnownDirection[2] }.getAngle();
}

void Leka::update(TankBattleStateData *info)
{
	this->me = info;
	pos = Vec2{ me->position[0], me->position[2] };
	for (int a = 0; a < 3; enemy[a].setData(me, a), a++);
	b_dir = Vec2{ me->forward[0], me->forward[2] }.getAngle();
	c_dir = Vec2{ me->cannonForward[0], me->cannonForward[2] }.getAngle();


	brain();
}

void Leka::wander()
{
	float angle;
	switch (WanderData.hasActiveNode)
	{
	case 1:
		angle = pos.getAngleBetween(WanderData.node) - b_dir;
		if (abs(angle) > 2.0f)
		{
			if (angle > 0) { decisions.body_left = 1; }
			else { decisions.body_right = 1; }
			WanderData.travelTime += sfw::getDeltaTime();
			if (WanderData.travelTime >= WanderData.maxTime)
				WanderData.hasActiveNode = 0;
		}
		else
		{
			WanderData.travelTime = 0.0f;
			WanderData.hasActiveNode = 2;
		}
		break;
	case 2:
		decisions.body_up = 1;
		WanderData.travelTime += sfw::getDeltaTime();
		if ((WanderData.node - pos).getMag() <= 0.5f || WanderData.travelTime >= WanderData.maxTime)
		{
			WanderData.hasActiveNode = 0;
		}
		break;
	default:
		angle = (std::rand() % 360) * DEG2RAD;
		WanderData.node = Vec2{ cos(angle), sin(angle) } *25.0f;
		//WanderData.node += pos;
		WanderData.hasActiveNode = 1;
		WanderData.travelTime = 0.0f;
		if (WanderData.node.x > 45.0f) { WanderData.node.x = 45.0f; }
		else if (WanderData.node.x < -45.0f) { WanderData.node.x = -45.0f; }
		if (WanderData.node.y > 45.0f) { WanderData.node.y = 45.0f; }
		else if (WanderData.node.y < -45.0f) { WanderData.node.y = -45.0f; }

		WanderData.node.print();
		cout << " " << pos.getAngleBetween(WanderData.node);
		cout << endl;
	}
	
}


void tankBot::Leka::brain()
{
	decisions = CS_NULL;

	if (playerOverride)
	{
		for (int a = 0; a < 7; a++)
			if (sfw::getKey(controls[a]))
				decisions.set(a, 1);
	}
	else
	{
		wander();
	}
}