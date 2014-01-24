// En este archivo implementamos las funciones más complejas de la
// clase Teleporter, definiendo así su comportamiento.

// Incluimos librerías.

#include "RutinasSDL/rutinasSDL.h"
#include "Teleport.h"

// Funciones de la clase.

std::set<std::pair<int,int> > Teleporter::casillasConTeleporters;

void Teleporter::Inicializar(int X, int Y)
{
	MCX = X - 1;
	MCY = Y;
    CasillaCoordenada(MX,MY,MCX,MCY);
	Nivel->SetEventos(MCX    ,Y,evPisoRigido);
	Nivel->SetEventos(MCX + 1,Y,evPisoRigido);
	Nivel->SetEventos(MCX + 2,Y,evPisoRigido);
	Nivel->SetEventos(MCX + 3,Y,evPisoRigido);
    casillasConTeleporters.insert(std::make_pair(MCX+1,Y));
    casillasConTeleporters.insert(std::make_pair(MCX+2,Y));
	Activo    = false;
	Teleporto = false;
	Recibo    = false;
	Compa     = NULL;
}

void Teleporter::Check()
{
	if (MX - Pos < - 400)
	{
        Matarme();
        return;
    }
    if (!Activo) Activo = true;

	if (Teleporto)
	{
		NH -= VelocidadTeleport * Delta;
		if (NH < 0)
		{
			NH = 0;
			Teleporto = false;
			DescargarSuperficie(TeleImage);
		}
	}
	else if (Recibo)
	{
		NH += VelocidadTeleport * Delta;
		if (NH > 90)
		{
			NH = 90;
			Recibo = false;

			Hitazo = false; // Por las dudas...
			Nicho->Teleportando = false;
			Nicho->ImagenNicholasX = MX + 30;
			Nicho->ImagenNicholasY = MY - 90;
		}
	}
	else if (ATP && ATPY == MCY && ATPX >= MCX && ATPX <= MCX + 3 && Compa != NULL)
	{
		NH            = 90;
		Teleporto     = true;
		Compa->Recibir();
		ATP           = false;
        
		// Y ahora preparamos la imagen!
        /*
		DDSURFACEDESC ddsd;
		ZeroMemory(&ddsd,sizeof(ddsd));
		ddsd.dwSize   = sizeof(ddsd);
		ddsd.dwHeight = 90;
		ddsd.dwWidth  = 60;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		ddsd.dwFlags  = DDSD_HEIGHT | DDSD_WIDTH | DDSD_CAPS;
		lpDD->CreateSurface(&ddsd,&TeleImage,NULL);
	
		DDCOLORKEY AuxCC;
		AuxCC.dwColorSpaceLowValue =
		AuxCC.dwColorSpaceHighValue = 0;
		TeleImage->SetColorKey(DDCKEY_SRCBLT,&AuxCC);
		*/
        
        if (TeleImage != NULL)
        {
            SDL::freeSurface(TeleImage);
            TeleImage = NULL;
        }
        {
            SDL_Surface *rval;
            rval = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCCOLORKEY,60,90,16,0,0,0,0);
            TeleImage = SDL_DisplayFormat(rval);
            SDL::freeSurface(rval);
        }

        SDL_SetColorKey(TeleImage,SDL_SRCCOLORKEY,0);
        
		WhiteLine(imglstNicholas[Nicho->Estado],TeleImage,60,0);
		for (int i = 1;i < 89;i++)
			WhiteExtremes(imglstNicholas[Nicho->Estado],TeleImage,60,i);
		WhiteLine(imglstNicholas[Nicho->Estado],TeleImage,60,89);
		
	}
}
