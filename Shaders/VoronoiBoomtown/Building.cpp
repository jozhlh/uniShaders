// VoronoiBoomtown - Building.cpp
// Josh Hale - 2017
// A container for models which are placed on the grid

#include "Building.h"

Building::Building()
{
}

Building::~Building()
{
}

// Initialisation method
/// PARAMETERS
// model - a pointer to the model data for the asset which is to be placed
// width, height, length - the dimensions of the model(in cells)
// size - the amount the model is to be scaled from its default size
// importance - the buildings place in the order of buildings to be placed
void Building::Init(Model* model, float width, float height, float length, float size, int importance)
{
	asset = model;
	dimensions.x = width;
	dimensions.y = height;
	dimensions.z = length;
	priority = importance;
	scale = size;

}
