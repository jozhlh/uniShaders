#include "Building.h"


Building::Building()
{
}

Building::~Building()
{
}

void Building::Init(Model* model, float width, float height, float length, float size, int importance)
{
	asset = model;
	dimensions.x = width;
	dimensions.y = height;
	dimensions.z = length;
	priority = importance;
	scale = size;

}
