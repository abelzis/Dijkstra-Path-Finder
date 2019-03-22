#pragma once

#include "resource.h"
#include <cmath>
#include <list>
#include "stdafx.h"
#include <Windows.h>

using std::list;


#define GRID_SIZE 20

//structure grid
struct Grid {
	//int x_pos, y_pos;	//positions in x and y axis
	RECT tile;

	//mode to cotrol the type of tile
	//0 - normal
	//1 - start position
	//2 - end position;
	//3 - obstacle
	unsigned char mode = 0;
	
	//mode to control visualization of dijkstra path finding
	//1 - dijkstra path
	//2 - dijkstra selected path
	unsigned char path_mode = 0;

	unsigned char r = 0, g = 0, b = 0;	//rgb

	//value about tile's difficulty to pass through
	unsigned char value = 1;
};

struct xyValues {
	INT x, y;
};



void createGrid(HWND& hWnd, HDC hdc, int width, int height);