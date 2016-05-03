#include "Leka.h"


void rotateCannon(float currentAngle)
{
	if (currentAngle < 0) { decisions.cannon_left = 1; }
	else { decisions.cannon_right = 1; }
}