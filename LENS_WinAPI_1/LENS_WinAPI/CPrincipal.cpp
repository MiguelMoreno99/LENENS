#define WIN32_LEAN_AND_MEAN //No agrega librerías que no se vayan a utilizar
#include <Windows.h>
#include <stdio.h>
#include <dinput.h>
#include <gdiplus.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdlib.h>
#include <time.h>
#include "CPlusPlus.h"
using namespace Gdiplus;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void MainRender(HWND hWnd);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, int nCmdShow){
	Puntuaciones1.CargarPuntuacionTxt();
	WNDCLASSEX wc;									// Windows Class Structure
	HWND hWnd;
	MSG msg;

	TCHAR szAppName[] = TEXT("MyWinAPIApp");		
	TCHAR szAppTitle[] = TEXT("LENS App");

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window

	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance	
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= (HBRUSH) (COLOR_WINDOW + 1);			// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu	
	wc.lpszClassName	= szAppName;							// Set The Class Name
	wc.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);
	
	if (!RegisterClassEx(&wc)) {									// Attempt To Register The Window Class
	
		MessageBox(NULL,
		L"Fallo al registrar clase (Failed To Register The Window Class).",
		L"ERROR",
		MB_OK|MB_ICONEXCLAMATION);
		return 0;
	}

	hWnd = CreateWindowEx(	
	WS_EX_CLIENTEDGE | WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,	// Extended Style For The Window
	szAppName,							// Class Name
	szAppTitle,							// Window Title
	WS_OVERLAPPEDWINDOW |				// Defined Window Style
	WS_CLIPSIBLINGS |					// Required Window Style
	WS_CLIPCHILDREN,					// Required Window Style
	0, 0,								// Window Position
	ANCHO_VENTANA,						// Calculate Window Width
	ALTO_VENTANA,						// Calculate Window Height
	NULL,								// No Parent Window
	NULL,								// No Menu
	hInstance,							// Instance
	NULL);								// Pass this class To WM_CREATE								

	if(hWnd == NULL) {
		MessageBox(NULL, 
		L"Error al crear ventana (Window Creation Error).", 
		L"ERROR", 
		MB_OK|MB_ICONEXCLAMATION);
		return 0;
	}
		
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	Init();
	ShowWindow(hWnd, nCmdShow);
	SetFocus(hWnd);
	SetTimer(hWnd, TICK, TICK, NULL);
	ZeroMemory(&msg, sizeof(MSG));

	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return(int)msg.wParam;
}

//Funcion tipo Callback para el manejo de los eventos de la ventana. 
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){

	switch(uMsg) {			// Check For Windows Messages
		case WM_TIMER:{
			if(wParam == TICK){
				MainRender(hWnd);
			}
		}
		break;
		case WM_PAINT:{
			HDC hdc; 
			PAINTSTRUCT ps;
			hdc = BeginPaint(hWnd, &ps);
			BITMAP bm;
			HBITMAP h_CMC = CreateBitmap(ANCHO_VENTANA, ALTO_VENTANA, 1, 32, ptrBuffer);
			HDC hdcMem = CreateCompatibleDC(hdc);
			HBITMAP hbmOld = (HBITMAP) SelectObject(hdcMem, h_CMC);
			GetObject(h_CMC, sizeof(bm), &bm);
			BitBlt(hdc, 0, 0, ANCHO_VENTANA, ALTO_VENTANA, hdcMem, 0, 0, SRCCOPY);
			DeleteObject(h_CMC);
			SelectObject(hdcMem, hbmOld);
			DeleteDC(hdcMem);
			DeleteObject(hbmOld);
		}
		break;		
		case WM_KEYDOWN:{
			KEYS[ wParam ] = true;
		}
		break;
		case WM_KEYUP:{
			KEYS[ wParam ] = false;
		}
		break;
		case WM_CLOSE: {
			Puntuaciones1.GuardarPuntuacionTxt();
			DestroyWindow(hWnd);
		}
		break;
		case WM_DESTROY:{		//Send A Quit Message
			KillTimer(hWnd, TICK);
			PostQuitMessage(0);
		}
		break;
		default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}
	
 //Funcion principal. Encargada de hacer el redibujado en pantalla cada intervalo (o "tick") del timer que se haya creado. @param hWnd. Manejador de la ventana.
void MainRender(HWND hWnd){
	srand(time(NULL));
	KeysEvents(hWnd);
	if (iniciojuego) {
		RamdomRoadItem = rand() % 7 + 1;
		RamdomRoadSide = rand() % 3 + 1;
		DibujaMenuPrincipal(ptrBuffer, (int*)ptrMenuPrincipal, dmnMenuPrincipal, posMenuPrincipal);
	}
	else if (!iniciojuego) {
		DibujaFondo(ptrBuffer, (int*)ptrBack, dmnBack /*increfondo*/);   //recibe lo de incremento
		DibujaRoad(ptrBuffer, (int*)ptrRoad, dmnRoad, posRoad);
		if (TiempoDeJuego==24){
			DibujaFinish(ptrBuffer, (int*)ptrFinish, dmnFinish, posFinish);
			posFinish.X++;
		}
		DibujaTrees(ptrBuffer, (int*)ptrTrees, dmnTrees, posTrees, increTrees);
		switch (RamdomRoadItem){
		case 1: {
			DibujaObstacleCarLeft(ptrBuffer, (int*)ptrObstacleCarLeft, dmnObstacleCarLeft, posObstacleCarLeft, RamdomRoadSide);
		}break;
		case 2: {
			DibujaObstacleCarRight(ptrBuffer, (int*)ptrObstacleCarRight, dmnObstacleCarRight, posObstacleCarRight, RamdomRoadSide);
		}break;
		case 3: {
			DibujaObstacleCarStatic(ptrBuffer, (int*)ptrObstacleCarStatic, dmnObstacleCarStatic, posObstacleCarStatic, RamdomRoadSide);
		}break;
		case 4: {
			DibujaObstacleRock(ptrBuffer, (int*)ptrObstacleRock, dmnObstacleRock, posObstacleRock, RamdomRoadSide);
		}break;
		case 5: {
			DibujaRewardCoin(ptrBuffer, (int*)ptrRewardCoin, dmnRewardCoin, posRewardCoin, RamdomRoadSide);
		}break;
		case 6: {
			DibujaRewardCoin(ptrBuffer, (int*)ptrRewardCoin, dmnRewardCoin, posRewardCoin, RamdomRoadSide);
		}break;
		case 7: {
			DibujaRewardCoin(ptrBuffer, (int*)ptrRewardCoin, dmnRewardCoin, posRewardCoin, RamdomRoadSide);
		}break;
		default:
			break;
		}
		DibujaPlayerCar(ptrBuffer, (int*)ptrPlayerCar, dmnPlayerCar, posPlayerCar, RoadPlayerCar);
		DibujaPoints(ptrBuffer, (int*)ptrPoints, dmnPoints, posPoints);
		DibujaPoints2(ptrBuffer, (int*)ptrPoints2, dmnPoints2, posPoints2);
	}
	//Funciones que deberan estar el final de la funcion de Render.
	InvalidateRect(hWnd, NULL, FALSE);
	UpdateWindow(hWnd);
}