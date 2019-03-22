#pragma once

#include <Windows.h>
#include "DijkstraAlgorithm.h"


void dijkstraAlgorithm(HWND hWnd, HDC hdc, xyValues end, Grid grid[][GRID_SIZE], INT x, INT y, INT& speed);
void dijkstraShortestPath(INT x, INT y, Grid grid[][GRID_SIZE], INT previous[][GRID_SIZE]);

void dijkstraNeighbourCheck(HWND hWnd, HDC hdc, INT x, INT y, Grid grid[][GRID_SIZE], INT distances[][GRID_SIZE], INT previous[][GRID_SIZE],
	BOOL visited[][GRID_SIZE], INT& speed);

void resetGrid(HWND hWnd, Grid grid[][GRID_SIZE], xyValues& start, xyValues& end);
void clearGrid(HWND hWnd, Grid grid[][GRID_SIZE]);