// En este archivo de implementan las funciones más complejas de
// la clase Sauriolo.

// Incluimos las librerías necesarias.

#include "RutinasSDL/rutinasSDL.h"
#include "Sauriolo.h"

// Implementamos las funciones.

void Sauriolo::Inicializar(int CX, int CY)
{
    ImagenSaurioloPicture = SaurioloIz1;
	Sentido               = Izquierda;
    long X;
	int  Y;
    CasillaCoordenada(X, Y, CX, CY);
    MY = Y - 30;
    MX = X;
    EstoyMuerto	   = false;
    Activo		   = false;
	VY			   = 0;
	Ahora		   = true;
	ExactoH		   = false;
	ExactoV		   = false;
	Subiendo       = true;
	CorreccionH    = 0;
	CorreccionV    = 0;

	switch (Dificultad)
	{
		case inQuesoRemachado:
			Salud = SaurioloGolpesSoportables - 2;
			break;
		case inNewby:
			Salud = SaurioloGolpesSoportables - 1;
			break;
		case inJuegueroDecente:
			Salud = SaurioloGolpesSoportables;
			break;
		case inCyberAdicto:
			Salud = SaurioloGolpesSoportables + 2;
			break;
		default:
			Salud = SaurioloGolpesSoportables;
			break;
	}
}

void Sauriolo::Check()
{
    if (MY > 600 || MX - Pos < -400)
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
    if (EstoyMuerto)
	{
        if (Sentido == Derecha)
            MX += 0.1 * Delta;
        else
            MY -= 0.1 * Delta;
        MY += VY * Delta + Delta * Delta * AceleracionGravedad / 2;
        VY += AceleracionGravedad * Delta;
        return;
    }
	if (fabs(Nicho->ImagenNicholasX - MX) > 30)
	{
		if (Sentido == Izquierda)
		{
			if (Nicho->ImagenNicholasX > MX)
			{
				Sentido = Derecha;
				ImagenSaurioloPicture = SaurioloDe1;
				Subiendo = true;
			}
		}
		else
		{
			if (Nicho->ImagenNicholasX < MX)
			{
				Sentido = Izquierda;
				ImagenSaurioloPicture = SaurioloIz1;
				Subiendo = true;
			}
		}
	}
	if (fabs(Nicho->ImagenNicholasY - MY) < 60 && Ciclo == 0 && rand() % ValorBolas == 0)
	{
		PlayWav(sndLanzarBF);
		ObjetoBase *NuevaBolaFuego = new BolaFuego();
		BolaFuego  *AuxBolaFuego   = (BolaFuego *)NuevaBolaFuego;
		double BFX,BFY;
		double BFVX, BFVY;
		BFY  = MY + 2;
		BFVY = VelocidadBolaFuego * sin(AnguloBolaFuego);
		if (Sentido == Derecha)
		{
			BFX  = MX + 55;
			BFVX = VelocidadBolaFuego * cos(AnguloBolaFuego);
		}
		else
		{
			BFX  = MX + 5;
			BFVX = -VelocidadBolaFuego * cos(AnguloBolaFuego);
		}
		AuxBolaFuego->Inicializar(BFX,BFY,BFVX,BFVY,true);
		BolasFuego->Agregar(NuevaBolaFuego);
	}
    if (RevisarMuerte())
	{
        return;
    }
    if (Ciclo == 0)
	{
		if (Ahora)
		{
			switch (ImagenSaurioloPicture)
			{
				case SaurioloDe1:
					Subiendo = true;
					ImagenSaurioloPicture = SaurioloDe2;
					break;
				case SaurioloDe3:
					Subiendo = false;
					ImagenSaurioloPicture = SaurioloDe2;
					break;
				case SaurioloIz1:
					Subiendo = true;
					ImagenSaurioloPicture = SaurioloIz2;
					break;
				case SaurioloIz3:
					Subiendo = false;
					ImagenSaurioloPicture = SaurioloIz2;
					break;
				case SaurioloDe2:
					ImagenSaurioloPicture = Subiendo?SaurioloDe3:SaurioloDe1;
					break;
				case SaurioloIz2:
					ImagenSaurioloPicture = Subiendo?SaurioloIz3:SaurioloIz1;
					break;
                default:
                    break;
			}
			Ahora = false;
		}
	}
	else
		Ahora = true;
	MoverSauriolo(Sentido, VelocidadSauriolo * Delta);
    if (EstoyMuerto)
        return;
    MoverSauriolo(Abajo, VY * Delta + Delta * Delta * AceleracionGravedad / 2);
	VY += AceleracionGravedad * Delta;
}

bool Sauriolo::MoverSauriolo(eDireccion Direccion, double Velocidad)
{
	if (Direccion == Quieto)
		return true;
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
		if (!MoverSauriolo(Direccion,MaximoMovimiento))
			return false;
		// Ultimo detalle: si alguno de los sub-movimientos falla,
		// devolvemos falso. Esto gana velocidad, ya que no siempre
		// es necesario chequear todos los movimientos.
	}
    double LlegadaX, LlegadaY;
    switch (Direccion)
	{
		case Derecha:
		    LlegadaX = MX + Velocidad;
			LlegadaY = MY;
			break;
		case Izquierda:
			LlegadaX = MX - Velocidad;
			LlegadaY = MY;
			break;
		case Abajo:
			LlegadaX = MX;
			LlegadaY = MY + Velocidad;
			break;
		case Arriba:
			LlegadaX = MX;
			LlegadaY = MY - Velocidad;
			break;
        default:
            break;
	}
    // Ahora, solo tenemos que chequear si la posicion
    // marcada es valida.
    int X , Y;
    int X1, Y1;
    int X2, Y2;
    bool Sirve;
    CoordenadaCasilla((long)LlegadaX, (int)LlegadaY, X1, Y1);
    CoordenadaCasilla((long)LlegadaX + 59, (int)LlegadaY + 59, X2, Y2);
    Sirve = true;
    for (X = X1; X <= X2; X++)
        for (Y = Y1; Y <= Y2; Y++)
          {
            if (Nivel->Eventos(X, Y) == evPisoFragil || Nivel->Eventos(X, Y) == evPisoRigido || 
				Nivel->Eventos(X,Y) == evPinchePalo ||
				Nivel->Eventos(X,Y) == evPuenteFlojo  ||
				Nivel->Eventos(X,Y) == evBloqueMoneda ||
			   (Nivel->Eventos(X,Y) == evSoloParriba && 
			    Direccion == Abajo &&
				!(Y * 30 > MY - 30 &&
				  Y * 30 < MY + 60)))
			{
				Sirve = false;
			}
			else if (Nivel->Eventos(X, Y) == evZonaDueleRigido)
			{
				PlayWav(sndCaida);
                Puntaje += PuntajeMuerteSauriolo * Multiplicador;
				ObjetoBase *CartelPuntaje = new Cartel();
				Cartel	   *AuxCartel	  = (Cartel * )CartelPuntaje;
				char	   Cadena[MAX_LONG_CARTEL];
				ltoa(PuntajeMuerteSauriolo * Multiplicador,Cadena);
                AuxCartel->Inicializar(MX, MY, Cadena,RGB(255,255,255));
                Carteles->Agregar(CartelPuntaje);
                Multiplicador++;
                EstoyMuerto = true;
				ImagenSaurioloPicture = (Sentido == Derecha)?SaurioloRevDe:SaurioloRevIz;
				VY = -SaltoSaurioloImpacto;
				return false;
			}
			else if (Nivel->Eventos(X,Y) == evPuenteCae)
				{
					Sirve = false;
					if (Direccion == Abajo)
					{
						ObjetoBase *NuevoPuente = new PuenteCae();
						PuenteCae  *ElPuente	= (PuenteCae *)NuevoPuente;
						ElPuente->Inicializar(X,Y,(Nivel->Eventos(X,Y) == evPuenteCae)?(RetardoCaidaPuente):(RetardoPuenteFlojo));
						PuentesCaen->Agregar(NuevoPuente);
						Nivel->SetEventos(X,Y, evPisoRigido);
					}
				}
            else if (Nivel->Eventos(X, Y) == evExplotando)
                if (Direccion == Abajo)
				{
                    // Tenemos que morir: nos dieron en
                    // el piso donde estábamos.
					PlayWav(sndCaida);
                    Puntaje += PuntajeMuerteSauriolo * Multiplicador;
					ObjetoBase *CartelPuntaje = new Cartel();
					Cartel	   *AuxCartel	  = (Cartel * )CartelPuntaje;
					char	   Cadena[MAX_LONG_CARTEL];
					ltoa(PuntajeMuerteSauriolo * Multiplicador,Cadena);
                    AuxCartel->Inicializar(MX, MY, Cadena,RGB(255,255,255));
                    Carteles->Agregar(CartelPuntaje);
                    Multiplicador++;
                    EstoyMuerto = true;
					ImagenSaurioloPicture = (Sentido == Derecha)?SaurioloRevDe:SaurioloRevIz;
                    VY = -SaltoSaurioloImpacto;
                    return false;
				}
            }
    // Listo el for, ahora a trabajar!
    if (Sirve)
	{
		// Chequeamos valores para ExactoH y ExactoV.
		double A,B;
		switch (Direccion)
		{
			case Derecha:
			case Izquierda:
				ExactoV = false;
				if (MX > LlegadaX)
				{
					A = LlegadaX;
					B = MX;
				}
				else
				{
					A = MX;
					B = LlegadaX;
				}
				ExactoH = (CorreccionH = ((long)(A / 30) + 1) * 30) < B;
				break;
			case Arriba:
			case Abajo:
				ExactoH = false;
				if (MY > LlegadaY)
				{
					A = LlegadaY;
					B = MY;
				}
				else
				{
					A = MY;
					B = LlegadaY;
				}
				ExactoV = (CorreccionV = ((long)(A / 30) + 1) * 30) < B;
				break;
            default:
                break;
		}
        MX = LlegadaX;
        MY = LlegadaY;
    }
	else
	{
		double ViejoValor;
		switch (Direccion)
		{
			case Arriba:
			case Abajo:
				if (ExactoH)
				{
					ExactoH = ExactoV = false;
					ViejoValor        = MX;
					MX                = CorreccionH;
					if (!MoverSauriolo(Direccion,Velocidad))
						MX = ViejoValor;
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
					ViejoValor		  = MY;
					MY    = CorreccionV;
					if (!MoverSauriolo(Direccion,Velocidad))
						MY = ViejoValor;
					else
						return true;
				}
				else
					ExactoH = ExactoV = false;
				break;
            default:
                break;
		}
		if (Direccion == Abajo)
			VY = 0;
	}
	return Sirve;
}

void Sauriolo::ColisionTortuga()
{
	long		Cantidad, Total;
	Tortuga     * AuxPunta;
	Cantidad = 0;
    Total    = Tortugas->Tamano();
	Tortugas->SelPrimero();
    while (Cantidad < Total)
	{
        AuxPunta = ((Tortuga *)Tortugas->Avanzar());
		RECT R1,R2;
		SetRect(&R1,(long)AuxPunta->ImagenTortugaX     ,(long)AuxPunta->ImagenTortugaY,
			        (long)AuxPunta->ImagenTortugaX + 90,(long)AuxPunta->ImagenTortugaY + 30);
		SetRect(&R2,(long)MX     ,(long)MY,
			        (long)MX + 60,(long)MY + 60);
		if (AuxPunta->Resbalando &&
			ColisionGeneral(R1,R2,imglstEnemigos[AuxPunta->ImagenTortugaPicture],imglstEnemigos[ImagenSaurioloPicture]))
		{
			PlayWav(sndCaida);
            Puntaje += PuntajeMuerteTortuga * Multiplicador;
			ObjetoBase *CartelPuntaje = new Cartel();
			Cartel	   *AuxCartel	  = (Cartel *)CartelPuntaje;
			char	   Cadena[MAX_LONG_CARTEL];	
			ltoa(PuntajeMuerteTortuga * Multiplicador,Cadena);
            AuxCartel->Inicializar(AuxPunta->ImagenTortugaX, AuxPunta->ImagenTortugaY, Cadena, RGB(255,255,255));
            Carteles->Agregar(CartelPuntaje);
            Multiplicador++;
            AuxPunta->EstoyMuerto = true;
			AuxPunta->Resbalando  = false;
			AuxPunta->ImagenTortugaPicture = TortugaRev;
            AuxPunta->VY = -SaltoTortugaImpacto;
		}
        Cantidad++;
    }
}

bool Sauriolo::ColisionBolaFuego()
{
	long		Cantidad, Total;
	BolaFuego * AuxPunta;
	Cantidad = 0;
    Total    = BolasFuego->Tamano();
	BolasFuego->SelPrimero();
    while (Cantidad < Total)
	{
        AuxPunta = ((BolaFuego *)BolasFuego->Avanzar());
		RECT R1,R2;
		SetRect(&R1,(long)AuxPunta->MX     ,(long)AuxPunta->MY,
			        (long)AuxPunta->MX + 15,(long)AuxPunta->MY + 15);
		SetRect(&R2,(long)MX     ,(long)MY,
			        (long)MX + 60,(long)MY + 60);
		if (!AuxPunta->MataNicholas && ColisionGeneral(R1,R2,imglstPremios[prBolaFuego],imglstEnemigos[ImagenSaurioloPicture]))
		{
			EBF = true;
			CoordenadaCasilla((long)AuxPunta->MX + 7,(int)AuxPunta->MY + 7,EBFX,EBFY);
			Salud--;
			if (Salud <= 0)
				return true;
		}
        Cantidad++;
    }
	return false;
}

bool Sauriolo::ColisionRayoEnergia()
{
	long Cantidad, Total;
	Rayo *AuxPunta;
	Cantidad = 0;
    Total    = Rayos->Tamano();
	Rayos->SelPrimero();
    while (Cantidad < Total)
	{
        AuxPunta = ((Rayo *)Rayos->Avanzar());
		RECT R1,R2;
		SetRect(&R1,(long)AuxPunta->MX     ,(long)AuxPunta->MY,
			        (long)AuxPunta->MX + 24,(long)AuxPunta->MY + 8);
		SetRect(&R2,(long)MX     ,(long)MY,
			        (long)MX + 60,(long)MY + 60);
		if (ColisionGeneral(R1,R2,imglstEnergia[enerRayo],imglstEnemigos[ImagenSaurioloPicture]))
		{
			ERE = true;
			CoordenadaCasilla((long)AuxPunta->MX + 12,(int)AuxPunta->MY + 4,EREX,EREY);
			Salud = 0;
			return true;
		}
        Cantidad++;
    }
	return false;
}

bool Sauriolo::RevisarMuerte()
{
	RECT R;
	SetRect(&R,(long)MX     ,(long)MY,
			   (long)MX + 60,(long)MY + 60);
	ColisionTortuga();
    if (ColisionBolaFuego()   ||
		ColisionRayoEnergia() ||
		(Nicho->AuraEscudo && 
		 ColisionaNicholas(R,imglstEnemigos[ImagenSaurioloPicture]) != ColNula))
	{
		PlayWav(sndCaida);
        ImagenSaurioloPicture = (Sentido == Derecha)?SaurioloRevDe:SaurioloRevIz;
        Puntaje += PuntajeMuerteSauriolo * Multiplicador;
        ObjetoBase *CartelPuntaje = new Cartel();
		Cartel	   *AuxCartel	  = (Cartel *)CartelPuntaje;
		char	   Cadena[MAX_LONG_CARTEL];
		ltoa(PuntajeMuerteSauriolo * Multiplicador,Cadena);
        AuxCartel->Inicializar(MX, MY, Cadena,RGB(255,255,255));
        Carteles->Agregar(CartelPuntaje);
		EstoyMuerto = true;
		VY     = -SaltoSaurioloImpacto;
        return true;
	}
	if (ColisionaNicholas(R,imglstEnemigos[ImagenSaurioloPicture]) != ColNula)
		Hitazo = true;
    return false;
}
