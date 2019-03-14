#pragma once

#include "resource.h"
#include <Windows.h>

//structure grid
struct Grid {
	//int x_pos, y_pos;	//positions in x and y axis
	RECT tile;

	//mode to cotrol the type of tile
	//0 - normal
	//1 - start position
	//2 - end position;
	unsigned char mode = 0;
};

struct xyValues {
	int x, y;
};
