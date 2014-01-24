// En este archivo se implementan las funciones más complejas de
// la clase Triturin.

// Incluimos librerías...

#include "RutinasSDL/rutinasSDL.h"
#include "Triturin.h"

// Funciones de la clase.

void Triturin::Inicializar(int X, int Y)
{
	Nivel->SetEventos(X,Y,evPisoRigido);
	long AuxX;
	int  AuxY;
	MCX          = X;
	MCY          = Y + 1;
	CasillaCoordenada(AuxX,AuxY,MCX,MCY);
	MX			 = AuxX;
	MY			 = AuxY;
	CuantoAfuera = 0;
	Falta		 = RetardoCaidaTriturin;
	Activo		 = false;
	VY			 = 0;
}

bool Triturin::HuboChoque()
{
	int CY;
	MCY2 = MCY + ((long)CuantoAfuera) / 30;
	for (CY = MCY;CY <= MCY2;CY++)
	{
		if (Nivel->Eventos(MCX,CY) == evPuenteFlojo ||
			Nivel->Eventos(MCX,CY) == evPuenteCae)
		{
			ObjetoBase *NuevoPuente = new PuenteCae();
			PuenteCae  *ElPuente	= (PuenteCae *)NuevoPuente;
			ElPuente->Inicializar(MCX,CY,(Nivel->Eventos(MCX,CY) == evPuenteCae)?(RetardoCaidaPuente):(RetardoPuenteFlojo));
			PuentesCaen->Agregar(NuevoPuente);
			Nivel->SetEventos(MCX,CY, evPisoRigido);
			CuantoAfuera = (CY - MCY) * 30;
			return true;
		}
		else if (Nivel->Eventos(MCX,CY) != evNulo)
		{
			CuantoAfuera = (CY - MCY) * 30;
			return true;
		}
	}
	return false;
}

void Triturin::CheckCol()
{
    if (CuantoAfuera <= 90)
    {
        RECT R;
        SetRect(&R,(long)MX     ,(long)(MY),
                   (long)MX + 30,(long)(MY + CuantoAfuera));
        if (CuantoAfuera > 0 && 
            ColisionaNicholas(R,imglstCosasMalas[cmTriturin],90 - (long)CuantoAfuera) != ColNula)
        {
            Hitazo = true;
            return;
        }
    }
    else
    {
        RECT R;
        SetRect(&R,(long)MX     ,(long)(MY + CuantoAfuera - 90),
                   (long)MX + 30,(long)(MY + CuantoAfuera));
        if (ColisionaNicholas(R,imglstCosasMalas[cmTriturin]) != ColNula)
        {
            Hitazo = true;
            return;
        }
    }
}

void Triturin::Check()
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
	if (Falta > 0)
		if (CuantoAfuera == 0)
			Falta -= Delta;
		else
		{
			if (Falta2 > 0)
			{
				Falta2 -= Delta;
				CheckCol();
			}
			else
			{
				CuantoAfuera -= VelocidadMovTriturin * Delta;

				if (CuantoAfuera <= 0)
					CuantoAfuera = 0;
				else
					CheckCol();
			}
		}
	else
	{
		if (!HuboChoque())
		{
			CuantoAfuera += VY * Delta + Delta * Delta * AceleracionGravedad / 2;
			VY			 += AceleracionGravedad * Delta;
			CheckCol();
		}
		else
		{
            PlayWav(sndPumPum);
			Falta  = RetardoCaidaTriturin;
			Falta2 = RetardoLevantadaTriturin;
			VY	   = 0;
		}
	}
}
