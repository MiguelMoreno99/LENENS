#define WIN32_LEAN_AND_MEAN //No agrega librer�as que no se vayan a utilizar

#include <Windows.h>
#include <stdio.h>
#include <dinput.h>
#include <gdiplus.h>
#include <gl/gl.h>
#include <gl/glu.h>

using namespace Gdiplus;

//Variables constantes
 //Variable constante para calcular el ancho de la ventana 
const int ANCHO_VENTANA = 600;

 //Variable constante para calcular el alto de la ventana 
const int ALTO_VENTANA = 600;

 //Variable constante que define la cantidad de bytes por pixel, usada en las operaciones de desplegar sprites/imagenes en pantalla 
const int BPP = 4;

/* Variable constante que define el intervalo del contador o timer en milisegundos, 
	con cada TICK del contador se ejecuta el codigo dentro del case WM_TIMER en la funcion WndProc */
const int TICK = 100;

 //Variables constantes de los colores primarios de un pixel de 32 bits 
const unsigned int BLUE = 0xFF0000FF;
const unsigned int GREEN = 0xFF00FF00;
const unsigned int RED = 0xFFFF0000;

 //Estructura con las coordenadas de los sprites en pantalla en un plano 2D 
struct POSITION {
	int X;
	int Y;
};

 //Estructura con las dimensiones de los sprites a cargar y desplegar en pantalla 
struct DIMENSION {
	int ANCHO;
	int ALTO;
};

/* Estructura con la enumeracion de algunas teclas.
	Se tiene un objeto o variable del tipo de esta estructura, llamado 'input' 
	que sera para acceder a cada uno de las elementos de la enumeracion; ejemplo:
	input.A para la tecla 'A'.*/
 //declaracion del objeto de la estructura Input 
struct Input
{
	enum Keys
	{
		Backspace = 0x08, Tab,
		Clear = 0x0C, Enter,
		Shift = 0x10, Control, Alt,
		Escape = 0x1B,
		Space = 0x20, PageUp, PageDown, End, Home, Left, Up, Right, Down,
		Zero = 0x30, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
		A = 0x41, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		NumPad0 = 0x60, NumPad1, NumPad2, NumPad3, NumPad4, NumPad5, NumPad6, NumPad7, NumPad8, NumPad9,
		F1 = 0x70, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
	};
}input;

//Variables Globales
int *ptrBuffer;
unsigned char* ptrBack;
unsigned char* ptrPlayerCar;
unsigned char* ptrObstacleCarLeft;
unsigned char* ptrObstacleCarRight;
unsigned char* ptrObstacleCarStatic;
unsigned char* ptrObstacleRock;
unsigned char* ptrRewardCoin;
unsigned char* ptrPoints;
unsigned char* ptrFinish;
unsigned char* ptrTrees;
unsigned char* ptrRoad;
unsigned char* ptrMenuPrincipal;
//unsigned char * ptrpelota;
DIMENSION dmnBack, dmnPlayerCar, dmnObstacleCarLeft, dmnObstacleCarRight,
dmnObstacleCarStatic, dmnObstacleRock, dmnRewardCoin, dmnPoints, dmnFinish, dmnTrees, dmnRoad, dmnMenuPrincipal/*dmnPelota*/;
POSITION posPlayerCar;
POSITION posObstacleCarLeft;
POSITION posObstacleCarRight;
POSITION posObstacleCarStatic;
POSITION posObstacleRock;
POSITION posRewardCoin;
POSITION posPoints;
POSITION posFinish;
POSITION posTrees;
POSITION posRoad;
POSITION posMenuPrincipal;
//POSITION posPel;
int indiPlayerCar = 0;
int indiObstacleCarLeft = 0;
int indiObstacleCarRight = 0;
int indiObstacleCarStatic = 0;
int indiObstacleRock = 0;
int indiRewardCoin = 0;
int indiPoints = 0;
int indiMenuPrincipal = 0;
int indiRoad = 0;
//int indiPelota=0;
int initfondo;
bool KEYS[256];
int increTrees = 0;    //que incremente el valor del pixel 
int contadorsh = 10;
int coloresdif = 0;
bool mirror;
//bool loboD=TRUE;
//bool lobomirror = FALSE;
int start;
int scale = 1;

//Declaracion de funciones
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void MainRender(HWND hWnd);
void Init();
void KeysEvents();
unsigned char * CargaImagen(WCHAR rutaImagen[], DIMENSION * dmn);
POSITION setPosition(int x, int y);
void DibujaFondo(int *buffer, int *imagen, DIMENSION dmn /*int incremento*/);
void DibujaPlayerCar(int *buffer, int *playercar, DIMENSION dmn1, POSITION pos1);
void DibujaRoad(int* buffer, int* road, DIMENSION dmn2, POSITION pos2);
void DibujaObstacleCarLeft(int* buffer, int* obstaclecarleft, DIMENSION dmn3, POSITION pos3);
void DibujaObstacleCarRight(int* buffer, int* obstaclecarright, DIMENSION dmn4, POSITION pos4);
void DibujaObstacleCarStatic(int* buffer, int* obstaclecarstatic, DIMENSION dmn5, POSITION pos5);
void DibujaObstacleRock(int* buffer, int* obstaclerock, DIMENSION dmn6, POSITION pos6);
void DibujaRewardCoin(int* buffer, int* rewardcoin, DIMENSION dmn7, POSITION pos7);
void DibujaPoints(int* buffer, int* points, DIMENSION dmn8, POSITION pos8);
void DibujaFinish(int* buffer, int* finish, DIMENSION dmn9, POSITION pos9);
void DibujaTrees(int* buffer, int* trees, DIMENSION dmn10, POSITION pos10, int incremento);
void DibujaMenuPrincipal(int *buffer, int * menuprincipal, DIMENSION dmn11, POSITION pos11);
//void DibujaPelota(int *buffer, int *pelota, DIMENSION dmn12, POSITION pos12);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
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
	
	if (!RegisterClassEx(&wc))									// Attempt To Register The Window Class
	{
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)									// Check For Windows Messages
	{
		case WM_TIMER:
			if(wParam == TICK)
			{
				MainRender(hWnd);
			}
			break;
		case WM_PAINT:
			{
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
		case WM_KEYDOWN:							
			{
				KEYS[ wParam ] = true;
			}
			break;
		case WM_KEYUP:
			{
				KEYS[ wParam ] = false;
			}
			break;
		case WM_CLOSE: 
			{
				DestroyWindow(hWnd);
			}
			break;
		case WM_DESTROY: //Send A Quit Message
			{
				KillTimer(hWnd, TICK);
				PostQuitMessage(0);
			}
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

 //Funcion usada para la inicializacion de variables y reserva de espacio en memoria.
void Init()
{
	posPlayerCar.X = 200;
	posPlayerCar.Y = 300;
	scale++;
	scale = scale >= 3 ? 1 : scale;
	//initfondo = 0;

	posObstacleCarLeft.X = 100;
	posObstacleCarLeft.Y = 200;

	posRoad.X = 150;
	posRoad.Y = 0;

	posFinish.X = 172;
	posFinish.Y = 0;

	posMenuPrincipal.X = 120;
	posMenuPrincipal.Y = 0;
	//posPel.X = 20;
	//posPel.Y = 50;

	for (int i = 0; i < 256; i++)
	{
		KEYS[i] = false;
	}

	//Inicializar el puntero tipo int 'ptrBuffer' que contiene la direccion inicial  del area de memoria reservada para el despliegue de sprites/imagenes.
	ptrBuffer = new int[ANCHO_VENTANA * ALTO_VENTANA];

	//Inicializar el puntero tipo unsigned char 'ptrBack' que contiene la direccion inicial en memoria del arreglo de pixeles de la imagen especificada en el primer parametro
	//y en la variable dmnBack de tipo DIMENSION* estan los valores de ANCHO y ALTO de la imagen.
	ptrBack = CargaImagen(TEXT("Fondo.png"), &dmnBack); //puntero a la imagen
	ptrPlayerCar = CargaImagen(TEXT("PlayerCar.png"), &dmnPlayerCar);   //puntero a mi personaje sprite
	ptrObstacleCarLeft = CargaImagen(TEXT("ObstacleCarLeft.png"), &dmnObstacleCarLeft);
	ptrObstacleCarRight = CargaImagen(TEXT("ObstacleCarRight.png"), &dmnObstacleCarRight);
	ptrObstacleCarStatic = CargaImagen(TEXT("ObstacleCarStatic.png"), &dmnObstacleCarStatic);
	ptrObstacleRock = CargaImagen(TEXT("ObstacleRock.png"), &dmnObstacleRock);
	ptrRewardCoin = CargaImagen(TEXT("RewardCoin.png"), &dmnRewardCoin);
	ptrPoints = CargaImagen(TEXT("Points.png"), &dmnPoints);
	ptrFinish = CargaImagen(TEXT("Finish.png"), &dmnFinish);
	ptrTrees= CargaImagen(TEXT("Trees.png"), &dmnTrees);
	ptrMenuPrincipal = CargaImagen(TEXT("MenuPrincipal.png"), &dmnMenuPrincipal);
	ptrRoad = CargaImagen(TEXT("Road.png"), &dmnRoad);
	//ptrpelota = CargaImagen(TEXT("pelotita.png"),&dmnPelota);
}

 //Funcion principal. Encargada de hacer el redibujado en pantalla cada intervalo (o "tick") del timer que se haya creado.
	//@param hWnd. Manejador de la ventana.
	//
void MainRender(HWND hWnd) 
{
	KeysEvents();

	DibujaFondo(ptrBuffer, (int*)ptrBack, dmnBack /*increfondo*/);   //recibe lo de incremento
	DibujaRoad(ptrBuffer, (int*)ptrRoad, dmnRoad, posRoad);
	DibujaFinish(ptrBuffer, (int*)ptrFinish, dmnFinish, posFinish);
	DibujaTrees(ptrBuffer, (int*)ptrTrees, dmnTrees, posTrees, increTrees);
	DibujaPlayerCar(ptrBuffer, (int*)ptrPlayerCar, dmnPlayerCar, posPlayerCar);
	DibujaObstacleCarLeft(ptrBuffer, (int*)ptrObstacleCarLeft, dmnObstacleCarLeft, posObstacleCarLeft);
	DibujaObstacleCarRight(ptrBuffer, (int*)ptrObstacleCarRight, dmnObstacleCarRight, posObstacleCarRight);
	DibujaObstacleCarStatic(ptrBuffer, (int*)ptrObstacleCarStatic, dmnObstacleCarStatic, posObstacleCarStatic);
	DibujaObstacleRock(ptrBuffer, (int*)ptrObstacleRock, dmnObstacleRock, posObstacleRock);
	DibujaRewardCoin(ptrBuffer, (int*)ptrRewardCoin, dmnRewardCoin, posRewardCoin);
	DibujaPoints(ptrBuffer, (int*)ptrPoints, dmnPoints, posPoints);
	DibujaMenuPrincipal(ptrBuffer, (int*)ptrMenuPrincipal, dmnMenuPrincipal, posMenuPrincipal);
	//DibujaPelota(ptrBuffer, (int*)ptrpelota, dmnPelota, posPel);

	//Funciones que deberan estar el final de la funcion de Render.
	InvalidateRect(hWnd, NULL, FALSE);
	UpdateWindow(hWnd);
}

 //Funcion que regresa la posicion del sprite en pantalla.
	//@param x. Coordenada X en la ventana.
	//@param y. Coordenada Y en la ventana.
	//
POSITION setPosition(int x, int y) {
	POSITION p;
	p.X = x;
	p.Y = y;
	return p;
}

 //Funcion para manejar eventos del teclado dependiendo de la(s) tecla(s) que se haya(n) presionado.
void KeysEvents() 
{
	if(KEYS[input.W] || KEYS[input.Up]) {
		//ptrBack += 1;
	}
	if (KEYS[input.Q]) {
		//scale++;
		//scale = scale >= 3 ? 1 : scale;
	}


	if(KEYS[input.D] || KEYS[input.Right]) {
		mirror = FALSE;
		indiPlayerCar++;
		indiPlayerCar = indiPlayerCar >= 4 ? 0 : indiPlayerCar;
		if (posPlayerCar.X < 500)   //para que tope en mi ventana de 800 del lado derecho
		{
			posPlayerCar.X += 10;
		}
		else
		{
			if (increTrees < 590)  //para que tope en mi imagen 2048
			{
				increTrees += 10;  //velocidad en que avanza
			}
		}
	}
	
	if(KEYS[input.A] || KEYS[input.Left]) {
		mirror = TRUE;
		indiPlayerCar++;
		indiPlayerCar = indiPlayerCar >= 4 ? 0 : indiPlayerCar;
		if (posPlayerCar.X > 10)  //para que tope en mi ventana de 800 del lado izquierdo
		{
			posPlayerCar.X -= 10;
		}
		else
		{
			if (increTrees > 10)  //para que tope en mi imagen 2048
			{
				increTrees -= 10;  //velocidad que se regresa
			}
		}
	}
	if (KEYS[input.Space]){
		//indiPelota++;
		//if (indiPelota > 3) {
		//	indiPelota = 0;
		//}
	}
	if (KEYS[input.R])
	{
		coloresdif = RED;
	}
	if (KEYS[input.B])
	{
		coloresdif = BLUE;
	}
	if (KEYS[input.G])
	{
		coloresdif = GREEN;
	}
	if (KEYS[input.N])
	{
		coloresdif = 0;
	}

	indiRoad++;
	indiRoad = indiRoad >= 2 ? 0 : indiRoad;

	indiObstacleCarLeft++;
	indiObstacleCarLeft = indiObstacleCarLeft >= 5 ? 0 : indiObstacleCarLeft;

	indiObstacleCarRight++;
	indiObstacleCarRight = indiObstacleCarRight >= 5 ? 0 : indiObstacleCarRight;

	indiObstacleCarStatic++;
	indiObstacleCarStatic = indiObstacleCarStatic >= 4 ? 0 : indiObstacleCarStatic;

	indiObstacleRock++;
	indiObstacleRock = indiObstacleRock >= 3 ? 0 : indiObstacleRock;

	indiRewardCoin++;
	indiRewardCoin = indiRewardCoin >= 4 ? 0 : indiRewardCoin;


	indiPoints++;		
	indiPoints = indiPoints >= 10 ? 0 : indiPoints;


	indiMenuPrincipal++;
	indiMenuPrincipal = indiMenuPrincipal >= 6 ? 0 : indiMenuPrincipal;

	//if (loboD==TRUE) {
	//	lobomirror = FALSE;
	//	posLob.X += 10;
	//	contadorsh++;
	//	if (contadorsh == 50) {
	//		loboD = FALSE;
	//	}
	//}
	//else {
	//	if (loboD == FALSE) {
	//		lobomirror = TRUE;
	//		posLob.X -= 10;
	//		contadorsh--;
	//		if (contadorsh == 5) {
	//			loboD = TRUE;
	//		}
	//	}
	//	
	//}
}

 //Funcion para cargar imagenes y obtener un puntero al area de memoria reservada para la misma.
	//@param rutaImagen.			Nombre o ruta de la imagen a cargar en memoria.
	//@return unsigned char *.	Direccion base de la imagen.
	//
unsigned char * CargaImagen(WCHAR rutaImagen[], DIMENSION * dmn)
{
	unsigned char * ptrImagen;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR  gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	Bitmap *bitmap=new Bitmap(rutaImagen);
	BitmapData *bitmapData=new BitmapData;

	dmn->ANCHO = bitmap->GetWidth();
	dmn->ALTO = bitmap->GetHeight();

	Rect rect(0, 0, dmn->ANCHO, dmn->ALTO);

	//Reservamos espacio en memoria para la imagen
	bitmap->LockBits(&rect, ImageLockModeRead, PixelFormat32bppRGB, bitmapData);

	//"pixels" es el puntero al area de memoria que ocupa la imagen
	unsigned char* pixels = (unsigned char*)bitmapData->Scan0;

	//"tama�o" lo usaremos para reservar los bytes que necesita la imagen. 
	//Para calcular la cantidad de bytes total necesitamos multiplicamos el area de la imagen * 4. 
	//Se multiplica por 4 debido a que cada pixel ocupa 4 bytes de memoria. Noten el 3er parametro de la funcion LockBits, dos lineas de codigo arriba.
	//PixelFormat32bppARGB -> Specifies that the format is 32 bits per pixel; 8 bits each are used for the alpha, red, green, and blue components.
	//Mas info: https://msdn.microsoft.com/en-us/library/system.drawing.imaging.pixelformat(v=vs.110).aspx
	int tama�o;
	tama�o = dmn->ANCHO * dmn->ALTO * 4;
	//hagamos un try de la reserva de memoria
	try
	{
		ptrImagen = new unsigned char [tama�o]; 
	}
	catch(...)
	{
		return NULL;
	}

	//Despu�s de este for, ptrImagen contiene la direccion en memoria de la imagen.
	for(int i=0, j=tama�o; i < j; i++)
	{
		ptrImagen[i]=pixels[i];
	}

	//Es necesario liberar el espacio en memoria, de lo contrario marcaria una excepcion de no hay espacio de memoria suficiente.
	bitmap->UnlockBits(bitmapData);
	delete bitmapData;
	delete bitmap;
	  
	GdiplusShutdown(gdiplusToken);

	return ptrImagen;
}

#pragma region LENS_CODE
void DibujaFondo(int * buffer, int * imagen, DIMENSION dmn/* int incremento*/) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO;
	__asm {

		cld  
		mov esi, imagen   
		mov edi, buffer   

		mov ecx, ALTO_VENTANA 

		//mov eax, incremento   //mi parametro para incrementar los pixeles, lo mando a eax
		//mul BPP               //multiplico por los bytes
		//add esi, eax          //se lo agrego a mi imagen para que se recorra

	PonerA :
		push ecx  //el alto lo pongo al inicio de mi linea
			mov ecx, ANCHO_VENTANA //recorre el ancho

		PonerL :
		mov eax, [esi]     //mi imagen la paso a eax
			mov[edi], eax      //eax lo pongo en mi ventana  (paso mi imagen a la ventana)
			add edi, BPP                     //incremento mi pixel 4 bytes
			add esi, BPP
			loop PonerL
			mov eax, 600                    //ancho de mi imagen
			mul BPP                          //multiplico por 4 bytes
			add esi, eax                     //incremento para que me pase a la linea de abajo
			mov eax, ANCHO_VENTANA
			mul BPP
			sub esi, eax                     //resto para posicionarlo al principio de mi linea
			pop ecx   //para sacar el alto y restarle uno
			loop PonerA

		//; Inicializar registros indices
		//	; con la direccion inicial en memoria de la imagen a desplegar(fuente->imagen->registro indice fuente->ESI)
		//	; y del area de memoria reservada para el despliegue(destino->buffer->registro indice destino->EDI)
		//	; mov esi, imagen
		//	; mov edi, buffer
		//	; Cargar la direccion en memoria de la variable 'dmn' el registro base(EBX). 'dmn' es la estructura con el ANCHO y ALTO.
		//	; lea ebx, dmn
		//;Transferir o copiar 4 bytes en memoria a partir de la direccion en memoria especificada por EBX; 
		//; son 4 bytes porque el operando destino es de 4 bytes en este caso es el registro acumulador(EAX).
		//	; Es decir, EBX es un puntero a 'dmn' donde 'dmn' es de 8 bytes(4Bytes de la variable ANCHO y los siguientes 4Bytes de la variable ALTO),
		//	; entonces EAX = ANCHO
		//	; mov eax, [ebx]
		//	; sumar 4 al registro puntero(EBX) para apuntar al siguiente elemento de la estructura DIMENSION el cual es ALTO y vendria siendo el otro factor de la multiplicacion
		//	; add ebx, 4
		//	; Especificar el otro factor en la instruccion de multiplicacion que son los siguientes 4Bytes a partir de la direccion especificada por EBX(variable ALTO de DIMENSION)
		//	; Se multplican ANCHO(= EAX) * ALTO(= dword ptr[ebx])
		//	; con 'dword ptr [ebx]' estamos indicando que EBX se comporte como puntero tipo DWORD,
		//	; es decir, se esta especificando que se desean los 4Bytes (4Bytes = 1Dword) a partir de la direccion en memoria especificada por el registro EBX
		//	; el cual contiene la direccion inicial de la variable ALTO en la estructura DIMENSION.
		//	; mul dword ptr[ebx]
		//	; el resultado de la multiplicacion anterior es del doble del tamano de los factores, en este caso los factores son de 4Bytes por lo que el resultado es de 8Bytes
		//	; y se encuentra en EDX : EAX donde EDX contiene los 4Bytes de mayor peso del producto de la multiplicacion y EAX contiene los 4Bytes de menor peso.
		//	; Como el producto de la multiplicacion sabemos y esperamos que es un valor con un tamano menor a 4Bytes entonces dicho resultado esta en EAX
		//	; y con la siguiente instruccion se lo copiamos al registro contador(ECX)
		//	; para posteriormente hacer el ciclo / bucle de leer cada pixel de la imagen en memoria y pasarlo al buffer(area de mem.reservada para el despliegue)
		//	; mov ecx, eax
		//;poner codigo para el bucle de leer pixel por pixel y transferirlo al buffer.
	}
}

void DibujaRoad(int* buffer, int* road, DIMENSION dmn, POSITION pos){
	int w = dmn.ANCHO;
	int h = dmn.ALTO;
	int x = posRoad.X;
	int y = posRoad.Y;
	//if (lobomirror == FALSE) {
	__asm {
		cld

		mov esi, road
		mov edi, buffer

		//posicionar mi sprite		
		mov eax, y
		mov ebx, 0
		mul ebx
		add edi, eax

		mov eax, x
		mul BPP
		add edi, eax

		add edi, 0


		mov eax, 249       //porque es la cantidad de pixeles de cada cuadrito
		mul BPP
		mul  indiRoad
		add esi, eax

		xor ecx, ecx
		mov ecx, h    //asigno mi altura a ecx

		repetir :
		push ecx     //guardo en mi pila ecx
			mov ecx, 249

			//PARA QUITAR EL FONDO ROJO
			muestra :
			mov eax, [esi]         //mi imagen la paso a eax
			cmp eax, 0FFFF2BD8h    //comparo lo rojo 
			je color1   //si es igual salta a color1
			/*or eax, coloresdif*/
			mov[edi], eax
			color1 :
		add esi, BPP   //incrementa y pasa el pixel rojo
			add edi, BPP
			loop muestra

			//suma el total para luego restar y posicionarlo en la linea de abajo         
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			mov eax, 249
			mul BPP
			sub edi, eax

			//MUEVE EL PUNTERO A LA SIGUIENTE LINEA DE LA IMAGEN PARA SEGUIR DIBUJANDO
			mov eax, w
			mul BPP

			add esi, eax
			mov eax, 249
			mul BPP
			sub esi, eax

			pop ecx
			loop repetir
	}
	/*}*/
	//else {
	//	__asm {
	//		cld

	//		mov esi, lobo
	//		mov edi, buffer

	//		//posicionar mi sprite		
	//		mov eax, y
	//		mov ebx, 9200
	//		mul ebx
	//		add edi, eax

	//		mov eax, x
	//		mul BPP
	//		add edi, eax

	//		add edi, 680000


	//		mov eax, 48        //porque es la cantidad de pixeles de cada cuadrito
	//		mul BPP
	//		mul  indiRoad
	//		sub esi, eax

	//		xor ecx, ecx
	//		mov ecx, h    //asigno mi altura a ecx

	//		repetir2 :
	//		push ecx     //guardo en mi pila ecx
	//			mov ecx, 48

	//			//PARA QUITAR EL FONDO ROJO
	//			muestra2 :
	//			mov eax, [esi]         //mi imagen la paso a eax
	//			cmp eax, 0FFFF0000h    //comparo lo rojo 
	//			je color2   //si es igual salta a color1
	//						/*or eax, coloresdif*/
	//			mov[edi], eax
	//			color2 :
	//		    sub esi, BPP   //incrementa y pasa el pixel rojo
	//			add edi, BPP
	//			loop muestra2

	//			//suma el total para luego restar y posicionarlo en la linea de abajo         
	//			mov eax, ANCHO_VENTANA
	//			mul BPP

	//			add edi, eax
	//			mov eax, 48
	//			mul BPP
	//			sub edi, eax

	//			//MUEVE EL PUNTERO A LA SIGUIENTE LINEA DE LA IMAGEN PARA SEGUIR DIBUJANDO
	//			mov eax, w
	//			mul BPP

	//			add esi, eax
	//			mov eax, 48
	//			mul BPP
	//			add esi, eax

	//			pop ecx
	//			loop repetir2
	//	}
	//}
}

void DibujaPlayerCar(int *buffer, int *playercar, DIMENSION dmn, POSITION pos){
	int w = dmn.ANCHO;
	int h = dmn.ALTO;
	int x = posPlayerCar.X;
	int y = posPlayerCar.Y;


		if (mirror == FALSE)
		{
			__asm{
				cld
					//cargo el sprite
					mov esi, playercar
					mov edi, buffer

					//posicionar mi sprite		
					mov eax, y
					mov ebx, 3000
					mul ebx
					add edi, eax

					mov eax, x
					mul BPP
					add edi, eax

					add edi, 5000


					mov eax, 35   //porque es la cantidad de pixeles de cada cuadrito
					mul BPP
					mul indiPlayerCar    //para moverlo con la tecla que asigne, si le pico una ves el 0 default se convierte en uno, de multiplica por 59 y se mueve el sprite
					add esi, eax

					xor ecx, ecx
					mov ecx, h    //asigno mi altura a ecx
					ScaladoY :
				push ecx
					mov ecx, scale
					repite :
				push ecx
					mov ecx, 35
					mostrar :
					mov eax, [esi]         //mi imagen la paso a eax
					cmp eax, 0FFFF2BD8h    //comparo lo rojo 
					je colorro   //si es igual salta a colorro
					or eax, coloresdif
					push ecx
					mov ecx, scale
					scalax :
				mov[edi], eax
					add edi, BPP
					loop scalax
					pop ecx
					jmp Salir
			colorro :
					mov eax, BPP
					mul scale
					add edi, eax
			Salir :
					add esi, BPP
				loop mostrar
					mov eax, ANCHO_VENTANA
					mul BPP
					add edi, eax
					MOV EAX, 35
					MUL BPP
					sub esi, eax
					mul scale
					SUB EDI, EAX
					pop ecx
					loop repite
					mov eax, w
					mul BPP
					add esi, eax
			
					pop ecx
					loop ScaladoY
			}

			
		}
		else{
			__asm {
				cld
				//cargo el sprite
				mov esi, playercar
				mov edi, buffer

				//posicionar mi sprite		
				mov eax, y
				mov ebx, 3000
				mul ebx
				add edi, eax

				mov eax, x
				mul BPP
				add edi, eax

				add edi, 5000


				mov eax, 35   //porque es la cantidad de pixeles de cada cuadrito
				mul BPP
				mul indiPlayerCar    //para moverlo con la tecla que asigne, si le pico una ves el 0 default se convierte en uno, de multiplica por 59 y se mueve el sprite
				add esi, eax

				xor ecx, ecx
				mov ecx, h    //asigno mi altura a ecx
				ScaladoY2 :
				push ecx
					mov ecx, scale
					repite2 :
				push ecx
					mov ecx, 35
					mostrar2 :
					mov eax, [esi]
					cmp eax, 0FFFF2BD8h
					je Color2
					or eax, coloresdif
					push ecx
					mov ecx, scale
					scalax2 :
				mov[edi], eax
					add edi, BPP
					loop scalax2
					pop ecx
					jmp Salir2
					Color2 :
				mov eax, BPP
					mul scale
					add edi, eax
					Salir2 :
				sub esi, BPP//
					loop mostrar2
					mov eax, ANCHO_VENTANA
					mul BPP
					add edi, eax
					MOV EAX, 35
					MUL BPP
					add esi, eax
					mul scale
					SUB EDI, EAX
					pop ecx
					loop repite2
					mov eax, w
					mul BPP
					add esi, eax

					pop ecx
					loop ScaladoY2
			}
		}
	
}

void DibujaObstacleCarLeft(int* buffer, int* obstaclecarleft, DIMENSION dmn, POSITION pos) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO;
	int x = posObstacleCarLeft.X;
	int y = posObstacleCarLeft.Y;

	__asm {
		cld

		mov esi, obstaclecarleft
		mov edi, buffer

		//posicionar mi sprite		
		mov eax, y
		mov ebx, 6000
		mul ebx
		add edi, eax

		mov eax, x
		mul BPP
		add edi, eax

		add edi, 6000


		mov eax, 35       //porque es la cantidad de pixeles de cada cuadrito
		mul BPP
		mul  indiObstacleCarLeft
		add esi, eax

		xor ecx, ecx
		mov ecx, h    //asigno mi altura a ecx

		repetir :
		push ecx     //guardo en mi pila ecx
			mov ecx, 35

			//PARA QUITAR EL FONDO ROJO
			muestra :
			mov eax, [esi]         //mi imagen la paso a eax
			cmp eax, 0FFFF2BD8h    //comparo lo rojo 
			je color1   //si es igual salta a color1
			/*or eax, coloresdif*/
			mov[edi], eax
			color1 :
		add esi, BPP   //incrementa y pasa el pixel rojo
			add edi, BPP
			loop muestra

			//suma el total para luego restar y posicionarlo en la linea de abajo         
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			mov eax, 35
			mul BPP
			sub edi, eax

			//MUEVE EL PUNTERO A LA SIGUIENTE LINEA DE LA IMAGEN PARA SEGUIR DIBUJANDO
			mov eax, w
			mul BPP

			add esi, eax
			mov eax, 35
			mul BPP
			sub esi, eax

			pop ecx
			loop repetir
	}
}

void DibujaObstacleCarRight(int* buffer, int* obstaclecarright, DIMENSION dmn, POSITION pos) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO;
	int x = posObstacleCarRight.X;
	int y = posObstacleCarRight.Y;

	__asm {
		cld

		mov esi, obstaclecarright
		mov edi, buffer

		//posicionar mi sprite		
		mov eax, y
		mov ebx, 9000
		mul ebx
		add edi, eax

		mov eax, x
		mul BPP
		add edi, eax

		add edi, 9000


		mov eax, 35       //porque es la cantidad de pixeles de cada cuadrito
		mul BPP
		mul  indiObstacleCarRight
		add esi, eax

		xor ecx, ecx
		mov ecx, h    //asigno mi altura a ecx

		repetir :
		push ecx     //guardo en mi pila ecx
			mov ecx, 35

			//PARA QUITAR EL FONDO ROJO
			muestra :
			mov eax, [esi]         //mi imagen la paso a eax
			cmp eax, 0FFFF2BD8h    //comparo lo rojo 
			je color1   //si es igual salta a color1
			/*or eax, coloresdif*/
			mov[edi], eax
			color1 :
		add esi, BPP   //incrementa y pasa el pixel rojo
			add edi, BPP
			loop muestra

			//suma el total para luego restar y posicionarlo en la linea de abajo         
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			mov eax, 35
			mul BPP
			sub edi, eax

			//MUEVE EL PUNTERO A LA SIGUIENTE LINEA DE LA IMAGEN PARA SEGUIR DIBUJANDO
			mov eax, w
			mul BPP

			add esi, eax
			mov eax, 35
			mul BPP
			sub esi, eax

			pop ecx
			loop repetir
	}
}

void DibujaObstacleCarStatic(int* buffer, int* obstaclecarstatic, DIMENSION dmn, POSITION pos) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO;
	int x = posObstacleCarStatic.X;
	int y = posObstacleCarStatic.Y;

	__asm {
		cld

		mov esi, obstaclecarstatic
		mov edi, buffer

		//posicionar mi sprite		
		mov eax, y
		mov ebx, 7000
		mul ebx
		add edi, eax

		mov eax, x
		mul BPP
		add edi, eax

		add edi, 7000


		mov eax, 35       //porque es la cantidad de pixeles de cada cuadrito
		mul BPP
		mul  indiObstacleCarStatic
		add esi, eax

		xor ecx, ecx
		mov ecx, h    //asigno mi altura a ecx

		repetir :
		push ecx     //guardo en mi pila ecx
			mov ecx, 35

			//PARA QUITAR EL FONDO ROJO
			muestra :
			mov eax, [esi]         //mi imagen la paso a eax
			cmp eax, 0FFFF2BD8h    //comparo lo rojo 
			je color1   //si es igual salta a color1
			/*or eax, coloresdif*/
			mov[edi], eax
			color1 :
		add esi, BPP   //incrementa y pasa el pixel rojo
			add edi, BPP
			loop muestra

			//suma el total para luego restar y posicionarlo en la linea de abajo         
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			mov eax, 35
			mul BPP
			sub edi, eax

			//MUEVE EL PUNTERO A LA SIGUIENTE LINEA DE LA IMAGEN PARA SEGUIR DIBUJANDO
			mov eax, w
			mul BPP

			add esi, eax
			mov eax, 35
			mul BPP
			sub esi, eax

			pop ecx
			loop repetir
	}
}

void DibujaObstacleRock(int* buffer, int* obstaclerock, DIMENSION dmn, POSITION pos) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO;
	int x = posObstacleRock.X;
	int y = posObstacleRock.Y;

	__asm {
		cld

		mov esi, obstaclerock
		mov edi, buffer

		//posicionar mi sprite		
		mov eax, y
		mov ebx, 5000
		mul ebx
		add edi, eax

		mov eax, x
		mul BPP
		add edi, eax

		add edi, 5000


		mov eax, 60       //porque es la cantidad de pixeles de cada cuadrito
		mul BPP
		mul  indiObstacleRock
		add esi, eax

		xor ecx, ecx
		mov ecx, h    //asigno mi altura a ecx

		repetir :
		push ecx     //guardo en mi pila ecx
			mov ecx, 60

			//PARA QUITAR EL FONDO ROJO
			muestra :
			mov eax, [esi]         //mi imagen la paso a eax
			cmp eax, 0FFFF2BD8h    //comparo lo rojo 
			je color1   //si es igual salta a color1
			/*or eax, coloresdif*/
			mov[edi], eax
			color1 :
		add esi, BPP   //incrementa y pasa el pixel rojo
			add edi, BPP
			loop muestra

			//suma el total para luego restar y posicionarlo en la linea de abajo         
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			mov eax, 60
			mul BPP
			sub edi, eax

			//MUEVE EL PUNTERO A LA SIGUIENTE LINEA DE LA IMAGEN PARA SEGUIR DIBUJANDO
			mov eax, w
			mul BPP

			add esi, eax
			mov eax, 60
			mul BPP
			sub esi, eax

			pop ecx
			loop repetir
	}
}

void DibujaRewardCoin(int* buffer, int* rewardcoin, DIMENSION dmn, POSITION pos) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO;
	int x = posRewardCoin.X;
	int y = posRewardCoin.Y;

	__asm {
		cld

		mov esi, rewardcoin
		mov edi, buffer

		//posicionar mi sprite		
		mov eax, y
		mov ebx, 4000
		mul ebx
		add edi, eax

		mov eax, x
		mul BPP
		add edi, eax

		add edi, 4000


		mov eax, 50       //porque es la cantidad de pixeles de cada cuadrito
		mul BPP
		mul  indiRewardCoin
		add esi, eax

		xor ecx, ecx
		mov ecx, h    //asigno mi altura a ecx

		repetir :
		push ecx     //guardo en mi pila ecx
			mov ecx, 50

			//PARA QUITAR EL FONDO ROJO
			muestra :
			mov eax, [esi]         //mi imagen la paso a eax
			cmp eax, 0FFFF2BD8h    //comparo lo rojo 
			je color1   //si es igual salta a color1
			/*or eax, coloresdif*/
			mov[edi], eax
			color1 :
		add esi, BPP   //incrementa y pasa el pixel rojo
			add edi, BPP
			loop muestra

			//suma el total para luego restar y posicionarlo en la linea de abajo         
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			mov eax, 50
			mul BPP
			sub edi, eax

			//MUEVE EL PUNTERO A LA SIGUIENTE LINEA DE LA IMAGEN PARA SEGUIR DIBUJANDO
			mov eax, w
			mul BPP

			add esi, eax
			mov eax, 50
			mul BPP
			sub esi, eax

			pop ecx
			loop repetir
	}
}

void DibujaPoints(int* buffer, int* points, DIMENSION dmn, POSITION pos) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO;
	int x = posPoints.X;
	int y = posPoints.Y;

	__asm {
		cld

		mov esi, points
		mov edi, buffer

		//posicionar mi sprite		
		mov eax, y
		mov ebx, 4500
		mul ebx
		add edi, eax

		mov eax, x
		mul BPP
		add edi, eax

		add edi, 4500


		mov eax, 30       //porque es la cantidad de pixeles de cada cuadrito
		mul BPP
		mul  indiPoints
		add esi, eax

		xor ecx, ecx
		mov ecx, h    //asigno mi altura a ecx

		repetir :
		push ecx     //guardo en mi pila ecx
			mov ecx, 30

			//PARA QUITAR EL FONDO ROJO
			muestra :
			mov eax, [esi]         //mi imagen la paso a eax
			cmp eax, 0FFFF2BD8h    //comparo lo rojo 
			je color1   //si es igual salta a color1
			/*or eax, coloresdif*/
			mov[edi], eax
			color1 :
		add esi, BPP   //incrementa y pasa el pixel rojo
			add edi, BPP
			loop muestra

			//suma el total para luego restar y posicionarlo en la linea de abajo         
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			mov eax, 30
			mul BPP
			sub edi, eax

			//MUEVE EL PUNTERO A LA SIGUIENTE LINEA DE LA IMAGEN PARA SEGUIR DIBUJANDO
			mov eax, w
			mul BPP

			add esi, eax
			mov eax, 30
			mul BPP
			sub esi, eax

			pop ecx
			loop repetir
	}
}

void DibujaFinish(int* buffer, int* finish, DIMENSION dmn, POSITION pos) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO;
	int x = posFinish.X;
	int y = posFinish.Y;

	__asm {
		cld

		mov esi, finish
		mov edi, buffer

		//posicionar mi sprite		
		mov eax, y
		mov ebx, 0
		mul ebx
		add edi, eax

		mov eax, x
		mul BPP
		add edi, eax

		add edi, 0


		mov eax, 205       //porque es la cantidad de pixeles de cada cuadrito
		mul BPP
		//mul  indiFinish
		add esi, eax

		xor ecx, ecx
		mov ecx, h    //asigno mi altura a ecx

		repetir :
		push ecx     //guardo en mi pila ecx
			mov ecx, 205

			//PARA QUITAR EL FONDO ROJO
			muestra :
			mov eax, [esi]         //mi imagen la paso a eax
			cmp eax, 0FFFF2BD8h    //comparo lo rojo 
			je color1   //si es igual salta a color1
			/*or eax, coloresdif*/
			mov[edi], eax
			color1 :
		add esi, BPP   //incrementa y pasa el pixel rojo
			add edi, BPP
			loop muestra

			//suma el total para luego restar y posicionarlo en la linea de abajo         
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			mov eax, 205
			mul BPP
			sub edi, eax

			//MUEVE EL PUNTERO A LA SIGUIENTE LINEA DE LA IMAGEN PARA SEGUIR DIBUJANDO
			mov eax, w
			mul BPP

			add esi, eax
			mov eax, 205
			mul BPP
			sub esi, eax

			pop ecx
			loop repetir
	}
}

void DibujaTrees(int* buffer, int* trees, DIMENSION dmn, POSITION pos, int incremento) {
	//int w = dmn.ANCHO;
	//int h = dmn.ALTO;
	//int x = posTrees.X;
	//int y = posTrees.Y;

	//__asm {

	//	cld
	//	mov esi, trees
	//	mov edi, buffer

	//	mov ecx, ALTO_VENTANA

	//	mov eax, incremento   //mi parametro para incrementar los pixeles, lo mando a eax
	//	mul BPP               //multiplico por los bytes
	//	add esi, eax          //se lo agrego a mi imagen para que se recorra

	//	PonerA :
	//	push ecx  //el alto lo pongo al inicio de mi linea
	//		mov ecx, ANCHO_VENTANA //recorre el ancho

	//		PonerL :
	//	mov eax, [esi]     //mi imagen la paso a eax
	//		mov[edi], eax      //eax lo pongo en mi ventana  (paso mi imagen a la ventana)
	//		add edi, BPP                     //incremento mi pixel 4 bytes
	//		add esi, BPP
	//		loop PonerL
	//		mov eax, 110                    //ancho de mi imagen
	//		mul BPP                          //multiplico por 4 bytes
	//		add esi, eax                     //incremento para que me pase a la linea de abajo
	//		mov eax, ANCHO_VENTANA
	//		mul BPP
	//		sub esi, eax                     //resto para posicionarlo al principio de mi linea
	//		pop ecx   //para sacar el alto y restarle uno
	//		loop PonerA
	//}
}

void DibujaMenuPrincipal(int* buffer, int* menuprincipal, DIMENSION dmn, POSITION pos) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO;
	int x = posMenuPrincipal.X;
	int y = posMenuPrincipal.Y;

	__asm {
		cld

		mov esi, menuprincipal
		mov edi, buffer

		//posicionar mi sprite		
		mov eax, y
		mov ebx, 50000
		mul ebx
		add edi, eax

		mov eax, x
		mul BPP
		add edi, eax

		add edi, 300000


		mov eax, 310       //porque es la cantidad de pixeles de cada cuadrito
		mul BPP
		mul  indiMenuPrincipal
		add esi, eax

		xor ecx, ecx
		mov ecx, h    //asigno mi altura a ecx

		repetir :
		push ecx     //guardo en mi pila ecx
			mov ecx, 310

			//PARA QUITAR EL FONDO ROJO
			muestra :
			mov eax, [esi]         //mi imagen la paso a eax
			cmp eax, 0FFFF2BD8h    //comparo lo rojo 
			je color1   //si es igual salta a color1
			/*or eax, coloresdif*/
			mov[edi], eax
			color1 :
		add esi, BPP   //incrementa y pasa el pixel rojo
			add edi, BPP
			loop muestra

			//suma el total para luego restar y posicionarlo en la linea de abajo         
			mov eax, ANCHO_VENTANA
			mul BPP

			add edi, eax
			mov eax, 310
			mul BPP
			sub edi, eax

			//MUEVE EL PUNTERO A LA SIGUIENTE LINEA DE LA IMAGEN PARA SEGUIR DIBUJANDO
			mov eax, w
			mul BPP

			add esi, eax
			mov eax, 310
			mul BPP
			sub esi, eax

			pop ecx
			loop repetir
	}
}

//void DibujaPelota(int * buffer, int * pelota, DIMENSION dmn3, POSITION pos3)
//{
//	int w = dmn3.ANCHO;
//	int h = dmn3.ALTO-1;
//	int posX = pos3.X;
//	int posY = pos3.Y;
//	int total = 0;
//	int total2 = 0;
//
//	if (indiPelota == 0) {
//		indiPelota++;
//		__asm
//		{
//			cld
//			mov esi, pelota
//			mov edi, buffer
//
//			mov eax, posX
//			mul BPP
//			add edi, eax
//			mov eax, ANCHO_VENTANA
//			mul BPP
//			mul posY
//			add edi, eax
//
//			mov eax, w
//			mul BPP
//			add esi, eax
//
//			xor ecx, ecx
//			mov ecx, h
//
//			repite2 :
//
//			push ecx
//				mov ecx, w
//
//
//				mostrar2 :
//				mov eax, [esi]
//				cmp eax, 0FFED1C24h
//				je Color2
//				mov[edi], eax
//
//
//				Color2 :
//
//				add esi, BPP
//				add edi, BPP
//
//
//				loop mostrar2
//				mov eax, ANCHO_VENTANA
//				mul BPP
//
//				add edi, eax
//				MOV EAX, w
//				MUL BPP
//				SUB EDI, EAX
//
//				mov eax, w
//				mul BPP
//
//				add esi, eax
//				mov eax, w
//				mul BPP
//				sub esi, eax
//
//
//
//				pop ecx
//
//				loop repite2
//		}
//	}else if(indiPelota==1){
//		indiPelota++;
//		__asm{
//		cld
//
//			mov esi, pelota
//			mov edi, buffer
//
//			
//			mov eax, w
//			mul BPP
//			mul h
//			add esi, eax
//			mov total, esi
//
//			mov eax, posX
//			mul BPP
//			add edi, eax
//			mov eax, ANCHO_VENTANA
//			mul BPP
//			mul posY
//			add edi, eax
//
//			xor ecx, ecx
//			mov ecx, w
//
//
//			repite :
//
//		push ecx
//			mov ecx, h
//
//
//			mostrar :
//
//		mov eax, w
//			mul BPP
//			sub esi, eax
//			cmp[esi], 0FFED1C24h
//			je Color1
//			mov eax, [esi]
//			mov[edi], eax
//
//
//			Color1 :
//		add edi, BPP
//			loop mostrar
//
//			mov eax, ANCHO_VENTANA
//			mul BPP
//
//			add edi, eax
//			MOV EAX, h
//			MUL BPP
//			SUB EDI, EAX
//			
//			add total, 4
//			mov esi, total
//
//			pop ecx
//
//			loop repite
//
//
//
//	}
//	}
//	else if (indiPelota == 2) {
//		indiPelota++;
//		__asm
//		{
//			cld
//
//			mov esi, pelota
//			mov edi, buffer
//
//			mov eax, posX
//			mul BPP
//			add edi, eax
//			mov eax, ANCHO_VENTANA
//			mul BPP
//			mul posY
//			add edi, eax
//
//			mov eax, w
//			mul BPP
//			mul h
//			sub eax, BPP
//			add esi, eax
//
//			xor ecx, ecx
//			mov ecx, h
//
//			repite3 :
//
//			push ecx
//				mov ecx, w
//
//
//				mostrar3 :
//			
//				mov eax, [esi]
//				cmp eax, 0FFED1C24h
//				je Color3
//				
//				mov[edi], eax
//
//
//				Color3 :
//
//				sub esi, BPP
//				add edi, BPP
//
//
//				loop mostrar3
//
//				mov eax, ANCHO_VENTANA
//				mul BPP
//
//				add edi, eax
//				MOV EAX, 80
//				MUL BPP
//				SUB EDI, EAX
//
//				pop ecx
//
//				loop repite3
//
//		}
//	}
//	else if (indiPelota == 3) {
//		indiPelota = 0;
//		__asm {
//			cld
//
//			mov esi, pelota
//			mov edi, buffer
//
//			
//			mov eax, w
//			mul BPP
//			mul h
//			mov total2, eax
//			sub esi, BPP
//			mov total2, esi
//
//			mov eax, posX
//			mul BPP
//			add edi, eax
//			mov eax, ANCHO_VENTANA
//			mul BPP
//			mul posY
//			add edi, eax
//
//			xor ecx, ecx
//			mov ecx, w
//
//
//			repite4 :
//
//			push ecx
//				mov ecx, h
//
//
//				mostrar4 :
//
//			mov eax, w
//				mul BPP
//				add esi, eax
//				cmp[esi], 0FFED1C24h
//				je Color4
//				
//				mov eax, [esi]
//				mov[edi], eax
//
//
//				Color4 :
//			add edi, BPP
//				loop mostrar4
//
//				mov eax, ANCHO_VENTANA
//				mul BPP
//
//				add edi, eax
//				MOV EAX, h
//				MUL BPP
//				SUB EDI, EAX
//
//				sub total2, 4
//				mov esi, total2
//
//				pop ecx
//
//				loop repite4
//
//
//
//		}
//	}
//}

#pragma endregion



