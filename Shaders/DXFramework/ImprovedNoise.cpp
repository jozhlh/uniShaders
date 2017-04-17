#include "ImprovedNoise.h"

ImprovedNoise::ImprovedNoise()
{
}

ImprovedNoise::~ImprovedNoise()
{
}

double ImprovedNoise::fade(double t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double ImprovedNoise::myLerp(double t, double a, double b)
{
	return a + t * (b - a);
}

double ImprovedNoise::grad(int hash, double x, double y, double z)
{
	int h = hash & 15; // CONVERT LO 4 BITS OF HASH CODE
	double u = h < 8 ? x : y, // INTO 12 GRADIENT DIRECTIONS.
		v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

double ImprovedNoise::noise(double x, double y, double z)
{
	int X = (int)floor(x) & 255, // FIND UNIT CUBE THAT
		Y = (int)floor(y) & 255, // CONTAINS POINT.
		Z = (int)floor(z) & 255;

	x -= floor(x); // FIND RELATIVE X,Y,Z
	y -= floor(y); // OF POINT IN CUBE.
	z -= floor(z);

	double u = fade(x), // COMPUTE FADE CURVES
		v = fade(y), // FOR EACH OF X,Y,Z.
		w = fade(z);

	int A = q[X] + Y, AA = q[A] + Z, AB = q[A + 1] + Z, // HASH COORDINATES OF
		B = q[X + 1] + Y, BA = q[B] + Z, BB = q[B + 1] + Z; // THE 8 CUBE CORNERS,

	return myLerp(w, myLerp(v, myLerp(u, grad(q[AA], x, y, z), // AND ADD
		grad(q[BA], x - 1, y, z)), // BLENDED
		myLerp(u, grad(q[AB], x, y - 1, z), // RESULTS
			grad(q[BB], x - 1, y - 1, z))), // FROM  8
		myLerp(v, myLerp(u, grad(q[AA + 1], x, y, z - 1), // CORNERS
			grad(q[BA + 1], x - 1, y, z - 1)), // OF CUBE
			myLerp(u, grad(q[AB + 1], x, y - 1, z - 1),
				grad(q[BB + 1], x - 1, y - 1, z - 1))));
}
