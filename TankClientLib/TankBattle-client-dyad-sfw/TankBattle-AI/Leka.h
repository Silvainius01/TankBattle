#pragma once

#include "StateData.h"

#define CS_NULL CommandSend{0,0,0,0,0,0,0}

namespace tankBot
{
	struct Enemy
	{
	public:

		TankTacticalInfo player;
		Vec2 lastPos[3];
		Vec2 lastDir;

		void setData(TankBattleStateData *info, int player);
		Vec2 averagePos();
	};

	class Leka
	{
	private:
		friend struct Enemy;

		BodyState s_Body = WANDER;
		CannonState s_Cannon = SCAN;
		tankNet::TankBattleStateData *me;

		Enemy enemy[3];
		Vec2 pos;
		float b_dir;
		float c_dir;

		BodyState optimalState();
		int weakestEnemy(bool returnHP = false);
		int closestEnemy();

		void wander(s_wander &data);
		void pursue();
		void search();
		void hide();

		void aim();
		void scan();

		void brain();

	public:
		Leka(void) {}
		void update(TankBattleStateData *info);
	};

	
}

extern tankBot::Leka leka;
extern tankBot::CommandSend decisions;
