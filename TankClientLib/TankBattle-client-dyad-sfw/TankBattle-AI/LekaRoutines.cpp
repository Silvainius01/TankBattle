#include <iostream>
#include <ctime>
#include "Leka.h"
#include "../libs/sfw/sfwdraw.h"
using namespace tankBot;

#define BulletDist 14.65f

float angle = std::time(NULL);
s_hide HideData;
s_search SearchData;
s_pursuit PursuitData;
s_scan ScanData;
s_aim AimData;

void Leka::wander(s_wander &data)
{
	//cout << data.hasActiveNode << "  ";

	switch (data.hasActiveNode)
	{
	case 1:
		angle = abs(c_dir - b_dir);
		if (angle < 0)
			decisions.cannon_right = 1;
		else
			decisions.cannon_left = 1;

		angle = pos.getAngleBetween(data.node) - b_dir;
		if (abs(angle) > 5.0f)
		{
			if (angle > 0) { decisions.body_left = 1; }
			else { decisions.body_right = 1; }
			data.travelTime += sfw::getDeltaTime();

			//This is protection from low packet rates.
			if (data.travelTime >= 1.05f) 
				data.hasActiveNode = 0;
		}
		else
		{
			data.travelTime = 0.0f;
			data.hasActiveNode = 2;
		}
		break;
	case 2:
		decisions.body_up = 1;
		data.travelTime += sfw::getDeltaTime();
		if (data.travelTime >= data.maxTime && data.maxTime > 0.0f) 
			data.hasActiveNode = 0;
		else if (data.minDist > 0.0f && abs(data.node - pos) <= data.minDist)
			data.hasActiveNode = 0;

		break;
	default:
		angle = ((std::rand() % data._maxAngle) + data._minAngle) * DEG2RAD;
		data.node = Vec2{ cos(angle), sin(angle) } * data.dist;
		data.node += data.mod;
		data.hasActiveNode = 1;
		data.travelTime = 0.0f;
		if (data.node.x > 45.0f) { data.node.x = 45.0f; }
		else if (data.node.x < -45.0f) { data.node.x = -45.0f; }
		if (data.node.y > 45.0f) { data.node.y = 45.0f; }
		else if (data.node.y < -45.0f) { data.node.y = -45.0f; }

		if (data.maxTime + data.minDist < 0.0f)
			data.maxTime = 1.5f, data.minDist = 0.5f;
	}
}

void Leka::hide()
{
	Vec2 runFromHere = enemy[closestEnemy()].lastPos[0];
	if ((runFromHere - pos).getMag() < 20.0f && HideData.hasFoundCorner && HideData.isMoving) 
		{ HideData.hasFoundCorner = false, HideData.isMoving = false; }

	if (!HideData.hasFoundCorner)
	{
		float comp = ((HideData.cornerNodes[0] - Vec2{ 1.0f, 1.0f }) - runFromHere).getMag();
		for (int a = 0; a < me->playerCount; a++)
		{
			float blerg = (HideData.cornerNodes[a] - runFromHere).getMag();
			if (blerg > comp) { comp = blerg, HideData.corner = a; }
		}
		HideData.hasFoundCorner = true;
		HideData.isMoving = false;
		HideData.WanderData = s_wander{ 7.0f, 0, 0, comp, 0 };
		HideData.WanderData.node = HideData.cornerNodes[HideData.corner];
		HideData.calcWander();
		wander(HideData.WanderData);
	}
	else
	{
		if (HideData.WanderData.hasActiveNode == 0 && !HideData.isMoving)
		{
			HideData.WanderData = s_wander{ 1.0f, HideData.WanderData._minAngle, HideData.WanderData._maxAngle, 7.5f, 0.5f };
			HideData.WanderData.mod = HideData.cornerNodes[HideData.corner];
			HideData.isMoving = true;
		}
		
		wander(HideData.WanderData);
	}
}

void Leka::search()
{
	if (SearchData.huntedPlayer == -1 || SearchData.forceHunt)
	{
		if(SearchData.huntedPlayer == -1)
			SearchData.huntedPlayer = weakestEnemy();

		Vec2 node = enemy[SearchData.huntedPlayer].averagePos();		

		SearchData.huntTime = SearchData.maxHuntTime;
		SearchData.WanderData = s_wander{ 2.0f, 0, 360, 10.0f, 5.0f };
		SearchData.WanderData.mod = node;
		SearchData.WanderData.node = node;
		SearchData.WanderData.hasActiveNode = 1;
		SearchData.forceHunt = false;
	}
	else if (SearchData.huntTime > 0.0f)
		SearchData.huntTime -= sfw::getDeltaTime();
	else
	{
		SearchData.huntedPlayer = -1;
		PursuitData.s_chase = false;
		s_Body = WANDER;
		return;
	}

	wander(SearchData.WanderData);
	if (enemy[SearchData.huntedPlayer].player.inSight)
		s_Body = PURSUE, s_Cannon = AIM, PursuitData.sentFrom = SEARCH;
}

void Leka::pursue()
{
	AimData.target = (enemy[SearchData.huntedPlayer].lastDir * 12.0f) + enemy[SearchData.huntedPlayer].lastPos[0];
	
	PursuitData.cannonAngle = AimData.target.getAngleBetween(pos);
	if (!PursuitData.s_chase)
	{
		PursuitData.WanderData = s_wander{ 0.0f, 0, 0, 0, 0.0f };
		PursuitData.WanderData.node = AimData.target;
		PursuitData.WanderData.hasActiveNode = 1;
		PursuitData.s_chase = true;
	}
	else
	{
		if (PursuitData.WanderData.hasActiveNode == 0)
			PursuitData.WanderData.hasActiveNode = 2;
		
		wander(PursuitData.WanderData);
		
		if (!enemy[SearchData.huntedPlayer].player.inSight)
			s_Body = SEARCH, s_Cannon = SCAN, PursuitData.s_chase = false, SearchData.forceHunt = true;
	}
}

void Leka::scan()
{
	if (b_dir + ScanData.angleVariance > c_dir && !ScanData.right)
		{ decisions.cannon_left = 1; return; }
	else
		ScanData.right = true;

	if (b_dir - ScanData.angleVariance < c_dir && ScanData.right)
		{ decisions.cannon_right = 1; return; }
	else
		ScanData.right = false;

	if (!PursuitData.s_chase)
	{
		for (int a = 0; a < me->playerCount; a++)
		{
			if (enemy[a].player.inSight && s_Body != SEARCH)
			{
				s_Body = PURSUE;
				SearchData.huntedPlayer = a;
				PursuitData.sentFrom = s_Body;
			}
		}
	}
	
}

void Leka::aim()
{
	float comp = (PursuitData.cannonAngle - c_dir);
	if (abs(comp) < 1.0f)
	{
		rotateCannon(angle);
	}
	else if(abs(abs(AimData.target) - abs(pos)) < BulletDist)
		decisions.fire = 1;
}