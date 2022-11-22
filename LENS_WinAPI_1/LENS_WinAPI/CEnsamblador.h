#pragma once
#define WIN32_LEAN_AND_MEAN //No agrega librerías que no se vayan a utilizar
#include <Windows.h>
#include <stdio.h>
#include <dinput.h>
#include <gdiplus.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "CStructVarConst.h"
using namespace Gdiplus;

void DibujaFondo(int* buffer, int* imagen, DIMENSION dmn/* int incremento*/) {
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

		PonerA:
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

void DibujaRoad(int* buffer, int* road, DIMENSION dmn, POSITION pos) {
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

void DibujaPlayerCar(int* buffer, int* playercar, DIMENSION dmn, POSITION pos, int road) {
		int w = dmn.ANCHO;
		int h = dmn.ALTO;
		int x = posPlayerCar.X + 270000;
		int y = posPlayerCar.Y;
		switch (road) {
		case 1: {
			road = 696;
		}break;
		case 2: {
			road = 960;
		}break;
		case 3: {
			road = 1224;
		}break;
		default:
			break;
		}
			__asm {
				cld
				//cargo el sprite
				mov esi, playercar
				mov edi, buffer

				//posicionar mi sprite		
				mov eax, y
				mov ebx, 1000
				mul ebx
				add edi, eax

				mov eax, x
				mul BPP
				add edi, eax

				add edi, road


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

void DibujaObstacleCarLeft(int* buffer, int* obstaclecarleft, DIMENSION dmn, POSITION pos, int roadrandom) {
		int w = dmn.ANCHO;
		int h = dmn.ALTO;
		int x = posObstacleCarLeft.X * 15000;
		int y = posObstacleCarLeft.Y;
		switch (roadrandom) {
		case 1: {
			roadrandom = 696;
		}break;
		case 2: {
			roadrandom = 960;
		}break;
		case 3: {
			roadrandom = 1224;
		}break;
		default:
			break;
		}

		__asm {
			cld
			//cargo el sprite
			mov esi, obstaclecarleft
			mov edi, buffer

			//posicionar mi sprite		
			mov eax, y
			mov ebx, 1000
			mul ebx
			add edi, eax

			mov eax, x
			mul BPP
			add edi, eax

			add edi, roadrandom

			mov eax, 35   //porque es la cantidad de pixeles de cada cuadrito
			mul BPP
			mul indiObstacleCarLeft    //para moverlo con la tecla que asigne, si le pico una ves el 0 default se convierte en uno, de multiplica por 59 y se mueve el sprite
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

void DibujaObstacleCarRight(int* buffer, int* obstaclecarright, DIMENSION dmn, POSITION pos, int roadrandom) {
		int w = dmn.ANCHO;
		int h = dmn.ALTO;
		int x = posObstacleCarRight.X * 15000;
		int y = posObstacleCarRight.Y;
		switch (roadrandom) {
		case 1: {
			roadrandom = 696;
		}break;
		case 2: {
			roadrandom = 960;
		}break;
		case 3: {
			roadrandom = 1224;
		}break;
		default:
			break;
		}

		__asm {
			cld
			//cargo el sprite
			mov esi, obstaclecarright
			mov edi, buffer

			//posicionar mi sprite		
			mov eax, y
			mov ebx, 1000
			mul ebx
			add edi, eax

			mov eax, x
			mul BPP
			add edi, eax

			add edi, roadrandom

			mov eax, 35   //porque es la cantidad de pixeles de cada cuadrito
			mul BPP
			mul indiObstacleCarRight    //para moverlo con la tecla que asigne, si le pico una ves el 0 default se convierte en uno, de multiplica por 59 y se mueve el sprite
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

void DibujaObstacleCarStatic(int* buffer, int* obstaclecarstatic, DIMENSION dmn, POSITION pos, int roadrandom) {
		int w = dmn.ANCHO;
		int h = dmn.ALTO;
		int x = posObstacleCarStatic.X * 15000;
		int y = posObstacleCarStatic.Y;
		switch (roadrandom) {
		case 1: {
			roadrandom = 696;
		}break;
		case 2: {
			roadrandom = 960;
		}break;
		case 3: {
			roadrandom = 1224;
		}break;
		default:
			break;
		}

		__asm {
			cld
			//cargo el sprite
			mov esi, obstaclecarstatic
			mov edi, buffer

			//posicionar mi sprite		
			mov eax, y
			mov ebx, 1000
			mul ebx
			add edi, eax

			mov eax, x
			mul BPP
			add edi, eax

			add edi, roadrandom

			mov eax, 35   //porque es la cantidad de pixeles de cada cuadrito
			mul BPP
			mul indiObstacleCarStatic    //para moverlo con la tecla que asigne, si le pico una ves el 0 default se convierte en uno, de multiplica por 59 y se mueve el sprite
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

void DibujaObstacleRock(int* buffer, int* obstaclerock, DIMENSION dmn, POSITION pos, int roadrandom) {
		int w = dmn.ANCHO;
		int h = dmn.ALTO;
		int x = posObstacleRock.X * 15000;
		int y = posObstacleRock.Y;
		switch (roadrandom) {
		case 1: {
			roadrandom = 696;
		}break;
		case 2: {
			roadrandom = 960;
		}break;
		case 3: {
			roadrandom = 1224;
		}break;
		default:
			break;
		}

		__asm {
			cld

			mov esi, obstaclerock
			mov edi, buffer

			//posicionar mi sprite		
			mov eax, y
			mov ebx, 1000
			mul ebx
			add edi, eax

			mov eax, x
			mul BPP
			add edi, eax

			add edi, roadrandom


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

void DibujaRewardCoin(int* buffer, int* rewardcoin, DIMENSION dmn, POSITION pos, int roadrandom) {
		int w = dmn.ANCHO;
		int h = dmn.ALTO;
		int x = posRewardCoin.X * 15000;
		int y = posRewardCoin.Y ;
		switch (roadrandom){
			case 1:{
				roadrandom = 740;
			}break;
			case 2: {
				roadrandom = 1000;
			}break;
			case 3: {
				roadrandom = 1260;
			}break;
			default:
				break;
		}

		__asm {
			cld

			mov esi, rewardcoin
			mov edi, buffer

			//posicionar mi sprite		
			mov eax, y
			mov ebx, 1000
			mul ebx
			add edi, eax

			mov eax, x
			mul BPP
			add edi, eax

			add edi, roadrandom

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
			mov ebx, 1000
			mul ebx
			add edi, eax

			mov eax, x
			mul BPP
			add edi, eax

			add edi, 9060


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

void DibujaPoints2(int* buffer, int* points2, DIMENSION dmn, POSITION pos) {
	int w = dmn.ANCHO;
	int h = dmn.ALTO;
	int x = posPoints2.X;
	int y = posPoints2.Y;

	__asm {
		cld

		mov esi, points2
		mov edi, buffer

		//posicionar mi sprite		
		mov eax, y
		mov ebx, 1000
		mul ebx
		add edi, eax

		mov eax, x
		mul BPP
		add edi, eax

		add edi, 9000


		mov eax, 30       //porque es la cantidad de pixeles de cada cuadrito
		mul BPP
		mul  indiPoints2
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
		int x = posFinish.X*12600;
		int y = posFinish.Y;

		__asm {
			cld

			mov esi, finish
			mov edi, buffer

			//posicionar mi sprite		
			mov eax, y
			mov ebx, 1000
			mul ebx
			add edi, eax

			mov eax, x
			mul BPP
			add edi, eax

			add edi, 700


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

