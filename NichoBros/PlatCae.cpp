// En este archivo implementamos la clase PlatCae, definida en el
// archivo de cabecera PlatCae.h

// Inclusion de librerias.

#include "RutinasSDL/rutinasSDL.h"
#include "PlatCae.h"

// Definicion de algo util.

#define NoHayCaida -1

// Funciones de la clase

void PlatCae::Inicializar(int CX, int CY)
{
	long AuxX;
	int  AuxY;
	Nivel->SetEventos(CX    ,CY, evPlatCae);
	Nivel->SetEventos(CX + 1,CY, evPlatCae);
	Nivel->SetEventos(CX + 2,CY, evPlatCae);
	CasillaCoordenada(AuxX,AuxY,CX,CY);
	MX			   = AuxX;
	MY			   = AuxY;
	MCX            = CX;
	MCY            = CY;
	VY			   = 0;
	TiempoRestante = NoHayCaida;
}

void PlatCae::Check()
{
	if (MX - Pos < - 400)
	{
        Matarme();
        return;
    }
    if (!Activo)
    {
        if (MX - Pos < 800)
            Activo = true;
        else
            return;
    }
	if (TiempoRestante == NoHayCaida)
	{
		if (APC && 
			APCX >= MCX && APCX <= MCX + 2 && 
			APCY == MCY)
		{
			APC = false;
			Nicho->ImagenNicholasY = MY - 90;
			TiempoRestante = RetardoPlataformaCae;
		}
		
	}
	else if (TiempoRestante == 0)
	{
		// Movimiento!
		MY += VY * Delta + Delta * Delta * AceleracionGravedad / 2;
		VY += Delta * AceleracionGravedad;
		if (MY > 700)
		{
			Matarme();
			return;
		}
	}
	else
	{
		TiempoRestante -= Delta;
		if (TiempoRestante <= 0)
		{
			PlayWav(sndPuenteCae);
			TiempoRestante = 0;
			Nivel->SetEventos(MCX    ,MCY,evNulo);
			Nivel->SetEventos(MCX + 1,MCY,evNulo);
			Nivel->SetEventos(MCX + 2,MCY,evNulo);
		}
	}
}
