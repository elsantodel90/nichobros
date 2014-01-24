// En este archivo implementamos las funciones mas complejas de la
// clase BolaFuegoSalta.

// Inclusion de librerias...
#include <cmath>
#include "RutinasSDL/rutinasSDL.h"
#include "BolFuSal.h"

// Funciones de la clase.

void BolaFuegoSalta::Inicializar(int X,int Y,bool pez)
{
	long RX;
	int  RY;
	CasillaCoordenada(RX,RY,X,Y);
	MX	     = RX;
	MY	     = 600;
	VY = IVY = sqrt(2.0 * (600 - RY) * AceleracionGravedad);
	Activo   = false;
	MTime	 = 0;
    soypez = pez;
}

void BolaFuegoSalta::Check()
{
	if (MX - Pos < -30)
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
	if (MTime == 0)
	{
		MY -= VY * Delta + Delta * Delta * AceleracionGravedad / 2.0;
	    VY -= AceleracionGravedad * Delta;
		RECT R;
		SetRect(&R,(long)MX,(long)MY,(long)MX + 30,(long)MY + 30);
        if (soypez)
        {
            if (ColisionaNicholas(R,imglstEnemigos[(VY >= 0)?(36):(35)]) != ColNula)
                Hitazo = true;
        }
        else
        {
            if (ColisionaNicholas(R,imglstFuego[(VY >= 0)?(fuegArribaPico):(fuegAbajoPico)]) != ColNula)
                Hitazo = true;
        }
	}
	else
	{
		MTime -= Delta;
		if (MTime < 0)
			MTime = 0;
	}
	if (MY > 600)
	{
		MY	  = 600;
		VY	  = IVY;
		MTime = RetardoBolaPico;
	}
}
