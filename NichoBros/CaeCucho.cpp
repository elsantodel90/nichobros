// En este archivo se implementan las funciones más complejas de
// la clase Caecucho.

// Incluimos librerías...

#include "RutinasSDL/rutinasSDL.h"
#include "Secuen.h"
#include "CaeCucho.h"

// Funciones de la clase.

void CaeCucho::Inicializar(int X, int Y, eTipoCaeCucho T)
{
	long AuxX;
	int  AuxY;
	MCX          = X;
	MCY          = Y + 1;
	CasillaCoordenada(AuxX,AuxY,X,Y);
	MX			 = AuxX;
	MY			 = AuxY;
	Activo		 = false;
	Libre        = false;
	VY			 = 0;
	Tipo         = T;

	ImagenCaeCucho = ImagenCaeCuchoBase;
}

bool CaeCucho::HuboChoque()
{
	int CY;
	for (CY = MICY;CY <= MCY;CY++)
	{
		if (Nivel->Eventos(MCX,MCY) == evPuenteFlojo ||
			Nivel->Eventos(MCX,MCY) == evPuenteCae)
		{
			ObjetoBase *NuevoPuente = new PuenteCae();
			PuenteCae  *ElPuente	= (PuenteCae *)NuevoPuente;
			ElPuente->Inicializar(MCX,MCY,(Nivel->Eventos(MCX,MCY) == evPuenteCae)?(RetardoCaidaPuente):(RetardoPuenteFlojo));
			PuentesCaen->Agregar(NuevoPuente);
			Nivel->SetEventos(MCX,MCY, evPisoRigido);
			return true;
		}
		else if (Nivel->Eventos(MCX,MCY) != evNulo)
			return true;
	}
	return false;
}

void CaeCucho::Check()
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
	if (MY > 600)
		Matarme();

	if (Tipo == caecuchoDerecha)
		ImagenCaeCucho = ImagenCaeCuchoBase + (InterrN / 4) % 5;
	else
		ImagenCaeCucho = ImagenCaeCuchoBase + (0x7FFFFFFF-InterrN / 4) % 5;

	if (Libre)
	{
		MY += VY * Delta + 0.5 * AceleracionGravedad * Delta * Delta;
		VY += AceleracionGravedad * Delta;
		MICY = MCY;
		MCY  = ((long)MY + 60) / 30;
		if (HuboChoque())
		{
			ObjetoBase *ObjetoAuxiliar;
			Secuencia  *SecuenciaAuxiliar;
			ObjetoAuxiliar    = new Secuencia();
			SecuenciaAuxiliar = (Secuencia *)ObjetoAuxiliar;
            SecuenciaAuxiliar->Inicializar((long)MX - 33, (long)MY - 18,96,96,15,5,imglstExplosiones[4]);
			Explosiones->Agregar(ObjetoAuxiliar);
			Matarme();
			return;
		}
		RECT R;
		SetRect(&R,(long)MX,(long)MY,(long)MX + 30,(long)MY + 60);
		if (ColisionaNicholas(R,imglstEnemigos[ImagenCaeCucho]))
			Hitazo = true;
	}
	else if (Tipo == caecuchoIzquierda)
		Libre = MX - Nicho->ImagenNicholasX - 60 <= DistanciaActivacionCaecucho;
	else
		Libre = Nicho->ImagenNicholasX > MX;
}
