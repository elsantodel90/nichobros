// En este archivo se implementan las funciones más complejas de
// la clase PinchePalo.

// Incluimos librerías...

#include "RutinasSDL/rutinasSDL.h"
#include "PinPalo.h"

// Funciones de la clase.

void PinchePalo::Inicializar(int X, int Y)
{
	long MMX;
	int  MMY;
	CasillaCoordenada(MMX,MMY,X,Y);
	MX			 = MMX;
	MY			 = MMY;
	CuantoAfuera = 0;
	Falta		 = RetardoSalidaPinchePalo;
	Activo		 = false;
}

void PinchePalo::Check()
{
	if (!Activo)
    {
		if (MX - Pos < 800)
			Activo = true;
		else
			return;
    }
	if (Falta > 0)
	{
		Falta -= Delta;
		if (CuantoAfuera > 0)
		{
			Falta = RetardoSalidaPinchePalo;
			CuantoAfuera -= VelocidadMovPinchePalo * Delta;
			if (CuantoAfuera < 0)
				CuantoAfuera = 0;
		}
	}
	else
	{
		Falta = 0;
		if (CuantoAfuera < 56)
		{
			CuantoAfuera += VelocidadMovPinchePalo * Delta;
			if (CuantoAfuera > 56)
				CuantoAfuera = 56;
		}
		else
			Falta = RetardoSalidaPinchePalo;

	}
	RECT R;
	SetRect(&R,(long)MX     ,(long)(MY - CuantoAfuera),
		       (long)MX + 22,(long)MY);
	if (CuantoAfuera > 0 && ColisionaNicholas(R,imglstCosasMalas[cmPinchePalo]) != ColNula)
	{
		Hitazo = true; // Metodo practico para ensartar a Nicholas. :-)
		return;
	}
}
