#pragma once
#include "LekaStateData.h"

namespace tankBot
{
	struct e_Enemy
	{
	public:

		TankTacticalInfo player;
		Vec2 lastPos[3];
		Vec2 lastDir;

		void setData(TankBattleStateData *info, int player);
		Vec2 averagePos();
	};

	enum ObstacleGroup
	{
		TR_CANS,
		ROCKS,
		BL_CANS
	};
}