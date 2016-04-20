#pragma once

#include "../TankBattle-net/TankBattleHeaders.h"
#include "Math\Matrix3.h"
using namespace cml;
using namespace tankNet;

#define CS_NULL CommandSend{0,0,0,0,0,0,0}

namespace tankBot
{
	struct Enemy
	{
	public:

		TankTacticalInfo player;
		Vec2 lastPos[3];
		float lastDir;

		void setData(TankBattleStateData *info, int player);
	};

	class Leka
	{
	private:
		friend struct Enemy;

		enum State
		{
			WANDER,
			PURSUE,
			SEARCH,
			EVADE
		};

		State state = WANDER;
		tankNet::TankBattleStateData *me;

		Enemy enemy[3];
		Vec2 pos;
		float b_dir;
		float c_dir;

		State optimalState();

		void wander();
		void pursue(int player);
		void search(int player);
		void evade();

		void brain();

	public:
		Leka(void) {}
		void update(TankBattleStateData *info);
	};

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

	struct s_wander
	{
		int hasActiveNode = 0;
		float travelTime = 1.0f;
		Vec2 node;
	};
}

extern tankBot::Leka leka;
extern tankBot::CommandSend decisions;
