#include <iostream>
#include "../Leka.h"
using namespace tankBot;

void e_Enemy::setData(TankBattleStateData *info, int plyr)
{
	this->player = info->tacticoolData[plyr];
	Vec2 comp = Vec2{ player.lastKnownPosition[0], player.lastKnownPosition[2] };
	if (comp != this->lastPos[0])
	{
		for (int a = 2; a > 0; a--)
			this->lastPos[a] = this->lastPos[a - 1];

		this->lastPos[0] = comp;
	}
	lastDir = Vec2{ player.lastKnownDirection[0], player.lastKnownDirection[2] };
}

int Leka::closestEnemy()
{
	int result = 0;

	float pos  = ((enemy[0].lastPos[0] + Vec2{ 1.0f, 1.0f }) - this->pos).getMag();
	for (int a = 0; a < me->playerCount; a++)
	{
		float comp = (enemy[a].lastPos[0] - this->pos).getMag();
		if (comp < pos) { comp = pos; result = a; }
	}

	return result;
}
int Leka::weakestEnemy(bool rhp)
{
	float hp = 100;
	int p = -1;
	for (int a = 0; a < me->playerCount; a++)
	{
		float comp;
		switch (enemy[a].player.health)
		{
		case TankHealthStatus::HEALTHY: // x < 70
			comp = 85;
			break;
		case TankHealthStatus::HURT:	// 30 > 70 > x
			comp = 60;
			break;
		case TankHealthStatus::CRITICAL: // 0 < x  < 30
			comp = 10;
			break;
		default: continue;
		}
		if (comp < hp) { hp = comp; p = a; }
	}

	switch (rhp)
	{
	case true: return (int)hp;
	case false: return p;
	}
}

Vec2 e_Enemy::averagePos()
{
	Vec2 node;
	for (int a = 0; a < 3; a++)
		node += lastPos[a];

	return node /= 3;
}

cml::Graph Leka::LekaMap()
{
	return map;
}

char *stateName(BodyState bs)
{
	switch (bs)
	{
	case WANDER: return "WANDER";
	case SEARCH: return "SEARCH";
	case HIDE: return "HIDE";
	case PURSUE: return "PURSUE";
	}
}

char *stateName(CannonState cs)
{
	switch (cs)
	{
	case AIM: return "AIM";
	case SCAN: return "SCAN";
	}
}