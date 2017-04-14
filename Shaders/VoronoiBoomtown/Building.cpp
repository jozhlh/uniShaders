#include "Building.h"


void Building::Init(Model* model, float width, float length, int importance)
{
	asset = model;
	dimensions.x = width;
	dimensions.y = length;
	priority = importance;
}
