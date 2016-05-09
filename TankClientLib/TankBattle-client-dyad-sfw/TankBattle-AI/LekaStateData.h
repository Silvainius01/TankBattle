#pragma once
#include "../TankBattle-net/TankBattleHeaders.h"
#include "Math\Matrix3.h"
#include "Graph\graph.h"
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
		
	};

	struct s_hide
	{
		
	};

	struct s_search
	{
		
	};

	struct s_pursuit
	{
		
	};

	struct s_scan
	{
	
	};

	struct s_aim
	{
		
	};

	
	
}
//Used to store the optimum body turn direction.
extern int bif;

extern char *stateName(tankBot::BodyState bs);
extern char *stateName(tankBot::CannonState cs);