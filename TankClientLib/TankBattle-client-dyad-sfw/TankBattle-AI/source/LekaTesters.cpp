#include "../Leka.h"

Vec2 ccts(float mx, float my)
{
	if (mx > 800) mx = 800;
	else if (mx < 0) mx = 0;
	if (my > 800) my = 800;
	else if (my < 0) my = 0;

	{
		mx -= HALF_WIDTH;
		mx /= 2 * HALF_WIDTH;
		mx *= 100;
	}
	{
		my -= HALF_HEIGHT;
		my /= WINDOW_HEIGHT;
		my *= 100;
	}

	return Vec2{ mx, my };
}