// En este archivo implementamos la clase PuenteCae, definida en el
// archivo de cabecera PuenCae.h

// Inclusion de librerias.

#include "RutinasSDL/rutinasSDL.h"
#include "PuenCae.h"

// Funciones de la clase

void PuenteCae::Inicializar(int CX, int CY, long MiRetardo)
{
	long AuxX;
	int  AuxY;
	CasillaCoordenada(AuxX,AuxY,CX,CY);
	MX  = AuxX;
	MY  = AuxY;
	VY  = 0;
	MCX = CX;
	MCY = CY;
	MiSuperficie   = imglstTerreno[Nivel->Terreno(CX,CY) - 1];
	TiempoRestante = MiRetardo;
	Nivel->SetTerreno(CX,CY, caVacia);
}

void PuenteCae::Check()
{
	if (TiempoRestante == 0)
	{
		// Movimiento!
		MY += VY * Delta + Delta * Delta * AceleracionGravedad / 2;
		VY += Delta * AceleracionGravedad;
		if (MY > 600)
			Matarme();
	}
	else
	{
		TiempoRestante -= Delta;
		if (TiempoRestante <= 0)
		{
			PlayWav(sndPuenteCae);
			TiempoRestante = 0;
			Nivel->SetEventos(MCX, MCY, evNulo);
		}
	}
}
