#pragma once
#define WIN32_LEAN_AND_MEAN //No agrega librerías que no se vayan a utilizar
#include <Windows.h>
#include <stdio.h>
#include <dinput.h>
#include <gdiplus.h>
#include <gl/gl.h>
#include <gl/glu.h>
using namespace Gdiplus;
using namespace std;

#pragma region Estructs
	//Estructura con las coordenadas de los sprites en pantalla en un plano 2D 
	struct POSITION {
		int X;
		int Y;
	};
	//Funcion que regresa la posicion del sprite en pantalla. @param x. Coordenada X en la ventana. @param y. Coordenada Y en la ventana.
	POSITION setPosition(int x, int y) {
		POSITION p;
		p.X = x;
		p.Y = y;
		return p;
	}
	//Estructura con las dimensiones de los sprites a cargar y desplegar en pantalla 
	struct DIMENSION {
		int ANCHO;
		int ALTO;
	};
	//Estructura con la enumeracion de algunas teclas. Se tiene un objeto o variable del tipo de esta estructura, llamado 'input' que sera para acceder a cada uno de las elementos de la enumeracion; ejemplo: input.A para la tecla 'A'. declaracion del objeto de la estructura Input 
	struct Input {
		enum Keys {
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

	class Puntuaciones {

	private:
		// 1 Variable
		int id = 0;
		int Puntuacion;
		Puntuaciones* Ptr_Puntuaciones_anterior = NULL;
		Puntuaciones* Ptr_Puntuaciones_siguiente = NULL;
	public:
		//Funcciones
		void GuardarPuntuacion(int _Puntuacion) {

			if (PtrOrigenPuntuaciones == NULL) {
				PtrOrigenPuntuaciones = new Puntuaciones;
				PtrOrigenPuntuaciones->id++;
				id++;
				PtrOrigenPuntuaciones->Puntuacion = _Puntuacion;
				PtrOrigenPuntuaciones->Ptr_Puntuaciones_siguiente = NULL;
				PtrOrigenPuntuaciones->Ptr_Puntuaciones_anterior = NULL;
			}
			else {
				PtrAuxiliarPuntuaciones = PtrOrigenPuntuaciones;
				while (PtrAuxiliarPuntuaciones->Ptr_Puntuaciones_siguiente != NULL) {
					PtrAuxiliarPuntuaciones = PtrAuxiliarPuntuaciones->Ptr_Puntuaciones_siguiente;
				}
				PtrAuxiliarPuntuaciones->Ptr_Puntuaciones_siguiente = new Puntuaciones;
				PtrAuxiliarPuntuaciones->Ptr_Puntuaciones_siguiente->id = PtrAuxiliarPuntuaciones->id + 1;
				id++;
				PtrAuxiliarPuntuaciones->Ptr_Puntuaciones_siguiente->Puntuacion = _Puntuacion;
				PtrAuxiliarPuntuaciones->Ptr_Puntuaciones_siguiente->Ptr_Puntuaciones_siguiente = NULL;
				PtrAuxiliarPuntuaciones->Ptr_Puntuaciones_siguiente->Ptr_Puntuaciones_anterior = PtrAuxiliarPuntuaciones;
			}
			PtrAuxiliarPuntuaciones = PtrOrigenPuntuaciones;
			return;
		}

		void GuardarPuntuacionTxt() {

			if (PtrOrigenPuntuaciones != NULL) {
				ofstream OfGuardar("Puntuaciones.txt");
				if (PtrOrigenPuntuaciones->Ptr_Puntuaciones_siguiente==NULL) {
					OfGuardar << PtrOrigenPuntuaciones->Puntuacion << "\n";
					OfGuardar << PtrOrigenPuntuaciones->id << "\n";
				}
				else {
					PtrAuxiliarPuntuaciones = PtrOrigenPuntuaciones;
					while (PtrAuxiliarPuntuaciones != NULL) {
						OfGuardar << PtrAuxiliarPuntuaciones->Puntuacion << "\n";
						OfGuardar << PtrAuxiliarPuntuaciones->id << "\n";
						PtrAuxiliarPuntuaciones = PtrAuxiliarPuntuaciones->Ptr_Puntuaciones_siguiente;
					}
				}
				OfGuardar.close();
			}
		}

		void CargarPuntuacionTxt() {

			ifstream IfCargar("Puntuaciones.txt", ios::app);
			while (!IfCargar.eof()) {
				id = 0;
				IfCargar >> Puntuacion;
				IfCargar >> id;
				if (id==0){
					break;
				}
				GuardarPuntuacion(Puntuacion);
				}
			IfCargar.close();
		}

	}*PtrOrigenPuntuaciones = NULL, * PtrAuxiliarPuntuaciones = NULL;

#pragma endregion

#pragma region Var y Const
	//Variable constante para calcular el ancho de la ventana 
	const int ANCHO_VENTANA = 600;
	//Variable constante para calcular el alto de la ventana 
	const int ALTO_VENTANA = 600;
	//Variable constante que define la cantidad de bytes por pixel, usada en las operaciones de desplegar sprites/imagenes en pantalla 
	const int BPP = 4;
	/* Variable constante que define el intervalo del contador o timer en milisegundos, con cada TICK del contador se ejecuta el codigo dentro del case WM_TIMER en la funcion WndProc */
	const int TICK = 90;
	//Variables constantes de los colores primarios de un pixel de 32 bits 
	const unsigned int BLUE = 0xFF0000FF;
	const unsigned int GREEN = 0xFF00FF00;
	const unsigned int RED = 0xFFFF0000;
	//Variables Globales
	int RamdomRoadItem;
	int RamdomRoadSide;
	int TiempoDeJuego = 23;
	int RoadPlayerCar = 2;
	int TotalPoints = 0;
	int* ptrBuffer;
	unsigned char* ptrBack;
	unsigned char* ptrPlayerCar;
	unsigned char* ptrObstacleCarLeft;
	unsigned char* ptrObstacleCarRight;
	unsigned char* ptrObstacleCarStatic;
	unsigned char* ptrObstacleRock;
	unsigned char* ptrRewardCoin;
	unsigned char* ptrPoints;
	unsigned char* ptrPoints2;
	unsigned char* ptrFinish;
	unsigned char* ptrTrees;
	unsigned char* ptrRoad;
	unsigned char* ptrMenuPrincipal;
	//unsigned char * ptrpelota;
	DIMENSION dmnBack, dmnPlayerCar, dmnObstacleCarLeft, dmnObstacleCarRight,
	dmnObstacleCarStatic, dmnObstacleRock, dmnRewardCoin, dmnPoints, dmnPoints2, dmnFinish, dmnTrees, dmnRoad, dmnMenuPrincipal/*dmnPelota*/;
	POSITION posPlayerCar;
	POSITION posObstacleCarLeft;
	POSITION posObstacleCarRight;
	POSITION posObstacleCarStatic;
	POSITION posObstacleRock;
	POSITION posRewardCoin;
	POSITION posPoints;
	POSITION posPoints2;
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
	int indiPoints2 = 0;
	int indiMenuPrincipal = 0;
	int indiRoad = 0;
	//int indiPelota=0;
	int initfondo;
	bool KEYS[256];
	int increTrees = 0;    //que incremente el valor del pixel 
	int contadorsh = 10;
	int coloresdif = 0;
	bool mirror;
	bool iniciojuego = true;
	//bool loboD=TRUE;
	//bool lobomirror = FALSE;
	int start;
	int scale = 1;
#pragma endregion