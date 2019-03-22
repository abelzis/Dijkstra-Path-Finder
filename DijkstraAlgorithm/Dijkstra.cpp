#include "Dijkstra.h"

void dijkstraAlgorithm(HWND hWnd, HDC hdc, xyValues end, Grid grid[][GRID_SIZE], INT x, INT y, INT& speed)
{
	INT distances[GRID_SIZE][GRID_SIZE], previous[GRID_SIZE][GRID_SIZE];
	BOOL visited[GRID_SIZE][GRID_SIZE];

	//initialize default values
	for (int i = 0; i < GRID_SIZE; i++)
	{
		for (int j = 0; j < GRID_SIZE; j++)
		{
			distances[i][j] = INT_MAX;
			previous[i][j] = -1;
			visited[i][j] = FALSE;
		}
	}
	distances[x][y] = 0;

	//for each tile
	dijkstraNeighbourCheck(hWnd, hdc, x, y, grid, distances, previous, visited, speed);

	InvalidateRect(hWnd, NULL, TRUE);

	dijkstraShortestPath(end.x, end.y, grid, previous);

	InvalidateRect(hWnd, NULL, TRUE);
}



//paints the shortest path found
void dijkstraShortestPath(INT x, INT y, Grid grid[][GRID_SIZE], INT previous[][GRID_SIZE])
{
	INT new_x, new_y;
	new_y = previous[x][y] % GRID_SIZE;
	new_x = previous[x][y] / GRID_SIZE;

	grid[new_x][new_y].r = 8;
	grid[new_x][new_y].g = 208;
	grid[new_x][new_y].b = 219;

	if (new_x >= 0 && new_x < GRID_SIZE &&
		new_y >= 0 && new_y < GRID_SIZE)
		dijkstraShortestPath(new_x, new_y, grid, previous);	//recursive
}



void dijkstraNeighbourCheck(HWND hWnd, HDC hdc, INT x, INT y, Grid grid[][GRID_SIZE], INT distances[][GRID_SIZE], INT previous[][GRID_SIZE],
	BOOL visited[][GRID_SIZE], INT& speed)
{
	if (x < 0 || x >= GRID_SIZE ||
		y < 0 || x >= GRID_SIZE)
		return;
	if (visited[x][y] == TRUE)
		return;
	INT next_x = 0, next_y = 0;


	for (int j = 0; j < 4; j++)
	{
		INT shortest_x = 0, shortest_y = 0, shortest_distance = INT_MAX;

		for (int i = 0; i < 4; i++)	//iterate through all the neighbors
		{
			if (i == 0)
			{
				next_x = 1;
				next_y = 0;
			}
			if (i == 1)
			{
				next_x = 0;
				next_y = 1;
			}
			if (i == 2)
			{
				next_x = -1;
				next_y = 0;
			}
			if (i == 3)
			{
				next_x = 0;
				next_y = -1;
			}

			//check if neighboring tile is on the grid
			if (x + next_x >= 0 && x + next_x < GRID_SIZE &&
				y + next_y >= 0 && y + next_y < GRID_SIZE)
			{
				//color tiles
				grid[x][y].path_mode = 1;
				grid[x][y].r = 20;
				grid[x][y].g = 10;
				grid[x][y].b = 180;
				if (grid[x + next_x][y + next_y].mode != 3)
				{
					grid[x + next_x][y + next_y].path_mode = 1;
					grid[x + next_x][y + next_y].r = 255;
					grid[x + next_x][y + next_y].g = 241;
					grid[x + next_x][y + next_y].b = 188;
				}



				INT previous_y = previous[x][y] % GRID_SIZE;
				INT previous_x = previous[x][y] / GRID_SIZE;

				if (previous_x >= 0 && previous_x < GRID_SIZE &&
					previous_y >= 0 && previous_y < GRID_SIZE)
				{
					grid[previous_x][previous_y].r = 214;
					grid[previous_x][previous_y].g = 198;
					grid[previous_x][previous_y].b = 134;

				}


				Sleep(500 - speed * 5);

				//***DRAW ON TOP***

				//get screen dimensions
				RECT screen;
				GetWindowRect(hWnd, &screen);

				int win_width = screen.right - screen.left;
				int win_height = screen.bottom - screen.top - 60;	//-60 is to deduct from the toolbar

				createGrid(hWnd, hdc, win_width, win_height);	//draw grid

				//*******************


				//check if the neighboring tile is eligible
				if (grid[x + next_x][y + next_y].mode != 3)
				{
					INT distance = distances[x][y] + grid[x + next_x][y + next_y].value;	//distance formula between next and current tile
					if (distance < distances[x + next_x][y + next_y])	//if found shorter path
					{
						distances[x + next_x][y + next_y] = distance;
						previous[x + next_x][y + next_y] = x * GRID_SIZE + y;	//e.g. x = 4, y = 2 -> 42
					}
					if (distance < shortest_distance && !visited[x + next_x][y + next_y])
					{
						shortest_x = next_x;
						shortest_y = next_y;
						shortest_distance = distance;
					}
				}
			}
		}
		visited[x][y] = TRUE;

		if (shortest_x == 0 && shortest_y == 0)
			return;
		dijkstraNeighbourCheck(hWnd, hdc, x + shortest_x, y + shortest_y, grid, distances, previous, visited, speed);

		//dijkstraNeighbourCheck(hWnd, hdc, x, y, grid, distances, previous, visited);
	}
}



//function resets grid completely
void resetGrid(HWND hWnd, Grid grid[][GRID_SIZE], xyValues& start, xyValues& end)
{
	for (int i = 0; i < GRID_SIZE; i++)
	{
		for (int j = 0; j < GRID_SIZE; j++)
		{
			if (grid[i][j].mode != 0)
			{
				grid[i][j].mode = 0;
			}
			if (grid[i][j].path_mode != 0)
			{
				grid[i][j].path_mode = 0;
			}
			grid[i][j].r = 255;
			grid[i][j].g = 255;
			grid[i][j].b = 255;
		}
	}

	//reset start/end positions
	start.x = 0;
	start.y = 0;
	end.x = 0;
	end.y = 0;

	InvalidateRect(hWnd, NULL, TRUE);
}



//function clears the grid (obstacles, start/end stays the same)
void clearGrid(HWND hWnd, Grid grid[][GRID_SIZE])
{
	for (int i = 0; i < GRID_SIZE; i++)
	{
		for (int j = 0; j < GRID_SIZE; j++)
		{
			if (grid[i][j].path_mode != 0)
			{
				grid[i][j].path_mode = 0;
				grid[i][j].r = 255;
				grid[i][j].g = 255;
				grid[i][j].b = 255;
			}
		}
	}
	InvalidateRect(hWnd, NULL, TRUE);
}


