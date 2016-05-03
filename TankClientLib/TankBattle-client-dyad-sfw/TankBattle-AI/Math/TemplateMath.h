#pragma once
#include <cmath>

#define PI 3.14159265358979323846264338327950288419716939937510f
#define RAD2DEG 57.2958f
#define DEG2RAD 0.01745f
#define GRAVITY 50.0f

namespace cml
{
	template<typename T = float>
	T frac(const T div, const T num, const T den)
	{
		if (den == 0) { return 0; }
		return ((div / den) * num);
	}

	template<typename T = float>
<<<<<<< HEAD
	T expo(const T base, const int power)
=======
	T expo(const T base, const T power)
>>>>>>> origin/dev
	{
		float ans = base;
		if (power >= 2)
		{
			for (int a = 1; a < power; a++) { ans *= base; }
			return ans;
		}
		else if (power == 1) { return ans; }
		else if (power == 0) { return 1; }
		else if (power < 0)
		{
<<<<<<< HEAD
			for (int a = 1; a > power; a--) { ans *= base; }
=======
			for (int a = -1; a > power; a--) { ans *= base; }
>>>>>>> origin/dev
			return 1 / ans;
		}
	}

	template <typename T = float>
	T pythag(T a, T b) { return sqrt(expo(a, 2.0f) + expo(b, 2.0f)); }
<<<<<<< HEAD

	template <typename T = float>
	T slope(T m, T x, T b) { return (m * x) + b; }
=======
>>>>>>> origin/dev
}
