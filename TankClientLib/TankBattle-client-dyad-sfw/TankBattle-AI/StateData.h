#pragma once
#include "../TankBattle-net/TankBattleHeaders.h"
#include "Math\Matrix3.h"
using namespace cml;
using namespace tankNet;

namespace tankBot
{
	class CommandSend
	{
	public:
		int command[7] = { 0,0,0,0,0,0,0 };
		int &body_up = command[0],
			&body_down = command[2],
			&body_right = command[3],
			&body_left = command[1],
			&cannon_right = command[5],
			&cannon_left = command[4],
			&fire = command[6];

		CommandSend(void) { for (int a = 0; a < 7; command[a] = 0, a++); }
		CommandSend(int u, int d, int r, int l, int cr, int cl, int f) : body_up(u), body_down(d), body_right(r), body_left(l), cannon_right(cr), cannon_left(cl), fire(f) {}

		void set(int a, int val)
		{
			if (a > -1 && a < 7)
				command[a] = val;
		}

		CommandSend operator=(CommandSend cs)
		{
			for (int a = 0; a < 7; a++)
				command[a] = cs.command[a];
			return *this;
		}
		bool operator!=(CommandSend cs)
		{
			for (int a = 0; a < 7; a++)
			{
				if (command[a] != cs.command[a])
				{
					return true;
				}
			}
			return false;
		}
	};

	enum BodyState
	{
		WANDER,
		PURSUE,
		SEARCH,
		HIDE
	};

	enum CannonState
	{
		AIM,
		SCAN
	};

	struct s_wander
	{
		float maxTime;
		float travelTime = 0.0f;
		float dist;
		float minDist;
		int hasActiveNode = 0;
		int _minAngle, _maxAngle;
		Vec2 node;
		Vec2 mod;

		s_wander(void) { maxTime = 1.5f, _minAngle = 0, _maxAngle = 360, dist = 25.0f, minDist = 0.5f; }
		s_wander(float maxTravelTime, int minAngle, int maxAngle, float distance, float minDistance) : maxTime(maxTravelTime), _minAngle(minAngle), _maxAngle(maxAngle), dist(distance), minDist(minDistance) {}
	};

	struct s_hide
	{
		Vec2 cornerNodes[4] = { Vec2{45, 45}, Vec2{-45, -45}, Vec2{45, -45}, Vec2{-45, 45} };
		Vec2 &c_Upper = cornerNodes[0],
			 &c_Lower = cornerNodes[1],
			 &c_Right = cornerNodes[2],
			 &c_Left  = cornerNodes[3];
		int corner;
		bool hasFoundCorner = false;
		bool isMoving = false;
		s_wander WanderData;

		void calcWander();
	};

	struct s_search
	{
		s_wander WanderData;
		int huntedPlayer = -1;
		bool forceHunt = false;
		float maxHuntTime = 5.0f;
		float huntTime = 5.0f;
	};

	struct s_pursuit
	{
		s_wander WanderData;
		BodyState sentFrom;
		float cannonAngle;
		bool s_chase;
	};

	struct s_scan
	{
		bool left, right;
		float angleVariance = 30.0f;
	};

	struct s_aim
	{
		Vec2 target;
		bool allowFire;
	};
	
}

extern void rotateCannon(float currentAngle);
extern char *stateName(tankBot::BodyState bs);
extern char *stateName(tankBot::CannonState cs);