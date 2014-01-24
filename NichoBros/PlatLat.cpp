// En este archivo implementamos la clase PlatLat, definida en el
// archivo de cabecera PlatLat.h

// Inclusion de librerias.

#include "RutinasSDL/rutinasSDL.h"
#include "PlatLat.h"

// Funciones de la clase

void PlatLat::Inicializar(int CX, int CY, bool horiz)
{
	long AuxX;
	int  AuxY;
	CasillaCoordenada(AuxX,AuxY,CX,CY);
	MX			   = AuxX;
	MY			   = AuxY;

	MCB = horiz?MX:MY;

	Activo         = false;
	Engancho       = false;
	Tiempo		   = 0;
	Horiz          = horiz;
    ultTiempoEnganche = -1000;
}

void PlatLat::Check()
{
	if (MX + AmplitudPlataformaLateral - Pos < - 400)
	{
        Matarme();
        return;
    }
    if (!Activo)
    {
        if (MX - AmplitudPlataformaLateral - Pos < 800)
            Activo = true;
        else
            return;
    }
	Tiempo += Delta;
	if (Horiz)
		MX = MCB + AmplitudPlataformaLateral * sin((2 * Pi / PeriodoPlataformaLateral) * Tiempo);
	else
		MY = MCB + AmplitudPlataformaVertical * sin((2 * Pi / PeriodoPlataformaVertical) * Tiempo);

	if (Nicho->Enganchado && Engancho)
	{
		if (Horiz)
		{
			Nicho->IVX = (2 * Pi * AmplitudPlataformaLateral / PeriodoPlataformaLateral) *
				          cos((2 * Pi / PeriodoPlataformaLateral) * Tiempo);
			Nicho->IVY = 0;
		}
		else
		{
			Nicho->IVX = 0;
			Nicho->IVY = -(2 * Pi * AmplitudPlataformaVertical / PeriodoPlataformaVertical) *
		       	           cos((2 * Pi / PeriodoPlataformaVertical) * Tiempo);
		}
        double nx = MX + Nicho->EngancheX;
        double ny = MY - 90;
        eDireccion hDir = ((nx > Nicho->ImagenNicholasX)?Derecha:Izquierda);
        eDireccion vDir = ((ny > Nicho->ImagenNicholasY)?Abajo:Arriba);
        double enganchito = Nicho->EngancheX;
        if (!
           (Nicho->MoverNicholas(hDir, fabs(nx - Nicho->ImagenNicholasX),false) &&
            Nicho->MoverNicholas(vDir, fabs(ny - Nicho->ImagenNicholasY),false))
           )
        {
            Nicho->Enganchado = false;
            Engancho = false;
            ultTiempoEnganche = Tiempo;
        }
        else
            Nicho->EngancheX = enganchito;
		//Nicho->ImagenNicholasX = MX + Nicho->EngancheX;
		//Nicho->ImagenNicholasY = MY - 90;
		if (Nicho->EngancheX < -60 || Nicho->EngancheX > 90)
        {
			Nicho->Enganchado = false;
            Engancho = false;
            ultTiempoEnganche = Tiempo;
        }
	}
	else
	{
        if (Engancho)
        {
            Engancho = false;
            ultTiempoEnganche = Tiempo;
        }
		RECT R;
		SetRect(&R,(long)MX     ,(long)MY,
			       (long)MX + 90,(long)MY + 8);
		if (!(Nicho->Trepando || Nicho->Enganchado) && Nicho->LINY + 79 <= MY &&
			ColisionaNicholas(R,imglstPlataformas[Horiz?platCyan:platVerde]) != ColNula &&
            Tiempo - ultTiempoEnganche >= 200)
		{
            Teclado[SDLK_d] = false;
			Engancho = true;

			Nicho->Cayendo	 = false;
			Nicho->Rebotando = false;
			Nicho->Subiendo	 = false;

			if (Nicho->Estado == NicholasDe    ||
				Nicho->Estado == NicholasCaDe  ||
				Nicho->Estado == NicholasSalDe)
				Nicho->Estado = NicholasDe;
			else
				Nicho->Estado = NicholasIz;

			Nicho->Enganchado      = true;
			Nicho->ImagenNicholasY = MY - 90;
			Nicho->EngancheX       = Nicho->ImagenNicholasX - MX;
            if (Nicho->EngancheX < -60) Nicho->EngancheX = -60;
            if (Nicho->EngancheX >  90) Nicho->EngancheX = 90;
            Nicho->ImagenNicholasX = MX + Nicho->EngancheX;
		}
	}
}
