// En este archivo se implementan las funciones m�s complejas de
// la clase explosi�n.

// Incluimos las librer�as.

#include <cmath>
#include "RutinasSDL/rutinasSDL.h"
#include "Secuen.h"

// Implementamos las funciones de la clase.

void Secuencia::Inicializar(long X, long Y, long CAn, long CAl, long TI, long AnFila, LPDIRECTDRAWSURFACE S)
{
    NumeroImagen = 0;
	AnchoFila    = AnFila;
	NImagen      = TI;
	CuadroAncho  = CAn;
	CuadroAlto   = CAl;
	MX           = X;
	MY			 = Y;
	Surf         = S;
}

void Secuencia::Check()
{
	if (Ciclo % 2 == 0)
		NumeroImagen++;
	if (NumeroImagen >= NImagen)
        Matarme();
}
