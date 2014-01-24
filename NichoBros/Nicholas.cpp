// Este archivo define las funciones más complejas, y por lo tanto
// el comportamiento general, de la clase Nicholas.

// Incluimos librerías necesarias...

#include <iostream>
#include <cmath>
#include "RutinasSDL/rutinasSDL.h"
#include "Teleport.h"
#include "Nicholas.h"


// Maneja al personaje.
Nicholas *Nicho;

// Inicializar: función que da un valor inicial a todas las
// variables de la clase.

void Nicholas::CargarAuxSurf()
{
    {
        SDL_Surface *rval;
        rval = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCCOLORKEY,60,90,16,0,0,0,0);
        if (rval == NULL)
            std::cerr << "UH, rval = NULL en Nicholas.cpp" << std::endl;
        AuxSurf = SDL_DisplayFormat(rval);
        SDL::freeSurface(rval);
    }

    if (AuxSurf == NULL)
            std::cerr << "UH, AuxSurf = NULL en Nicholas.cpp" << std::endl;

    SDL_SetColorKey(AuxSurf,SDL_SRCCOLORKEY,0);

	RECT AuxRect;
	AuxRect.left   = 0;
	AuxRect.top    = 0;
	AuxRect.right  = 60;
	AuxRect.bottom = 90;
	
	DrawRectangle(AuxSurf,&AuxRect,RGB(0,0,0));

	BltFast(AuxSurf,0,0,imglstNicholas[Estado],&AuxRect);
}

void Nicholas::Inicializar(int CX, int CY)
{
    ImagenNicholasX		      = CX * 30;
    ImagenNicholasY			  = (CY - 2) * 30;
    Estado					  = NicholasDe;
	Super(false);
	VelSalto				  = 0;
    Cayendo					  = false;
	Trepando				  = false;
    Inmunidad                 = NoSoyInmune; //Bandera de que todo va bien
    Rebotando				  = false;
    Hitazo					  = false;
	CambiaTrepa				  = false;
	Subiendo				  = false;
	LastCiclo				  = Ciclo;
	ExactoH     = ExactoV	  = false;
	CorreccionH = CorreccionV = 0;
	AuraEscudo				  = false;
	Enganchado				  = false;
	Teleportando              = false;
	ValBot					  = 0;
	IVX						  = 0;
	IVY						  = 0;
	EngancheX                 = 0;
	AnguloAE				  = 0;
	Cansancio				  = 100;
	LineaAQuemar			  = 0;
	LCX = LCY				  = -1;
	AuxSurf					  = 0;
}



void Nicholas::Check()
{
	if (Estado == NicholasBotDe || Estado == NicholasBotIz)
	{
		RefrescarPS();
		return;
	}
	if (Teleportando)
		return;
	LINX = ImagenNicholasX;
	LINY = ImagenNicholasY;
	if (!EstoyVivo)
	{
		int AuxI;
		for (AuxI = (int)LineaAQuemar;AuxI < LineaAQuemar + Delta * VelocidadQuemarNicholas;AuxI++)
			if (AuxI < 90)
				QuemarLinea(AuxSurf,60,AuxI);
			else if (AuxI > 100)
			{
				//DWORD SoundStatus;
				//lpDSBuffers[sndEsoDolio]->GetStatus(&SoundStatus);
				if (!IsPlaying(lpDSBuffers[sndEsoDolio]))
				{
					int relojito = SDL::getTicks();
					while (TiempoDesde(relojito) < 1000);
					PlayWav(sndMarchaFunebre);
					while (IsPlaying(lpDSBuffers[sndMarchaFunebre]));
					ReintentarNivel = true;
					return;
				}
			}
		LineaAQuemar += Delta * VelocidadQuemarNicholas;
		return;
	}
    if (ImagenNicholasY > 600)
	{
        EstoyVivo = false;
		CargarAuxSurf();
		StopAllWavs();
		PlayWav(sndEsoDolio);
		return;
	}
	if (Rebotar)
		if (!Rebotando)
		{
            IVX = 0;
            IVY = 0;
			Cayendo   = false;
			Subiendo  = true;
			VelSalto  = NicholasRebote;
	        Rebotando = true;
			Rebotar   = false;
		}
	if (Colision(evZonaSalir))
		// GANAMOS!
		NivelTerminado = true;
	// A ver si estamos muertos...
	if (!AuraEscudo)
		if (Inmunidad == NoSoyInmune)
		{
			if (Hitazo || 
				Colision(evZonaDuele))
			{
			    if (SuperNicholas)
				{
					PlayWav(sndVueltaNormal);
			        Inmunidad = 0;
				}
				else
				{
					EstoyVivo = false;
					CargarAuxSurf();
					StopAllWavs();
					PlayWav(sndEsoDolio);
					return;
				}
			}
			Hitazo = false;
		}
		else
		{
			Hitazo = false;
			if (Inmunidad <= NicholasInmunidad)
			{
			    Super(Ciclo % 2 == 0);
				Inmunidad += Delta;
			}
		    else
			{
				Super(false);
				Inmunidad = NoSoyInmune;
			}
		}
	else
	{
		Hitazo = false;
		if (Inmunidad <= NicholasInmunidadAE)
			Inmunidad += Delta;
		else
		{
			Inmunidad  = NoSoyInmune;
			AuraEscudo = false;
            if (SDL::pausedMusic()) SDL::resumeMusic();
		}
	}
	if (Colision(evPremioSpeed))
	{
		Cansancio = 100;
		PlayWav(sndBebiendo);
		if (!AuraEscudo)
			Inmunidad = NoSoyInmune;
	    Super(true);
		Nivel->SetEventos(LCX,LCY,evNulo);
	    long X;
		int  Y;
		CasillaCoordenada(X, Y, LCX, LCY);
		ObjetoBase *CartelPorPremio = new Cartel();
		Cartel     *AuxCartel = (Cartel *)CartelPorPremio;
		char Cad[MAX_LONG_CARTEL];
		sprintf(Cad,"%d",PuntajeEnergizante);
		AuxCartel->Inicializar(X, Y, Cad,RGB(255,255,255));
		Carteles->Agregar(CartelPorPremio);
	    Puntaje += PuntajeEnergizante;
	}
	if (Colision(evPremioMoneda))
	{
		PlayWav(sndMoneda);
		Nivel->SetEventos(LCX,LCY,evNulo);
		long X;
		int  Y;
		CasillaCoordenada(X, Y, LCX, LCY);
		ObjetoBase *CartelPorPremio = new Cartel();
		Cartel     *AuxCartel		= (Cartel *)CartelPorPremio;
		AuxCartel->Inicializar(X, Y, MENSAJE_GANAR_MONEDA, RGB(255,255,0));
		Carteles->Agregar(CartelPorPremio);
		Monedas++;
	}
	if (Colision(evPremioBolaFuego))
	{
		PlayWav(sndBolaFuego);
		Nivel->SetEventos(LCX, LCY, evNulo);
		long X;
		int  Y;
		CasillaCoordenada(X, Y, LCX, LCY);
		ObjetoBase *CartelPorPremio = new Cartel();
		Cartel	   *AuxCartel	    = (Cartel *)CartelPorPremio;
		AuxCartel->Inicializar(X, Y, MENSAJE_GANAR_BF, RGB(255,0,0));
		Carteles->Agregar(CartelPorPremio);
		CantidadBF++;
	}
	if (Colision(evPremioEscudo))
	{
		//SoundStop(lpDSBuffers[sndAuraEscudo]);
        SDL::stopSound();
		PlayWav(sndAuraEscudo);
        if (SDL::playingMusic()) SDL::pauseMusic();
		if (Inmunidad != NoSoyInmune && !AuraEscudo)
			Super(false);
		Nivel->SetEventos(LCX, LCY, evNulo);
		Inmunidad       = 0;
		AuraEscudo      = true;
	}
	if (Colision(evPremioAureola))
	{
		PlayWav(sndAureola);
		Nivel->SetEventos(LCX, LCY, evNulo);
		long X;
		int  Y;
		CasillaCoordenada(X, Y, LCX, LCY);
		ObjetoBase *CartelPorPremio = new Cartel();
		Cartel     *AuxCartel		= (Cartel *)CartelPorPremio;
		AuxCartel->Inicializar(X, Y, MENSAJE_GANAR_VIDA, RGB(0,255,255));
		Carteles->Agregar(CartelPorPremio);
		Vidas++;
	}
	if (Colision(evPremioLaser))
	{
		PlayWav(sndLaser);
		Nivel->SetEventos(LCX, LCY, evNulo);
		long X;
		int  Y;
		CasillaCoordenada(X, Y, LCX, LCY);
		ObjetoBase *CartelPorPremio = new Cartel();
		Cartel	   *AuxCartel	    = (Cartel *)CartelPorPremio;
		AuxCartel->Inicializar(X, Y, MENSAJE_GANAR_LASER, RGB(0,0,255));
		Carteles->Agregar(CartelPorPremio);
		CantidadLaser++;
	}
	if (Colision(evPremioLiquidoVerde))
	{
		PlayWav(sndGluGluGlu);
		Nivel->SetEventos(LCX, LCY, evNulo);
		long X;
		int  Y;
		CasillaCoordenada(X, Y, LCX, LCY);
		ObjetoBase *CartelPorPremio = new Cartel();
		Cartel	   *AuxCartel	    = (Cartel *)CartelPorPremio;
		AuxCartel->Inicializar(X, Y, MENSAJE_LIQUIDO_VERDE, RGB(55,255,55));
		Carteles->Agregar(CartelPorPremio);
		TiempoLV = DuracionLiquidoVerde;
	}
	eEventos AuxEv;
	for (AuxEv = evPremioCereza; AuxEv <= evPremioRadio; AuxEv = eEventos(AuxEv + 1))//eEventos(int(AuxEv) + 1))
		if (Colision(AuxEv))
		{
			PlayWav(sndNiam);
			Nivel->SetEventos(LCX, LCY, evNulo);
			long X;
			int  Y;
			CasillaCoordenada(X, Y, LCX, LCY);
			ObjetoBase *CartelPorPremio = new Cartel();
			Cartel	   *AuxCartel	    = (Cartel *)CartelPorPremio;
			char c[10];
			sprintf(c,"%d",PuntajePremio[AuxEv - evPremioCereza]);
			Puntaje += PuntajePremio[AuxEv - evPremioCereza];
			AuxCartel->Inicializar(X, Y, c, RGB(255,255,255));
			Carteles->Agregar(CartelPorPremio);
		}
	if ((Colision(evBotRojo) || Colision(evBotRojoIn)) && Teclado[SDLK_e])
	{
		BotX = LCX;
		BotY = LCY;
		if (Nivel->Eventos(LCX - 1,LCY) == evPisoRigido) 
		{
			ImagenNicholasX = LCX * 30;
			Estado = NicholasBotIz;
		}
		else
		{
			ImagenNicholasX = (LCX - 1) * 30;
			Estado = NicholasBotDe;
		}
		RefrescarPS();
		return;
	}
	if ((Colision(evBotCyan) || Colision(evBotCyanIn)) && Teclado[SDLK_e])
	{
		BotX = LCX;
		BotY = LCY;
		if (Nivel->Eventos(LCX - 1,LCY) == evPisoRigido) 
		{
			ImagenNicholasX = LCX * 30;
			Estado = NicholasBotIz;
		}
		else
		{
			ImagenNicholasX = (LCX - 1) * 30;
			Estado = NicholasBotDe;
		}
		RefrescarPS();
		return;
	}
	if (Teclado[SDLK_e])
	{
		eTiposCol AuxCol;
		if (Trepando)
		{
			Teclado[SDLK_e] = false;
			Trepando     = false;
			if (Estado == NicholasTreDe)
				Estado = NicholasDe;
			else
				Estado = NicholasIz;
		}
		else if ((AuxCol = ColisionAlta(evEscalera)) != ColNula) // Nicholas debe agarrarse con las MANOS.
		{
			if (AuxCol == ColDerecha)
			{
				//if (((long)ImagenNicholasX) % 30 != 0)
				//	ImagenNicholasX = (((long)ImagenNicholasX) / 30 + 1) * 30;
				//else
				//	ImagenNicholasX = (long)ImagenNicholasX;
                ImagenNicholasX = (LCX-1) * 30;
				Estado = NicholasTreDe;
			}
			else
			{
				//if (((long)ImagenNicholasX) % 30 != 0)
				//	ImagenNicholasX = (((long)ImagenNicholasX) / 30) * 30;
				//else
				//	ImagenNicholasX = (long)ImagenNicholasX;
                ImagenNicholasX = LCX * 30;
				Estado = NicholasTreIz;
			}
			Teclado[SDLK_e] = false;
			Trepando     = true;
			CambiaTrepa  = false;
		}
		Teclado[SDLK_d] = false;
	}
	if (ColRebotador)
		return;
	if (Cansancio <= 5)
		Teclado[SDLK_w] = false;
	// Vemos si se ha pulsado una tecla, o si se ha dejado
	// de hacerlo, y actuamos en consecuencia.
	double VelMov;
	if (SuperNicholas)
		VelMov = (Teclado[SDLK_w])?(VelocidadNicholas):(VelocidadNicholas / 2);
	else
		VelMov = (Teclado[SDLK_w])?(VelocidadNicholas / 2):(VelocidadNicholas / 3);
	if (TiempoLV)
		VelMov *= 2;
	if (Trepando)
	{
		CambiaTrepa = true;
		if (Teclado[SDLK_UP])
			if (ColisionAlta(evEscalera))
				MoverNicholas(Arriba, VelMov * Delta);
			else
				CambiaTrepa = false;
		else if (Teclado[SDLK_DOWN])
			if (ColisionBaja(evEscalera))
				MoverNicholas(Abajo, VelMov * Delta);
			else
				CambiaTrepa = false;
		else
			CambiaTrepa = false;
	}
	else if (Teclado[SDLK_RIGHT])
	{
		if (Estado != NicholasSalDe && Estado != NicholasSalIz)
		{
			if (Cayendo)
				Estado = NicholasDe;
			else
				Estado = NicholasCaDe;
		}
		else
			if (Estado == NicholasSalIz)
				Estado = NicholasSalDe;
        if (!Teclado[SDLK_SPACE])
            MoverNicholas(Derecha, VelMov * Delta);
	}
	else if (Teclado[SDLK_LEFT])
	{
		if (Estado != NicholasSalDe && Estado != NicholasSalIz)
		{
			if (Cayendo)
				Estado = NicholasIz;
			else
				Estado = NicholasCaIz;
		}
		else
			if (Estado == NicholasSalDe)
				Estado = NicholasSalIz;
        if (!Teclado[SDLK_SPACE])
            MoverNicholas(Izquierda, VelMov * Delta);
	}
	else if (Estado == NicholasCaDe)
		Estado = NicholasDe;
	else if (Estado == NicholasCaIz)
		Estado = NicholasIz;
	if (!Rebotando)
    {
		if (Teclado[SDLK_d])
		{
			if (Estado == NicholasCaDe  ||
				Estado == NicholasDe    || 
				Estado == NicholasSalDe ||
				Estado == NicholasTreDe)
				Estado = NicholasSalDe;
			else
		        Estado = NicholasSalIz;
			if ((!Cayendo && VelSalto < 0) || Trepando || Enganchado)
			{
				PlayWav(sndChamelotCorto);
				Trepando   = false;
				Enganchado = false;
				VelSalto   = NicholasPotenciaSalto;
				Subiendo   = true;
			}
		}
		else
		{
			if (Subiendo)
				VelSalto = 0;
			Cayendo   = true;
			Rebotando = false;
		}
    }
	if (!Trepando && Teclado[SDLK_s] && CantidadBF > 0)
	{
		if (!Disparando)
		{
			PlayWav(sndLanzarBF);
			Disparando = true;
			CantidadBF--;
			ObjetoBase *NuevaBolaFuego = new BolaFuego();
			BolaFuego  *AuxBolaFuego   = (BolaFuego *)NuevaBolaFuego;
			double BFX,BFY;
			double BFVX, BFVY;
			BFY  = ImagenNicholasY + 40;
			BFVY = VelocidadBolaFuego * sin(AnguloBolaFuego);
			if (Estado == NicholasDe   ||
				Estado == NicholasCaDe ||
				Estado == NicholasSalDe)
			{
				BFX  = ImagenNicholasX + 60;
				BFVX = VelocidadBolaFuego * cos(AnguloBolaFuego);
			}
			else
			{
				BFX  = ImagenNicholasX;
				BFVX = -VelocidadBolaFuego * cos(AnguloBolaFuego);
			}
			AuxBolaFuego->Inicializar(BFX,BFY,BFVX,BFVY,false);
			BolasFuego->Agregar(NuevaBolaFuego);
		}
	}
	else
		Disparando = false;
	// LASERES
	if (!Trepando && Teclado[SDLK_a] && CantidadLaser > 0)
	{
		if (!DisparandoL)
		{
			PlayWav(sndSufuo);
			DisparandoL = true;
			CantidadLaser--;
			ObjetoBase *NuevoRayo = new Rayo();
			Rayo	   *AuxRayo   = (Rayo *)NuevoRayo;
			double RayoX,RayoY;
			double RayoVX;
			RayoX  = (Estado == NicholasDe ||
				      Estado == NicholasSalDe ||
					  Estado == NicholasCaDe)?(ImagenNicholasX + 55):(ImagenNicholasX - 20);
			RayoY  = ImagenNicholasY + 25 + rand() % 20;
			RayoVX = (Estado == NicholasDe ||
				      Estado == NicholasSalDe ||
					  Estado == NicholasCaDe)?(VelocidadRayoEnergia / (rand() % 4 + 1)):(-VelocidadRayoEnergia / (rand() % 4 + 1));
			AuxRayo->Inicializar(RayoX,RayoY,RayoVX,false);
			Rayos->Agregar(NuevoRayo);
		}
	}
	else
		DisparandoL = false;
	// GRAVEDAD!
	if (Trepando || Enganchado)
	{
		VelSalto = 0;
		Cayendo = false;
	}
	else 
	{
		if (VelSalto - AceleracionGravedad * Delta < 0 && Subiendo)
		{
			Subiendo   = false;
			Cayendo    = true;
			double DeltaE0 = VelSalto / AceleracionGravedad;
			MoverNicholas(Arriba, VelSalto * DeltaE0 - DeltaE0 * DeltaE0 * AceleracionGravedad / 2);
			//if ((long)ImagenNicholasY % 30 < ToleranciaYMaximo)
			//	ImagenNicholasY = (long)ImagenNicholasY - (long)ImagenNicholasY % 30;
		}
		else if (VelSalto > 0)
			MoverNicholas(Arriba, VelSalto * Delta - Delta * Delta * AceleracionGravedad / 2);
		else
			MoverNicholas(Abajo, - VelSalto * Delta + Delta * Delta * AceleracionGravedad / 2);
		VelSalto -= AceleracionGravedad * Delta;
	}
	// Inercia.
	if (!Trepando && !Enganchado)
	{
		MoverNicholas(Derecha,IVX * Delta);
		MoverNicholas(Arriba ,IVY * Delta);
	}
	// Cansancio
	if (!TiempoLV) // Si hay liquido verde, no importa el cansancio!
	{
		if (Estado == NicholasDe ||
			Estado == NicholasIz)
			Cansancio += (SuperNicholas?TasaRecuperacionSuper:TasaRecuperacionNormal) * Delta * 2;
		else if (Teclado[SDLK_w])
			Cansancio -= (SuperNicholas?TasaCansancioSuper:TasaCansancioNormal) * Delta;
		else
			Cansancio += (SuperNicholas?TasaRecuperacionSuper:TasaRecuperacionNormal) * Delta / 4;
		if (Cansancio < 0)
			Cansancio = 0;
		else if (Cansancio > 100)
			Cansancio = 100;
	}
	else
		Cansancio = 100;
	// CAMARA MOVIL!
	if (ImagenNicholasX >= Pos + 800 - FranjaCambio)
		MoverCamara((int)ImagenNicholasX - (Pos + 800 - FranjaCambio));
	if (ImagenNicholasX < Pos)
		ImagenNicholasX = Pos;
}

void Nicholas::RefrescarPS()
{
	int AumentoSuper, ExpInterr;
	AumentoSuper = (SuperNicholas)?(50):(0);
	ExpInterr	 = (Teclado[SDLK_w])?(InterrN):(InterrN / 2);
	imglstNicholas[NicholasDe   ] = imglstNicholasAni[0  + AumentoSuper];
	imglstNicholas[NicholasIz   ] = imglstNicholasAni[1  + AumentoSuper];
	imglstNicholas[NicholasCaDe ] = imglstNicholasAni[2  + ExpInterr % 14 + AumentoSuper];
	imglstNicholas[NicholasCaIz ] = imglstNicholasAni[16 + ExpInterr % 14 + AumentoSuper];
	imglstNicholas[NicholasSalDe] = imglstNicholasAni[30 + AumentoSuper];
	imglstNicholas[NicholasSalIz] = imglstNicholasAni[31 + AumentoSuper];
	if (!CambiaTrepa)
		ExpInterr = 0;
	imglstNicholas[NicholasTreDe] = imglstNicholasAni[32 + ExpInterr % 6  + AumentoSuper];
	imglstNicholas[NicholasTreIz] = imglstNicholasAni[38 + ExpInterr % 6  + AumentoSuper];

	static int Incre = 1;

	if (Estado == NicholasBotDe || Estado == NicholasBotIz)
	{
		imglstNicholas[NicholasBotDe] = imglstNicholasAni[44 + ValBot + AumentoSuper];
		imglstNicholas[NicholasBotIz] = imglstNicholasAni[47 + ValBot + AumentoSuper];
		if (Ciclo == 0)
		{
			ValBot += Incre;
			if (ValBot >= 2)
			{
				Incre = -1;
				switch (Nivel->Eventos(BotX,BotY))
				{
					case evBotRojo:
						Nivel->SetEventos(BotX,BotY,evBotRojoIn);
						ApretadoBotR = !ApretadoBotR;
						break;
					case evBotRojoIn:
						Nivel->SetEventos(BotX,BotY,evBotRojo);
						ApretadoBotR = !ApretadoBotR;
						break;
					case evBotCyan:
						Nivel->SetEventos(BotX,BotY,evBotCyanIn);
						ApretadoBotC = !ApretadoBotC;
						break;
					case evBotCyanIn:
						Nivel->SetEventos(BotX,BotY,evBotCyan);
						ApretadoBotC = !ApretadoBotC;
						break;
                    default:
                        break;
				}
			}
			if (ValBot <= 0)
			{
				Incre  = 1;
				Estado = (Estado == NicholasBotDe)?NicholasDe:NicholasIz;

			}
		}
	}
}

bool Nicholas::MoverNicholas(eDireccion Direccion, double Velocidad,bool Rompe)
{
	if (Velocidad == 0)	return true;
    if (Velocidad < 0 ) return MoverNicholas(dirInv(Direccion),-Velocidad,Rompe);
	while (Velocidad > MaximoMovimiento)
	{
		// El objetivo de esto es descomponer movimientos muy
		// grandes en varios más chicos. En efecto, es necesario
		// hacerlo. Pensemos en el dibujo:
		//			    #|
		// Nicholas es el # y el | es un bloque. Si nuestro
		// movimiento es de 3 casillas, la posicion final será
		//				 | #
		// que es perfectamente válida. Conclusión: debemos evitar
		// movimientos muy grandes.
		// Ah, me olvidava. Para simplificar el código, utilizamos
		// la magia de la recursividad. :-)-)-)
		Velocidad -= MaximoMovimiento;
		if (!MoverNicholas(Direccion,MaximoMovimiento))
			return false;
		// Ultimo detalle: si alguno de los sub-movimientos falla,
		// devolvemos falso. Esto gana velocidad, ya que no siempre
		// es necesario chequear todos los movimientos.
	}
    double LlegadaX, LlegadaY;
    switch (Direccion)
	{
		case (Derecha):
			if (Enganchado)
			//{
				EngancheX += Velocidad;
			//	return true;
			//}
            LlegadaX = ImagenNicholasX + Velocidad;
            LlegadaY = ImagenNicholasY;
			break;
		case (Izquierda):
			if (Enganchado)
			//{
				EngancheX -= Velocidad;
			//	return true;
			//}
            LlegadaX = ImagenNicholasX - Velocidad;
            if (LlegadaX < Pos)
                return false;
            LlegadaY = ImagenNicholasY;
			break;
		case (Abajo):
			//if (Enganchado)
            
			//	return true;
            LlegadaX = ImagenNicholasX;
            LlegadaY = ImagenNicholasY + Velocidad;
			break;
		case (Arriba):
			//if (Enganchado)
			//	return true;
            LlegadaX = ImagenNicholasX;
            LlegadaY = ImagenNicholasY - Velocidad;
			break;
        default:
            break;
    }
    // Ahora, solo tenemos que chequear si la posicion
    // marcada es valida.
    int X , Y;
    int X1, Y1;
    int X2, Y2;
    bool Sirve, Funca, PlayChoqueTecho;
	eEventos Valor;
    CoordenadaCasilla((long)LlegadaX, (int)LlegadaY, X1, Y1);
    CoordenadaCasilla((long)LlegadaX + 59, (int)LlegadaY + 89, X2, Y2);
    Sirve = PlayChoqueTecho = true;
    for (X = X1; X <= X2; X++)
        for (Y = Y1; Y <= Y2; Y++)
            {
                if (Teleporter::casillasConTeleporters.find(std::make_pair(X,Y)) != 
                    Teleporter::casillasConTeleporters.end())
                {
                    Sirve = false;
                    if (Teclado[SDLK_e] && !AuraEscudo)
                    {
                        // No se puede teletransportar mientras dura el aura escudo: simplemente
                        // ambas cosas no son compatibles.
                        Teleportando = true;
                        ATP          = true;
                        ATPX         = X;
                        ATPY         = Y;
                    }
                }
                else if ((Valor = Nivel->Eventos(X,Y)) == evPisoFragil || 
					 Valor == evBloqueMoneda)
				{
                    if (Direccion == Arriba && Rompe)
					{
                        if (Estado == NicholasSalDe)
                            Funca = X == X2;
                        else if (Estado == NicholasSalIz)
                            Funca = X == X1;
                        else
                            Funca = false;
                        if (Funca)
						{
							ObjetoBase *Cartelito;
							ObjetoBase *LaExplosion;
							ObjetoBase *NuevaMoneda;
							Cartel	   *AuxCartel;
							Explosion  *AuxExplosion;
							Moneda	   *LaMoneda;
							long	   RX;
							int		   RY;
							switch (Valor)
							{
								case evPisoFragil:
									PlayWav(sndRupturaBloque);
									Nivel->SetEventos(X, Y, evExplotando);
									LCX = X; 
									LCY = Y;
									// Modificamos los valores del nivel.
									Nivel->SetTerreno(X, Y, caVacia);
									// Creamos una explosion.
									LaExplosion  = new Explosion();
									AuxExplosion = (Explosion *)LaExplosion;
									AuxExplosion->Inicializar(X, Y);
									Explosiones->Agregar(LaExplosion);
									// Creamos un cartel con el puntaje.
									char	   Cad[MAX_LONG_CARTEL];
									CasillaCoordenada(RX,RY,X,Y);
									Cartelito = new Cartel();
									AuxCartel = (Cartel *)Cartelito;
									Puntaje += PuntajeRomperCasilla;
									sprintf(Cad,"%d",PuntajeRomperCasilla);
									AuxCartel->Inicializar(RX, RY, Cad, RGB(255,255,255));
									Carteles->Agregar(Cartelito);
									break;
								case evBloqueMoneda:
									Nivel->SetEventos(X,Y,evPisoRigido);
									Nivel->SetTerreno(X,Y,caBloqueAnulado);
									CasillaCoordenada(RX,RY,X,Y);
									NuevaMoneda = new Moneda();
									LaMoneda	= (Moneda *)NuevaMoneda;
									LaMoneda->Inicializar(RX,RY);
									ColecMonedas->Agregar(NuevaMoneda);
									Monedas++;
									break;
                                default:
                                    break;
							}
							VelSalto = 0;
							Subiendo = false;
							Cayendo  = true;
							PlayChoqueTecho = false;
						}
					}
                    Sirve = false;
				}
				else if (Nivel->Eventos(X,Y) == evCandado)
				{
					Sirve         = false;
					CandadoX      = X;
					CandadoY      = Y;
					
					if (HayCandados)
					{
						HayCandados = false;
						// Copia de las variables históricas.
						VidasG		   = Vidas;
						PuntajeG	   = Puntaje;
						MonedasG	   = Monedas;
						CantidadBFG	   = CantidadBF;
						CantidadLaserG = CantidadLaser;
					}
					CerrarCandado = true;
				}
                else if (Nivel->Eventos(X,Y) == evPisoRigido      ||
						 Nivel->Eventos(X,Y) == evZonaDueleRigido ||
						 Nivel->Eventos(X,Y) == evPinchePalo      ||

						((Nivel->Eventos(X,Y) == evSoloParriba ||
						  Nivel->Eventos(X,Y) == evPlatCae)&& 
						 Direccion == Abajo && !(
						 Y * 30 > ImagenNicholasY - 30 &&
						 Y * 30 < ImagenNicholasY + 89)))
				{
					Sirve = false;
					if (Nivel->Eventos(X,Y) == evZonaDueleRigido && Direccion == Abajo && ColNicholasCasilla(X,Y))
						Hitazo = true;
					else if (Nivel->Eventos(X,Y) == evPlatCae)
					{
						APC  = true;
						APCX = X;
						APCY = Y;
					}
				}
				else if (Nivel->Eventos(X,Y) == evPuenteCae  ||
						 Nivel->Eventos(X,Y) == evPuenteFlojo)
				{
					Sirve = false;
					if (Direccion == Abajo && (Nivel->Eventos(X,Y) == evPuenteCae || Teclado[SDLK_w] || fabs(VelSalto) > TopeSaltoFlojo))
					{
						ObjetoBase *NuevoPuente = new PuenteCae();
						PuenteCae  *ElPuente	= (PuenteCae *)NuevoPuente;
						ElPuente->Inicializar(X,Y,(Nivel->Eventos(X,Y) == evPuenteCae)?(RetardoCaidaPuente):(RetardoPuenteFlojo));
						PuentesCaen->Agregar(NuevoPuente);
						Nivel->SetEventos(X,Y, evPisoRigido);
					}
				}
				else if (Nivel->Eventos(X,Y) == evRebotadorSimple)
                {
					if (Direccion == Abajo)
					{
						ColRebotador = true;
						ColRebArr    = false;
						RebX		 = X;
						RebY		 = Y;
						PlayWav(sndResorte);
						return true;
					}
					else
						Sirve = false;
                }
            }

	// Listo el for! A trabajar!
    if (Sirve)
	{
		// Chequeamos valores para ExactoH y ExactoV.
		double A,B;
		switch (Direccion)
		{
			case Derecha:
			case Izquierda:
				ExactoV = false;
				if (ImagenNicholasX > LlegadaX)
				{
					A = LlegadaX;
					B = ImagenNicholasX;
				}
				else
				{
					A = ImagenNicholasX;
					B = LlegadaX;
				}
				ExactoH = (CorreccionH = ((long)(A / 30) + 1) * 30) < B;
				break;
			case Arriba:
			case Abajo:
				ExactoH	   = false;
				if (ImagenNicholasY > LlegadaY)
				{
					A = LlegadaY;
					B = ImagenNicholasY;
				}
				else
				{
					A = ImagenNicholasY;
					B = LlegadaY;
				}
				ExactoV = (CorreccionV = ((long)(A / 30) + 1) * 30) < B;
				break;
            default:
                break;
		}
		// Actualizamos la posicion de Nicholas.
        ImagenNicholasX = LlegadaX;
        ImagenNicholasY = LlegadaY;
		// Si caemos, seteamos la variable consecuentemente.
        if (Direccion == Abajo && !Enganchado)
		{
            Cayendo   = true;
			Rebotando = false;
		}
	}
    else // No sirve
	{
		double ViejoValor;
		switch (Direccion)
		{
			case Arriba:
				int AuxResto;
				if (abs((AuxResto = (long)ImagenNicholasX % 30) - 15) >= 15 - ToleranciaXSalto && AuxResto != 0)
				{
					ViejoValor      = ImagenNicholasX;
					ImagenNicholasX = (AuxResto < 15)?
						((long)ImagenNicholasX - AuxResto):
						((long)ImagenNicholasX + 30 - AuxResto);
					if (!MoverNicholas(Direccion,Velocidad,false))
						ImagenNicholasX = ViejoValor;
					else
						return true;
				}
				// No ponemos break: se ejecutan ambos segmentos
				// de codigo si case Arriba.
			case Abajo:
				if (ExactoH)
				{
					ExactoH = ExactoV = false;
					ViejoValor        = ImagenNicholasX;
					ImagenNicholasX   = CorreccionH;
					if (!MoverNicholas(Direccion,Velocidad,false))
						ImagenNicholasX = ViejoValor;
					else
						return true;
				}
				else
					ExactoH = ExactoV = false;
				break;
			case Izquierda:
			case Derecha:
				if (ExactoV)
				{
					ExactoH = ExactoV = false;
					ViejoValor		  = ImagenNicholasY;
					ImagenNicholasY   = CorreccionV;
					if (!MoverNicholas(Direccion,Velocidad,false))
						ImagenNicholasY = ViejoValor;
					else
						return true;
				}
				else
					ExactoH = ExactoV = false;
				break;
            default:
                break;
		}
        switch (Direccion)
		{
			case (Abajo):
                Multiplicador = 1;
				VelSalto	  = 0;
                if (Estado == NicholasSalIz)
                    Estado = NicholasIz;
                else if (Estado == NicholasSalDe)
                    Estado = NicholasDe;
                if ((long)ImagenNicholasY % 30 != 0)
                {
                    if (ImagenNicholasY >= 0)
                        ImagenNicholasY += 30 - (long)ImagenNicholasY % 30;
                    else
                        ImagenNicholasY = - (abs((long)ImagenNicholasY) / 30) * 30;
                }
				IVX       = 0;
				IVY       = 0;
                Cayendo   = false;
				Rebotando = false;
                Teclado[SDLK_d] = false;
				break;
            case (Arriba):
				if (PlayChoqueTecho)
				{
					SoundStop(lpDSBuffers[sndChamelotCorto]);
					PlayWav(sndChoqueTecho);
				}
                if ((long)ImagenNicholasY % 30)
                {
                    if (ImagenNicholasY >= 0)
                        ImagenNicholasY -= (long)ImagenNicholasY % 30;
                    else
                        ImagenNicholasY -= 30 - (long)ImagenNicholasY % 30;
                }
				IVY       = 0;
                VelSalto  = 0;
				Subiendo  = false;
                Cayendo   = true;
				Rebotando = false;
				break;
			case (Derecha):
				Estado = NicholasDe;
				if ((long)ImagenNicholasX % 30 != 0)
                {
			        if (ImagenNicholasX >= 0)
		                ImagenNicholasX += 30 - (long)ImagenNicholasX % 30;
	                else
                        ImagenNicholasX = - (abs((long)ImagenNicholasX) / 30) * 30;
                }
				IVX       = 0;
				break;
			case (Izquierda):
				Estado = NicholasIz;
				if ((long)ImagenNicholasX % 30)
                {
			        if (ImagenNicholasX >= 0)
		                ImagenNicholasX -= (long)ImagenNicholasX % 30;
	                else
                        ImagenNicholasX -= 30 - (long)ImagenNicholasX % 30;
                }
				IVX       = 0;
				break;
            default:
                break;
		}
	}
	return Sirve;
}

bool Nicholas::Colision(eEventos QueEvento)
{
	int X , Y;
    int X1, Y1;
    int X2, Y2;
    CoordenadaCasilla((long)ImagenNicholasX, (int)ImagenNicholasY, X1, Y1);
    CoordenadaCasilla((long)ImagenNicholasX + 59, (int)ImagenNicholasY + 89, X2, Y2);
    for (X = X1; X <= X2; X++)
        for (Y = Y1; Y <= Y2; Y++)
            if (Y >= 0 && Y <= 19 && X < Ancho)
				if (Nivel->Eventos(X,Y) == QueEvento)
				{
					LCX = X;
                    LCY = Y;
                    return true;
				}
    return false;
}

eTiposCol Nicholas::ColisionAlta(eEventos QueEvento)
{
	int X, Y;
	CoordenadaCasilla((long)ImagenNicholasX + 15, (int)ImagenNicholasY + 30, X, Y);
	if (Nivel->Eventos(X,Y) == QueEvento)
    {
        LCX = X;
        LCY = Y;
		return ColIzquierda;
    }
	CoordenadaCasilla((long)ImagenNicholasX + 45, (int)ImagenNicholasY + 30, X, Y);
	if (Nivel->Eventos(X,Y) == QueEvento)
    {
        LCX = X;
        LCY = Y;
		return ColDerecha;
    }
	return ColNula;
}
	
eTiposCol Nicholas::ColisionBaja(eEventos QueEvento)
{
	int X, Y;
	CoordenadaCasilla((long)ImagenNicholasX + 15, (int)ImagenNicholasY + 90, X, Y);
	if (Nivel->Eventos(X,Y) == QueEvento)
    {
        LCX = X;
        LCY = Y;
		return ColIzquierda;
    }
	CoordenadaCasilla((long)ImagenNicholasX + 45, (int)ImagenNicholasY + 90, X, Y);
	if (Nivel->Eventos(X,Y) == QueEvento)
    {
        LCX = X;
        LCY = Y;
		return ColDerecha;
    }
	return ColNula;
}
