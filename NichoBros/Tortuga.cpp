// En este archivo se implementan las funciones más complejas de
// la clase Tortuga.

// Incluimos librerías necesarias...

#include "RutinasSDL/rutinasSDL.h"
#include "Tortuga.h"

// Implementamos las funciones de la clase.

void Tortuga::Inicializar(int CX, int CY)
{
    long X;
	int  Y;
    CasillaCoordenada(X, Y, CX, CY);
    ImagenTortugaY = Y;
    ImagenTortugaX = X;
    Sentido = Izquierda;
    ImagenTortugaPicture = TortugaIz;
    EstoyMuerto  = false;
    Aplastado	 = false;
    Activo		 = false;
    Resbalando	 = false;
    MataNicholas = false;
	VY			 = 0;
	Ahora		 = true;
}

void Tortuga::Check()
{
    if (ImagenTortugaY > 600 || ImagenTortugaX - Pos < -600)
	{
        Matarme();
        return;
	}
    if (!Activo)
    {
		if (ImagenTortugaX - Pos < 800)
            Activo = true;
        else
            return;
    }
    if (EstoyMuerto)
	{
        if (Sentido == Derecha)
            ImagenTortugaX += 0.1 * Delta;
        else
            ImagenTortugaX -= 0.1 * Delta;
        ImagenTortugaY += VY * Delta + Delta * Delta * AceleracionGravedad / 2;
        VY += AceleracionGravedad * Delta;
        return;
    }
    if (Aplastado)
	{
        // Aunque esté aplastado, la gravedad sigue
        // afectando al cadaver, que caerá si está suspendido.
        MoverTortuga(Abajo, VY * Delta + Delta * Delta * AceleracionGravedad / 2);
		VY += AceleracionGravedad * Delta;
        if (Resbalando)
		{
            if (MataNicholas && RevisarMuerte())
			{
                Resbalando   = false;
				MataNicholas = false;
				return;
			}
            MoverTortuga(Sentido, VelocidadTortugaResbala * Delta);
		}
        else
		{
            // Chequeamos si somos empujados...
			if (RevisarMuerte())
				return;
            if (RevisarEmpuje())
			{
				PlayWav(sndOhOh);
                Resbalando = true;
			}
		}
        return;
    }
    if (RevisarMuerte())
	{
        Aplastado = true;
        return;
	}
    if (Ciclo == 0)
	{
		if (Ahora)
		{
			if (ImagenTortugaPicture == TortugaCaDe2 || 
				ImagenTortugaPicture == TortugaCaIz2)
					ImagenTortugaPicture = (eImagTortuga)((int)ImagenTortugaPicture - 2);
			else
				ImagenTortugaPicture = (eImagTortuga)((int)ImagenTortugaPicture + 1);
			Ahora = false;
		}
	}
	else
		Ahora = true;
	MoverTortuga(Sentido, VelocidadTortuga * Delta);
    if (EstoyMuerto)
        return;
    MoverTortuga(Abajo, VY * Delta + Delta * Delta * AceleracionGravedad / 2);
	VY += Delta * AceleracionGravedad;
}

bool Tortuga::MoverTortuga(eDireccion Direccion, double Velocidad)
{
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
		if (!MoverTortuga(Direccion,MaximoMovimiento))
			return false;
		// Ultimo detalle: si alguno de los sub-movimientos falla,
		// devolvemos falso. Esto gana velocidad, ya que no siempre
		// es necesario chequear todos los movimientos.
	}
    double LlegadaX , LlegadaY;
    switch (Direccion)
	{
		case Derecha:
			LlegadaX = ImagenTortugaX + Velocidad;
			LlegadaY = ImagenTortugaY;
			break;
		case Izquierda:
			LlegadaX = ImagenTortugaX - Velocidad;
			LlegadaY = ImagenTortugaY;
			break;
		case Abajo:
			LlegadaX = ImagenTortugaX;
			LlegadaY = ImagenTortugaY + Velocidad;
			break;
		case Arriba:
			LlegadaX = ImagenTortugaX;
			LlegadaY = ImagenTortugaY - Velocidad;
			break;
        default:
            break;
    }
    // Ahora, solo tenemos que chequear si la posicion
    // marcada es valida.
    int  X , Y;
    int  X1, Y1;
    int  X2, Y2;
    bool Sirve;
    CoordenadaCasilla((long)LlegadaX, (int)LlegadaY, X1, Y1);
    CoordenadaCasilla((long)LlegadaX + 90 - 1, (int)LlegadaY + 30 - 1, X2, Y2);
    Sirve = true;
    for (X = X1; X <= X2; X++)
        for (Y = Y1; Y <= Y2; Y++)
          {
            if (Nivel->Eventos(X, Y) == evPisoFragil	||
				Nivel->Eventos(X, Y) == evPisoRigido	||
				Nivel->Eventos(X,Y) == evPinchePalo		||
				Nivel->Eventos(X, Y) == evPuenteFlojo	||
				Nivel->Eventos(X,Y)  == evBloqueMoneda  ||
			   (Nivel->Eventos(X, Y) == evPararEne && Direccion != Abajo && !Resbalando) ||
			   (Nivel->Eventos(X,Y) == evSoloParriba && 
			    Direccion == Abajo &&
				!(Y * 30 > ImagenTortugaY - 30 &&
				  Y * 30 < ImagenTortugaY + 29)))
			{
                if (Direccion != Abajo && Sirve)
                {
                    if (Sentido == Derecha)
					{
                        Sentido = Izquierda;
                        if (!Resbalando)
                            ImagenTortugaPicture = TortugaIz;
						else
							MataNicholas = true;
					}
                    else
					{
                        Sentido = Derecha;
                        if (!Resbalando)
                            ImagenTortugaPicture = TortugaDe;
						else
							MataNicholas = true;
					}
                }
				Sirve = false;
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
			else if (Nivel->Eventos(X, Y) == evZonaDueleRigido)
			{
				PlayWav(sndCaida);
                Puntaje += PuntajeMuerteTortuga * Multiplicador;
				ObjetoBase *CartelPuntaje = new Cartel();
				Cartel	   *AuxCartel	  = (Cartel *)CartelPuntaje;
				char	   Cadena[MAX_LONG_CARTEL];	
				ltoa(PuntajeMuerteTortuga * Multiplicador,Cadena);
                AuxCartel->Inicializar(ImagenTortugaX, ImagenTortugaY, Cadena, RGB(255,255,255));
                Carteles->Agregar(CartelPuntaje);
                Multiplicador++;
                EstoyMuerto = true;
				Resbalando  = false;
				ImagenTortugaPicture = TortugaRev;
                VY = -SaltoTortugaImpacto;
                return false;
			}
            else if (Nivel->Eventos(X, Y) == evExplotando)
                if (Direccion == Abajo)
				{
                    // Tenemos que morir: nos dieron en
                    // el piso donde estábamos.
					PlayWav(sndCaida);
                    Puntaje += PuntajeMuerteTortuga * Multiplicador;
					ObjetoBase *CartelPuntaje = new Cartel();
					Cartel	   *AuxCartel	  = (Cartel *)CartelPuntaje;
					char	   Cadena[MAX_LONG_CARTEL];	
					ltoa(PuntajeMuerteTortuga * Multiplicador,Cadena);
                    AuxCartel->Inicializar(ImagenTortugaX, ImagenTortugaY, Cadena, RGB(255,255,255));
                    Carteles->Agregar(CartelPuntaje);
                    Multiplicador++;
                    EstoyMuerto = true;
					Resbalando  = false;
					ImagenTortugaPicture = TortugaRev;
                    VY = -SaltoTortugaImpacto;
                    return false;
                }
            }
	// Listo el for, ahora a trabajar!
    if (Sirve)
	{
        ImagenTortugaX = LlegadaX;
        ImagenTortugaY = LlegadaY;
	}
	else if (Direccion == Abajo)
		VY = 0;
	return Sirve;
}

bool Tortuga::ColisionTortuga()
{
	long		Cantidad, Total;
	Tortuga     * AuxPunta;
	Nodo		* AuxViejoActual;
	AuxViejoActual = Tortugas->actual;
	Cantidad = 0;
    Total    = Tortugas->Tamano();
	Tortugas->SelPrimero();
    while (Cantidad < Total)
	{
        AuxPunta = ((Tortuga *)Tortugas->Avanzar());
		RECT R1,R2;
		SetRect(&R1,(long)AuxPunta->ImagenTortugaX     ,(long)AuxPunta->ImagenTortugaY,
			        (long)AuxPunta->ImagenTortugaX + 90,(long)AuxPunta->ImagenTortugaY + 30);
		SetRect(&R2,(long)ImagenTortugaX     ,(long)ImagenTortugaY,
			        (long)ImagenTortugaX + 90,(long)ImagenTortugaY + 30);
		if (AuxPunta->Resbalando &&
			ColisionGeneral(R1,R2,imglstEnemigos[AuxPunta->ImagenTortugaPicture],imglstEnemigos[ImagenTortugaPicture]) != ColNula)
		{
			Tortugas->actual = AuxViejoActual;
			return true;
		}
        Cantidad++;
    }
	Tortugas->actual = AuxViejoActual;
	return false;
}

bool Tortuga::ColisionBolaFuego()
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
		SetRect(&R2,(long)ImagenTortugaX     ,(long)ImagenTortugaY,
			        (long)ImagenTortugaX + 90,(long)ImagenTortugaY + 30);
		if (ColisionGeneral(R1,R2,imglstPremios[prBolaFuego],imglstEnemigos[ImagenTortugaPicture]))
		{
			EBF = true;
			CoordenadaCasilla((long)AuxPunta->MX + 7,(int)AuxPunta->MY + 7,EBFX,EBFY);
			return true;
		}
        Cantidad++;
    }
	return false;
}

bool Tortuga::ColisionRayoEnergia()
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
		SetRect(&R2,(long)ImagenTortugaX     ,(long)ImagenTortugaY,
			        (long)ImagenTortugaX + 90,(long)ImagenTortugaY + 30);
		if (ColisionGeneral(R1,R2,imglstEnergia[enerRayo],imglstEnemigos[ImagenTortugaPicture]))
		{
			ERE = true;
			CoordenadaCasilla((long)AuxPunta->MX + 12,(int)AuxPunta->MY + 4,EREX,EREY);
			return true;
		}
        Cantidad++;
    }
	return false;
}

bool Tortuga::RevisarMuerte()
{
    eTiposCol AuxCol;
	RECT R;
	SetRect(&R,(long)ImagenTortugaX     ,(long)ImagenTortugaY,
		       (long)ImagenTortugaX + 90,(long)ImagenTortugaY + 30);
    if ((!Resbalando && ColisionTortuga()) ||
		 ColisionBolaFuego()			   ||
		 ColisionRayoEnergia()			   ||
		 (Nicho->AuraEscudo && 
		 ColisionaNicholas(R,imglstEnemigos[ImagenTortugaPicture])))
		{
			PlayWav(sndCaida);
            Puntaje += PuntajeMuerteTortuga * Multiplicador;
            ObjetoBase *CartelPuntaje = new Cartel();
			Cartel	   *AuxCartel	  = (Cartel *)CartelPuntaje;
			char	   Cadena[MAX_LONG_CARTEL];	
			ltoa(PuntajeMuerteTortuga * Multiplicador,Cadena);
            AuxCartel->Inicializar(ImagenTortugaX, ImagenTortugaY, Cadena, RGB(255,255,255));
            Carteles->Agregar(CartelPuntaje);
            EstoyMuerto = true;
			Resbalando  = false;
			ImagenTortugaPicture = TortugaRev;
            VY = -SaltoTortugaImpacto;
			return true;
		}
        else if (((AuxCol = ColisionaNicholas(R,imglstEnemigos[ImagenTortugaPicture])) == ColSuperior ||
				   AuxCol == ColSuperiorIz || AuxCol == ColSuperiorDe) && (!Aplastado || Resbalando))
		{
			PlayWav(sndPlop);
		    ImagenTortugaPicture = TortugaRev;
            Multiplicador++;
            Rebotar			= true;
			Nicho->VelSalto = NicholasRebote;
			Nicho->ImagenNicholasY = (long)Nicho->ImagenNicholasY - ((long)Nicho->ImagenNicholasY % 30 + 30) % 30;
            return true;
        }
		else if (AuxCol != ColNula && (!Aplastado || MataNicholas))
		{
			Hitazo	   = true;
			Resbalando = false;
		}
    return false;
}

bool Tortuga::RevisarEmpuje()
{
	eTiposCol AuxCol;
	RECT R;
	SetRect(&R,(long)ImagenTortugaX     ,(long)ImagenTortugaY,
		       (long)ImagenTortugaX + 90,(long)ImagenTortugaY + 30);
    if ((AuxCol = ColisionaNicholas(R,imglstEnemigos[ImagenTortugaPicture])) != ColNula)
	{
        switch (AuxCol)
		{
		case ColSuperiorIz:
		case ColIzquierda:
		case ColInferiorIz:
			Sentido = Derecha;
			break;
        case ColSuperiorDe:
		case ColDerecha:
		case ColInferiorDe:
			Sentido = Izquierda;
			break;
		default: // Colision superior inferior y total
			Sentido = (Nicho->ImagenNicholasX + 30 < ImagenTortugaX + 45)?(Derecha):(Izquierda);
			break;
		}
		MataNicholas = false;
        return true;
    }
    return false;
}
