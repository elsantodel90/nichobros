// En este archivo implementamos las funciones más complejas de la
// clase Rebotador, definiendo así su comportamiento.

// Incluimos librerías.

#include "RutinasSDL/rutinasSDL.h"
#include "Reboter.h"

// Funciones de la clase.

void Rebotador::Inicializar(int X, int Y)
{
	MCX = X;
	MCY = Y;
    CasillaCoordenada(MX,MY,X,Y);
	NumeroImagen = 0;
}

void Rebotador::Check()
{
	if (ColRebotador && RebX == MCX && RebY == MCY)
	{
        Nicho->IVX = 0;
        Nicho->IVY = 0;
		if (NumeroImagen == TAMANO_REBOTADOR - 1)
			ColRebArr = true;
		if (ColRebArr)
			NumeroImagen--;
		else
			NumeroImagen++;
		Nicho->ImagenNicholasY += (ColRebArr)?(-2):(2);
		if (NumeroImagen == 0)
		{
			ColRebotador = false;
			Nicho->VelSalto = -Nicho->VelSalto * CoefReboteSimple;
			if (Nicho->VelSalto > MaxReboteSimple)
				Nicho->VelSalto = MaxReboteSimple;
			Nicho->ImagenNicholasY = MY - 90;
		}
	}
}
