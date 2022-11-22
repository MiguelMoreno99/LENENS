#pragma once
#define WIN32_LEAN_AND_MEAN //No agrega librerías que no se vayan a utilizar
#include <Windows.h>
#include <stdio.h>
#include <dinput.h>
#include <gdiplus.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <fstream>
#include "CEnsamblador.h"
using namespace Gdiplus;
Puntuaciones Puntuaciones1;

//Funcion para manejar eventos del teclado dependiendo de la(s) tecla(s) que se haya(n) presionado.
void KeysEvents(HWND hWnd){

	if (iniciojuego){
		if (KEYS[input.Enter]) {
			iniciojuego = false;
		}
		indiMenuPrincipal++;
		indiMenuPrincipal = indiMenuPrincipal >= 6 ? 0 : indiMenuPrincipal;

	}
	if(!iniciojuego){
		if (posRewardCoin.X==21){
			posRewardCoin.X = 0;
			posObstacleCarLeft.X = 0;
			posObstacleCarRight.X = 0;
			posObstacleCarStatic.X = 0;
			posObstacleRock.X = 0;
			TiempoDeJuego++;
			RamdomRoadSide = rand() % 3 + 1;
			RamdomRoadItem = rand() % 5 + 1;
		}
		if (TiempoDeJuego == 25) {
			Puntuaciones1.GuardarPuntuacion(TotalPoints);
			TotalPoints = 0;
			TiempoDeJuego = 0;
			RoadPlayerCar = 2;
			indiPoints = 0;
			iniciojuego = true;
			posRewardCoin.X = 0;
			posObstacleCarLeft.X = 0;
			posObstacleCarRight.X = 0;
			posObstacleCarStatic.X = 0;
			posObstacleRock.X = 0;
			posFinish.X = 0;
			RamdomRoadSide = rand() % 3 + 1;
			RamdomRoadItem = rand() % 5 + 1;
			MessageBox(hWnd, L"Felicidades! :)", L"Ganaste!!", MB_ICONINFORMATION);
		}
		if ((posRewardCoin.X == 18) && (RoadPlayerCar == RamdomRoadSide)) {
			if (RamdomRoadItem == 5 || RamdomRoadItem == 6 || RamdomRoadItem == 7) {
				posRewardCoin.X++;
				indiPoints++;
				TotalPoints++;
				if (indiPoints == 10) {
					TotalPoints++;
					indiPoints2++;
				}
			}
			else{
				TotalPoints = 0;
				TiempoDeJuego = 0;
				indiPoints = 0;
				RoadPlayerCar = 2;
				iniciojuego = true;
				posRewardCoin.X = 0;
				posObstacleCarLeft.X = 0;
				posObstacleCarRight.X = 0;
				posObstacleCarStatic.X = 0;
				posObstacleRock.X = 0;
				posFinish.X = 0;
				RamdomRoadSide = rand() % 3 + 1;
				RamdomRoadItem = rand() % 5 + 1;
				MessageBox(hWnd, L"Lo Siento :(", L"Perdiste", MB_ICONEXCLAMATION);
			}
		}
		else {
			posRewardCoin.X++;
			posObstacleCarLeft.X++;
			posObstacleCarRight.X++;
			posObstacleCarStatic.X++;
			posObstacleRock.X++;
		}

		if (KEYS[input.D] || KEYS[input.Right]) {
			indiPlayerCar = indiPlayerCar >= 4 ? 0 : indiPlayerCar;
			//posPlayerCar.X += 10;
			switch (RoadPlayerCar){
				case 1: {
					RoadPlayerCar = 2;
					indiPlayerCar=3;
				}break;
				case 2: {
					RoadPlayerCar = 3;
					indiPlayerCar = 3;
				}break;
				case 3: {
					RoadPlayerCar = 3;
					indiPlayerCar = 0;
				}break;
				default:
				break;
			}
		}
		if (KEYS[input.A] || KEYS[input.Left]) {
			indiPlayerCar = indiPlayerCar >= 4 ? 0 : indiPlayerCar;
			//posPlayerCar.X -= 10;
			switch (RoadPlayerCar) {
			case 1: {
				RoadPlayerCar = 1;
				indiPlayerCar = 0;
			}break;
			case 2: {
				RoadPlayerCar = 1;
				indiPlayerCar = 1;
			}break;
			case 3: {
				RoadPlayerCar = 2;
				indiPlayerCar = 1;
			}break;
			default:
				break;
			}
		}

		indiPoints = indiPoints >= 10 ? 0 : indiPoints;
		indiPoints2 = indiPoints2 >= 10 ? 0 : indiPoints2;

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

	}
}

//Funcion para cargar imagenes y obtener un puntero al area de memoria reservada para la misma.
 //@param rutaImagen.			Nombre o ruta de la imagen a cargar en memoria.
 //@return unsigned char *.	Direccion base de la imagen.
unsigned char* CargaImagen(WCHAR rutaImagen[], DIMENSION* dmn)
{
	unsigned char* ptrImagen;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR  gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	Bitmap* bitmap = new Bitmap(rutaImagen);
	BitmapData* bitmapData = new BitmapData;

	dmn->ANCHO = bitmap->GetWidth();
	dmn->ALTO = bitmap->GetHeight();

	Rect rect(0, 0, dmn->ANCHO, dmn->ALTO);

	//Reservamos espacio en memoria para la imagen
	bitmap->LockBits(&rect, ImageLockModeRead, PixelFormat32bppRGB, bitmapData);

	//"pixels" es el puntero al area de memoria que ocupa la imagen
	unsigned char* pixels = (unsigned char*)bitmapData->Scan0;

	//"tamaño" lo usaremos para reservar los bytes que necesita la imagen. 
	//Para calcular la cantidad de bytes total necesitamos multiplicamos el area de la imagen * 4. 
	//Se multiplica por 4 debido a que cada pixel ocupa 4 bytes de memoria. Noten el 3er parametro de la funcion LockBits, dos lineas de codigo arriba.
	//PixelFormat32bppARGB -> Specifies that the format is 32 bits per pixel; 8 bits each are used for the alpha, red, green, and blue components.
	//Mas info: https://msdn.microsoft.com/en-us/library/system.drawing.imaging.pixelformat(v=vs.110).aspx
	int tamaño;
	tamaño = dmn->ANCHO * dmn->ALTO * 4;
	//hagamos un try de la reserva de memoria
	try
	{
		ptrImagen = new unsigned char[tamaño];
	}
	catch (...)
	{
		return NULL;
	}

	//Después de este for, ptrImagen contiene la direccion en memoria de la imagen.
	for (int i = 0, j = tamaño; i < j; i++)
	{
		ptrImagen[i] = pixels[i];
	}

	//Es necesario liberar el espacio en memoria, de lo contrario marcaria una excepcion de no hay espacio de memoria suficiente.
	bitmap->UnlockBits(bitmapData);
	delete bitmapData;
	delete bitmap;

	GdiplusShutdown(gdiplusToken);

	return ptrImagen;
}

//Funcion usada para la inicializacion de variables y reserva de espacio en memoria.
void Init(){

	posPlayerCar.X = 0;
	posPlayerCar.Y = 0;
	scale=2;

	posObstacleCarLeft.X = 0;
	posObstacleCarLeft.Y = 0;

	posObstacleCarRight.X = 0;
	posObstacleCarRight.Y = 0;

	posObstacleCarStatic.X = 0;
	posObstacleCarStatic.Y = 0;

	posRoad.X = 150;
	posRoad.Y = 0;

	posFinish.X = 0;
	posFinish.Y = 0;

	posRewardCoin.X = 0;
	posRewardCoin.Y = 0;

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
	ptrPoints2 = CargaImagen(TEXT("Points.png"), &dmnPoints2);
	ptrFinish = CargaImagen(TEXT("Finish.png"), &dmnFinish);
	ptrTrees = CargaImagen(TEXT("Trees.png"), &dmnTrees);
	ptrMenuPrincipal = CargaImagen(TEXT("MenuPrincipal.png"), &dmnMenuPrincipal);
	ptrRoad = CargaImagen(TEXT("Road.png"), &dmnRoad);
	//ptrpelota = CargaImagen(TEXT("pelotita.png"),&dmnPelota);
}
