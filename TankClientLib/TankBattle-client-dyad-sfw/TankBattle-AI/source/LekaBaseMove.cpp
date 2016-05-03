#include "../Leka.h"
#include <ctime>
using namespace tankBot;

int bif = 0;

void Leka::rotateCannon(Vec2 target)
{
	if (c_dir.getAngleBetween(pos - target) > 0) { decisions.cannon_left = 1; }
	else { decisions.cannon_right = 1; }
}

//Note: This function will rotate the body the LEAST POSSIBLE AMOUNT,
//	meaning the tank can and should drive backward if it's faster
void Leka::rotateBody(Vec2 target)
{
	float ang = b_dir.getAngleBetween(pos - target);

	if (std::abs(ang) <= 2.5f || std::abs(180 - std::abs(ang)) <= 2.5f)
	{
		DRIVE = true, bif = 0;
		return;
	}
		

	if (bif == 0)
	{
		cout << std::abs(ang) << " " << std::abs(180 - std::abs(ang)) << endl;

		if (std::abs(ang) < std::abs(180 - std::abs(ang)))
			bif = -1;
		else
			bif = 1;
	}
		

	if (ang > 0) 
	{ 
		if (bif == -1)
			decisions.body_right = 1;
		else
			decisions.body_left = 1;
	}
	else 
	{ 
		if (bif == -1)
			decisions.body_left = 1;
		else
			decisions.body_right = 1;
	}
}

void Leka::drive(Vec2 target)
{
	switch (bif)
	{
	case -1: decisions.body_down = 1; break;
	case  1: decisions.body_up = 1; break;
	}

	if (onPath)
	{
		if (pos.getDistBetween(target) <= 1.0f)
			DRIVE = false; placeInPath++;
	}
	else if (pos.getDistBetween(target) <= 2.5f)
		DRIVE = false;
}

void Leka::findPath(Vec2 target)
{
	bool findPath = false;
	float m = pos.getSlopeBetween(target);
	float b = -(m * pos.x) + pos.y;
	ObstacleGroup og;

	if (map.nodes[38].open) map.addNode(pos);
	else map.nodes[38].data = pos;
	if (map.nodes[39].open) map.addNode(target);
	else map.nodes[39].data = target;

	for (int a = 0; a < 8; a++)
	{
		Vec2 test = { map.nodes[a].data.x, 0.0f };
		if (test.x > pos.x && test.x > target.x + 5) { continue; }
		if (test.x < pos.x && test.x < target.x - 5) { continue; }
		test.y = slope(m, test.x, b);
		if (test.getDistBetween(map.nodes[a].data) <= 5.0f)
		{
			switch (a)
			{
			case 0:case 1: og = ROCKS; break;
			case 2:case 3:case 4: og = BL_CANS; break;
			case 5:case 6:case 7: og = TR_CANS; break;
			}

			findPath = true, a = 8;
		}
	}

	if (findPath)
	{
		switch (og)
		{
		case ROCKS:
			map.connectNodes(&map.nodes[38], map.findClosestNodeTo(pos, 8, 19));
			map.connectNodes(map.findClosestNodeTo(target, 8, 19), &map.nodes[39]);
			break;
		case BL_CANS: 
			map.connectNodes(&map.nodes[38], map.findClosestNodeTo(pos, 20, 28));
			map.connectNodes(map.findClosestNodeTo(target, 20, 28), &map.nodes[39]);
			break;
		case TR_CANS: 
			map.connectNodes(&map.nodes[38], map.findClosestNodeTo(pos, 29, 37));
			map.connectNodes(map.findClosestNodeTo(target, 29, 37), &map.nodes[39]);
			break;
		}
	}
	else
		map.connectNodes(&map.nodes[38], &map.nodes[39]);

	map.dijkstra(&map.nodes[38], &map.nodes[39]);
	onPath = true, needsPath = false;
}

void Leka::locate()
{
	if (needsPath)
		findPath(target);

	if (onPath)
	{
		rotateBody(map.nodes[map.path[placeInPath]].data);

		if (DRIVE)
			drive(map.nodes[map.path[placeInPath]].data);

		if (placeInPath >= map.pathLength)
		{
			onPath = false;
			map.removeNode(map.nodes[38]);
			map.removeNode(map.nodes[39]);
			placeInPath = 0;
		}
	}
	else
	{
		
	}
}