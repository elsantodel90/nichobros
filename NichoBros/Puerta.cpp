// En este archivo implementamos las funciones más complejas de la
// clase Puerta, definiendo así su comportamiento.

// Incluimos librerías.

#include "RutinasSDL/rutinasSDL.h"
#include "Puerta.h"

// Funciones de la clase.

void Puerta::Inicializar(int X, int Y,bool R)
{
	MCX = X;
	MCY = Y;
	long AuxX;
	int  AuxY;
    CasillaCoordenada(AuxX,AuxY,MCX,MCY);
	MX = AuxX;
	MY = AuxY;
	MBY = MY;
	Nivel->SetEventos(X,Y,evPisoRigido);
	Nivel->SetEventos(X,Y + 1,evPisoRigido);
	Nivel->SetEventos(X,Y + 2,evPisoRigido);
	Activo    = false;
	Rojo      = R;
	MiImagen  = (R)?(apaPuertaR):(apaPuertaC);
}

void Puerta::Check()
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
	if ((Rojo)?(ApretadoBotR):(ApretadoBotC))
	{
		VY = 0;
		if (MY > MBY - 90)
		{
			MY -= VelocidadLevantadaPuerta * Delta;
			if (MY < MBY - 90)
				MY = MBY - 90;
		}
		else
			MY = MBY - 90;
		// Limpiamos los eventos.
		for (int i = MCY + 2;i >= MCY;i--)
		{
			if (i * 30 < MY + 90)
				return;
			Nivel->SetEventos(MCX,i,evNulo);
		}
	}
	else
	{
		// La caída es libre, y además tiene la propiedad de que mata a lo que choca(falta eso).
		if (MY < MBY)
		{
			MY += VY * Delta + 0.5 * AceleracionGravedad * Delta * Delta;
			VY += AceleracionGravedad * Delta;
			if (MY > MBY)
				MY = MBY;
			for (int i = MCY;i <= MCY + 2;i++)
			{
				if (i * 30 > MY + 90)
					return;
				Nivel->SetEventos(MCX,i,evZonaDueleRigido);
			}
		}
		else
		{
			MY = MBY;
			for (int i = MCY;i <= MCY + 2;i++)
			{
				if (i * 30 > MY + 90)
					return;
				Nivel->SetEventos(MCX,i,evPisoRigido);
			}
		}
	}
	
}
