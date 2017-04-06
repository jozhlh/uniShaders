#ifndef _REGION_H
#define _REGION_H

#include "Cell.h"
#include <list>

//using namespace std;

class Region
{
public:
	Region() {}
	~Region() {}

	void SetNodeCoordinates(float x, float z) { nodeCoords = XMFLOAT2(x, z); }
	XMFLOAT2 GetNode() { return nodeCoords; }
	void GiveCell(Cell* m_Cell) { m_ChildCells.push_back(m_Cell); }

private:
	std::list<Cell*> m_ChildCells;
	XMFLOAT2 nodeCoords;
	int numOfCells;
};

#endif // !_REGION_H
