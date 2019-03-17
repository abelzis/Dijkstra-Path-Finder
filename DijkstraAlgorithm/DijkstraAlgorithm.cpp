// DijkstraAlgorithm.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DijkstraAlgorithm.h"

#define MAX_LOADSTRING 100
#define ID_BTNERASER 0
#define ID_BTNSTARTPOS 1
#define ID_BTNENDPOS 2
#define ID_BTNOBSTACLE 3

// Global Variables:
BOOL boolStartPosCursor = FALSE;
BOOL boolEndPosCursor = FALSE;
BOOL boolObstacleCursor = FALSE;
BOOL boolObstacleDraw = FALSE;
BOOL boolEraserCursor = FALSE;
BOOL boolEraserDraw = FALSE;
HCURSOR hCursorStyle = LoadCursor(nullptr, IDC_ARROW);	//handle to cursor type when Start pos button is pressed
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
POINT cursor;	//cursor location
INT count = 0;
INT CONST GRID_SIZE = 10;	//const grid size in x and y axis
Grid grid[GRID_SIZE][GRID_SIZE];
xyValues start{ 0,0 }, end{ 0,0 };
HWND hStartPosButtonWnd;
HWND hEndPosButtonWnd;
HWND hObstaclesButtonWnd;
HWND hEraserButtonWnd;


double CONST menu_y = 40;	//grid menu
double size_x;
double size_y;



// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DIJKSTRAALGORITHM, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIJKSTRAALGORITHM));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIJKSTRAALGORITHM));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DIJKSTRAALGORITHM);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//function paints the grid in the window
void paintingGridLines(HDC& hdc, const double size_x, const double size_y)
{
	//fill rectangles

	//fill start position
	if (grid[start.x][start.y].mode == 1)
	{
		HBRUSH brush = CreateSolidBrush(RGB(0, 233, 0));
		RECT rect = grid[start.x][start.y].tile;
		rect.bottom += 1;
		rect.right += 1;
		FillRect(hdc, &rect, brush);
		DeleteObject(brush);
	}
	
	//fill end position
	if (grid[end.x][end.y].mode == 2)
	{
		HBRUSH brush = CreateSolidBrush(RGB(233, 0, 0));
		RECT rect = grid[end.x][end.y].tile;
		rect.bottom += 1;
		rect.right += 1;
		FillRect(hdc, &rect, brush);
		DeleteObject(brush);
	}

	for (int i = 0; i < GRID_SIZE; i++)
	{
		for (int j = 0; j < GRID_SIZE; j++)
		{
			if (grid[i][j].mode == 3)
			{
				HBRUSH brush = CreateSolidBrush(RGB(99, 44, 0));
				RECT rect = grid[i][j].tile;
				rect.bottom += 1;
				rect.right += 1;
				FillRect(hdc, &rect, brush);
				DeleteObject(brush);
			}
		}
	}

	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));	//black brush
	//draw lines
	for (int i = 0; i < GRID_SIZE+1; i++)
	{



		RECT ver_line, hor_line;	//initializing lines

		//draw vertical strips
		ver_line.top = menu_y + size_y;
		ver_line.bottom = menu_y + (GRID_SIZE+1)*size_y;
		ver_line.left = (i+1)*size_x;
		ver_line.right = ver_line.left + 1;
		FillRect(hdc, &ver_line, brush);	//creating a vertical line

		//draw horizontal strips
		hor_line.left = size_x;
		hor_line.right = (GRID_SIZE+1)*size_x;
		hor_line.top = menu_y + (i+1)*size_y;
		hor_line.bottom = hor_line.top + 1;
		FillRect(hdc, &hor_line, brush);	//creating a vertical line
	}

	DeleteObject(brush);	//release memory
}


//function sets up variables in order to build the grid
void createGrid(HWND& hWnd, HDC hdc, int width, int height)
{
	//get constant sizes of a single tile

	size_x = 1.0*(width) / (1.0*(GRID_SIZE+2)) - 1.625/(sqrt(GRID_SIZE));
	size_y = 1.0*(height - menu_y) / (1.0*(GRID_SIZE+2));


	//give each tile a position in screen
	for (int y = 0; y < GRID_SIZE; y++)
	{
		for (int x = 0; x < GRID_SIZE; x++)
		{
			grid[x][y].tile.left = (x+1)*size_x;
			grid[x][y].tile.right = grid[x][y].tile.left + size_x;

			grid[x][y].tile.top = menu_y + (y + 1)*size_y;
			grid[x][y].tile.bottom = grid[x][y].tile.top + size_y;
		}
	}

	paintingGridLines(hdc, size_x, size_y);	//calling line painting function


	//move buttons
	MoveWindow(hStartPosButtonWnd, menu_y, menu_y / 6, menu_y * 2, menu_y * 2 / 3 + size_y * 2 / 3, NULL);
	MoveWindow(hEndPosButtonWnd, 7 * menu_y / 2, menu_y / 6, menu_y * 2, menu_y * 2 / 3 + size_y * 2 / 3, NULL);
	MoveWindow(hObstaclesButtonWnd, 6 * menu_y, menu_y / 6, menu_y * 4, menu_y * 2 / 3 + size_y * 2 / 3, NULL);
	MoveWindow(hEraserButtonWnd, 11 * menu_y, menu_y / 6, menu_y * 2, menu_y * 2 / 3 + size_y * 2 / 3, NULL);
}


//function sets the start/end positions on grid
void setTilePos(HWND hWnd, char const mode, xyValues& val)
{
	//get cursor position
	GetCursorPos(&cursor);
	ScreenToClient(hWnd, &cursor);

	//get screen dimensions
	RECT screen;
	GetWindowRect(hWnd, &screen);

	int width = screen.right - screen.left;
	int height = screen.bottom - screen.top - 60;	//-60 is to deduct from the toolbar

	size_x = 1.0*(width) / (1.0*(GRID_SIZE + 2)) - 1.625 / (sqrt(GRID_SIZE));
	size_y = 1.0*(height - menu_y) / (1.0*(GRID_SIZE + 2));

	//if cursor's position is on the grid, get coordinates, then color the tile
	if (cursor.x >= size_x && cursor.x < width - size_x &&
		cursor.y >= menu_y + size_y && cursor.y < height - size_y)
	{
		//*****use this formula to calculate tile's dimensions:*****
		int x = cursor.x / size_x - 1;
		int y = (cursor.y - menu_y) / size_y - 1;

		//test if x and y coordinates are inbetween 0 inclusive and GRID_SIZE not inclusive
		if (x >= 0 && x < GRID_SIZE &&
			y >= 0 && y < GRID_SIZE)
		{
			//save starting position indexes and update window
			if (grid[val.x][val.y].mode == mode)
				grid[val.x][val.y].mode = 0;
			grid[x][y].mode = mode;
			val.x = x;
			val.y = y;

			//if ()
			InvalidateRect(hWnd, NULL, TRUE);	//call WM_PAINT
		}
	}

	//set cursor back to normal
	hCursorStyle = LoadCursor(nullptr, IDC_ARROW);
	boolStartPosCursor = false;	//reset to false
	boolEndPosCursor = false;

	DeleteObject(&screen);
}


//function sets the states of tiles when the user moves the mouse in specific mode on the grid
void setTileStateByMouseDrawing(HWND hWnd, char const mode)
{
	//get cursor position
	GetCursorPos(&cursor);
	ScreenToClient(hWnd, &cursor);

	//get screen dimensions
	RECT screen;
	GetWindowRect(hWnd, &screen);

	int width = screen.right - screen.left;
	int height = screen.bottom - screen.top - 60;	//-60 is to deduct from the toolbar

	size_x = 1.0*(width) / (1.0*(GRID_SIZE + 2)) - 1.625 / (sqrt(GRID_SIZE));
	size_y = 1.0*(height - menu_y) / (1.0*(GRID_SIZE + 2));

	//if cursor's position is on the grid, get coordinates, then color the tile
	if (cursor.x >= size_x && cursor.x < width - size_x &&
		cursor.y >= menu_y + size_y && cursor.y < height - size_y)
	{
		//*****use this formula to calculate tile's dimensions:*****
		int x = cursor.x / size_x - 1;
		int y = (cursor.y - menu_y) / size_y - 1;

		//test if x and y coordinates are inbetween 0 inclusive and GRID_SIZE not inclusive
		if (x >= 0 && x < GRID_SIZE &&
			y >= 0 && y < GRID_SIZE)
		{
			//save starting position indexes and update window
			bool startMode;
			if (grid[x][y].mode != mode)
			{
				grid[x][y].mode = mode;

				//if ()
				InvalidateRect(hWnd, NULL, TRUE);	//call WM_PAINT
			}

		}
	}

	//set cursor back to normal
	//hCursorStyle = LoadCursor(nullptr, IDC_ARROW);
	boolStartPosCursor = false;	//reset to false
	boolEndPosCursor = false;

	DeleteObject(&screen);
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
	{

		//create button for starting position
		hStartPosButtonWnd = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"Start pos",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			5,         // x position 
			30,         // y position 
			20,        // Button width
			20,        // Button height
			hWnd,     // Parent window
			(HMENU)ID_BTNSTARTPOS,       //menu.
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.


				//create button for starting position
		hEndPosButtonWnd = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"End pos",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			45,         // x position 
			30,         // y position 
			20,        // Button width
			20,        // Button height
			hWnd,     // Parent window
			(HMENU)ID_BTNENDPOS,       //menu.
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.


		hObstaclesButtonWnd = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"Obstacle Brush",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			45,         // x position 
			30,         // y position 
			20,        // Button width
			20,        // Button height
			hWnd,     // Parent window
			(HMENU)ID_BTNOBSTACLE,       //menu.
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.


		hEraserButtonWnd = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"Eraser",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			45,         // x position 
			30,         // y position 
			20,        // Button width
			20,        // Button height
			hWnd,     // Parent window
			(HMENU)ID_BTNERASER,       //menu.
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.


	}
	break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
			//when Start pos button is pressed, user can select a starting tile.
		case ID_BTNSTARTPOS:
		{
			boolEndPosCursor = FALSE;
			boolObstacleCursor = FALSE;
			boolEraserCursor = FALSE;

			hCursorStyle = LoadCursor(nullptr, IDC_CROSS);	//load cursor model to cursor handle
			boolStartPosCursor = TRUE;
		}
		break;
			//when End pos button is pressed, user can select a starting tile.
		case ID_BTNENDPOS:
		{
			boolStartPosCursor = FALSE;
			boolObstacleCursor = FALSE;
			boolEraserCursor = FALSE;

			hCursorStyle = LoadCursor(nullptr, IDC_CROSS);	//load cursor model to cursor handle
			boolEndPosCursor = TRUE;
		}
		break;
			//when Obstacle button is pressed, user can draw obstacles on grid
		case ID_BTNOBSTACLE:
		{
			boolStartPosCursor = FALSE;
			boolEndPosCursor = FALSE;
			boolEraserCursor = FALSE;

			hCursorStyle = LoadCursor(nullptr, IDC_CROSS);
			boolObstacleCursor = TRUE;
		}
		break;
			//when Eraser button is pressed, user can reset tiles on grid
		case ID_BTNERASER:
		{
			boolStartPosCursor = FALSE;
			boolEndPosCursor = FALSE;
			boolObstacleCursor = FALSE;

			hCursorStyle = LoadCursor(nullptr, IDC_CROSS);
			boolEraserCursor = TRUE;
		}
		break;
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
	case WM_LBUTTONDOWN:
	{
		//when the Start pos button was pressed and the mouse clicks
		if (boolStartPosCursor)
		{
			setTilePos(hWnd, 1, start);
		}
		if (boolEndPosCursor)
		{
			setTilePos(hWnd, 2, end);
		}
		if (boolObstacleCursor)
		{
			boolObstacleDraw = TRUE;
			setTileStateByMouseDrawing(hWnd, 3);
		}
		if (boolEraserCursor)
		{
			boolEraserDraw = TRUE;
			setTileStateByMouseDrawing(hWnd, 0);
		}
	}
	break;
	case WM_LBUTTONUP:
	{

		boolObstacleDraw = FALSE;
		boolEraserDraw = FALSE;
	}
	break;
	case WM_RBUTTONDOWN:
	{
		boolStartPosCursor = FALSE;
		boolEndPosCursor = FALSE;
		boolObstacleCursor = FALSE;
		boolObstacleDraw = FALSE;
		boolEraserCursor = FALSE;
		boolEraserDraw = FALSE;

		hCursorStyle = LoadCursor(nullptr, IDC_ARROW);
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (boolObstacleDraw)
		{
			setTileStateByMouseDrawing(hWnd, 3);
		}
		if (boolEraserDraw)
		{
			setTileStateByMouseDrawing(hWnd, 0);
		}
	}
	break;
		
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = 800;
		lpMMI->ptMinTrackSize.y = 600;
	}
	break;
	//change cursor if the Start pos button is pushed
	case WM_SETCURSOR:
	{
		SetCursor(hCursorStyle);	//change cursor
	}
	break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

		//get screen dimensions
		RECT screen;
		GetWindowRect(hWnd, &screen);

		int win_width = screen.right - screen.left;
		int win_height = screen.bottom - screen.top - 60;	//-60 is to deduct from the toolbar

		createGrid(hWnd, hdc, win_width, win_height);	//draw grid
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
