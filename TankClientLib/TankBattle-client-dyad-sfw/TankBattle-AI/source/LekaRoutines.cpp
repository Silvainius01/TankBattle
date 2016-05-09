#include <iostream>
#include <ctime>
#include "../Leka.h"
using namespace tankBot;

#define BulletDist 14.65f

float t_PursueTime;

void Leka::wander()
{

}

void Leka::hide()
{
	
}

void Leka::search()
{
	
}

void Leka::pursue()
{
	
}

void Leka::scan()
{
	decisions.body_right = 1;
	decisions.cannon_right = 1;

	for (int a = 0; a < 3; a++)
	{
		if (enemy[a].player.inSight)
		{
			s_Cannon = AIM;
			needsPath = true;
			targetedEnemy = a;
			break;
		}
	}
}

void Leka::aim()
{
	decisions.fire = 1;

	if (enemy[targetedEnemy].player.inSight)
	{
		target = enemy[targetedEnemy].lastPos[0];
	}
	else
		s_Cannon = SCAN;
}