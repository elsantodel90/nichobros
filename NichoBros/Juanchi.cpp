// En este archivo de implementan las funciones más complejas de
// la clase Juanchi.

// Incluimos las librerías necesarias.

#include "RutinasSDL/rutinasSDL.h"
#include "Juanchi.h"

// Implementamos las funciones.

void Juanchi::Inicializar(int CX, int CY)
{
    ImagenJuanchiPicture = Juanchi1;
    long X;
	int  Y;
    CasillaCoordenada(X, Y, CX, CY);
    ImagenJuanchiY = Y;
    ImagenJuanchiX = X;
    Sentido		   = Izquierda;
    EstoyMuerto	   = false;
    Aplastado	   = 0;
    Activo		   = false;
	VY			   = 0;
	Ahora		   = true;
	ExactoH		   = false;
	ExactoV		   = false;
	CorreccionH    = 0;
	CorreccionV    = 0;
}

void Juanchi::Check()
{
    if (ImagenJuanchiY > 600 || ImagenJuanchiX - Pos < -400)
	{
        Matarme();
        return;
    }
    if (!Activo)
    {
        if (ImagenJuanchiX - Pos < 800)
            Activo = true;
        else
            return;
    }
    if (EstoyMuerto)
	{
        if (Sentido == Derecha)
            ImagenJuanchiX += 0.1 * Delta;
        else
            ImagenJuanchiX -= 0.1 * Delta;
        ImagenJuanchiY += VY * Delta + Delta * Delta * AceleracionGravedad / 2;
        VY += AceleracionGravedad * Delta;
        return;
    }
    if (Aplastado != 0)
	{
        // Aunque este aplastado, la gravedad sigue
        // afectando al cadaver, que caerá si está suspendido.
        MoverJuanchi(Abajo, VY * Delta + Delta * Delta * AceleracionGravedad / 2);
		VY += AceleracionGravedad * Delta;
        Aplastado++;
        if (Aplastado > 15)
            Matarme();
		return;
    }
    if (RevisarMuerte())
	{
        Aplastado = 1;
        return;
    }
    if (Ciclo == 0)
	{
		if (Ahora)
		{
			if (ImagenJuanchiPicture == Juanchi1)
				ImagenJuanchiPicture = Juanchi2;
			else
				ImagenJuanchiPicture = Juanchi1;
			Ahora = false;
		}
	}
	else
		Ahora = true;
	MoverJuanchi(Sentido, VelocidadJuanchi * Delta);
    if (EstoyMuerto)
        return;
    MoverJuanchi(Abajo, VY * Delta + Delta * Delta * AceleracionGravedad / 2);
	VY += AceleracionGravedad * Delta;
}

bool Juanchi::MoverJuanchi(eDireccion Direccion, double Velocidad)
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
		if (!MoverJuanchi(Direccion,MaximoMovimiento))
			return false;
		// Ultimo detalle: si alguno de los sub-movimientos falla,
		// devolvemos falso. Esto gana velocidad, ya que no siempre
		// es necesario chequear todos los movimientos.
	}
    double LlegadaX, LlegadaY;
    switch (Direccion)
	{
		case Derecha:
		    LlegadaX = ImagenJuanchiX + Velocidad;
			LlegadaY = ImagenJuanchiY;
			break;
		case Izquierda:
			LlegadaX = ImagenJuanchiX - Velocidad;
			LlegadaY = ImagenJuanchiY;
			break;
		case Abajo:
			LlegadaX = ImagenJuanchiX;
			LlegadaY = ImagenJuanchiY + Velocidad;
			break;
		case Arriba:
			LlegadaX = ImagenJuanchiX;
			LlegadaY = ImagenJuanchiY - Velocidad;
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
    CoordenadaCasilla((long)LlegadaX + 29, (int)LlegadaY + 29, X2, Y2);
    Sirve = true;
    for (X = X1; X <= X2; X++)
        for (Y = Y1; Y <= Y2; Y++)
          {
            if (Nivel->Eventos(X, Y) == evPisoFragil || Nivel->Eventos(X, Y) == evPisoRigido || 
				Nivel->Eventos(X,Y) == evPinchePalo ||
				Nivel->Eventos(X,Y) == evPuenteFlojo  ||
				Nivel->Eventos(X,Y) == evBloqueMoneda ||
			   (Nivel->Eventos(X, Y) == evPararEne && Direccion != Abajo) ||
			   (Nivel->Eventos(X,Y) == evSoloParriba && 
			    Direccion == Abajo &&
				!(Y * 30 > ImagenJuanchiY - 30 &&
				  Y * 30 < ImagenJuanchiY + 29)))
			{
                if (Direccion != Abajo && Sirve)
                {
                    if (Sentido == Derecha)
                        Sentido = Izquierda;
                    else
                        Sentido = Derecha;
                }
				Sirve = false;
			}
			else if (Nivel->Eventos(X, Y) == evZonaDueleRigido)
			{
				PlayWav(sndCaida);
                Puntaje += PuntajeMuerteJuanchi * Multiplicador;
				ObjetoBase *CartelPuntaje = new Cartel();
				Cartel	   *AuxCartel	  = (Cartel * )CartelPuntaje;
				char	   Cadena[MAX_LONG_CARTEL];
				ltoa(PuntajeMuerteJuanchi * Multiplicador,Cadena);
                AuxCartel->Inicializar(ImagenJuanchiX, ImagenJuanchiY, Cadena,RGB(255,255,255));
                Carteles->Agregar(CartelPuntaje);
                Multiplicador++;
                EstoyMuerto = true;
				ImagenJuanchiPicture = JuanchiRev;
				VY = -SaltoJuanchiImpacto;
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
                    if (Aplastado == 0)
					{
						PlayWav(sndCaida);
                        Puntaje += PuntajeMuerteJuanchi * Multiplicador;
						ObjetoBase *CartelPuntaje = new Cartel();
						Cartel	   *AuxCartel	  = (Cartel * )CartelPuntaje;
						char	   Cadena[MAX_LONG_CARTEL];
						ltoa(PuntajeMuerteJuanchi * Multiplicador,Cadena);
                        AuxCartel->Inicializar(ImagenJuanchiX, ImagenJuanchiY, Cadena,RGB(255,255,255));
                        Carteles->Agregar(CartelPuntaje);
                        Multiplicador++;
                        EstoyMuerto = true;
						ImagenJuanchiPicture = JuanchiRev;
                        VY = -SaltoJuanchiImpacto;
					}
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
				if (ImagenJuanchiX > LlegadaX)
				{
					A = LlegadaX;
					B = ImagenJuanchiX;
				}
				else
				{
					A = ImagenJuanchiX;
					B = LlegadaX;
				}
				ExactoH = (CorreccionH = ((long)(A / 30) + 1) * 30) < B;
				break;
			case Arriba:
			case Abajo:
				ExactoH = false;
				if (ImagenJuanchiY > LlegadaY)
				{
					A = LlegadaY;
					B = ImagenJuanchiY;
				}
				else
				{
					A = ImagenJuanchiY;
					B = LlegadaY;
				}
				ExactoV = (CorreccionV = ((long)(A / 30) + 1) * 30) < B;
				break;
            default:
                break;
		}
        ImagenJuanchiX = LlegadaX;
        ImagenJuanchiY = LlegadaY;
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
					ViejoValor        = ImagenJuanchiX;
					ImagenJuanchiX    = CorreccionH;
					if (!MoverJuanchi(Direccion,Velocidad))
						ImagenJuanchiX = ViejoValor;
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
					ViejoValor		  = ImagenJuanchiY;
					ImagenJuanchiY    = CorreccionV;
					if (!MoverJuanchi(Direccion,Velocidad))
						ImagenJuanchiY = ViejoValor;
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

bool Juanchi::ColisionTortuga()
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
		SetRect(&R2,(long)ImagenJuanchiX     ,(long)ImagenJuanchiY,
			        (long)ImagenJuanchiX + 30,(long)ImagenJuanchiY + 30);
		if (AuxPunta->Resbalando &&
			ColisionGeneral(R1,R2,imglstEnemigos[AuxPunta->ImagenTortugaPicture],imglstEnemigos[ImagenJuanchiPicture]))
			return true;
        Cantidad++;
    }
	return false;
}

bool Juanchi::ColisionBolaFuego()
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
		SetRect(&R2,(long)ImagenJuanchiX     ,(long)ImagenJuanchiY,
			        (long)ImagenJuanchiX + 30,(long)ImagenJuanchiY + 30);
		if (ColisionGeneral(R1,R2,imglstPremios[prBolaFuego],imglstEnemigos[ImagenJuanchiPicture]))
		{
			EBF = true;
			CoordenadaCasilla((long)AuxPunta->MX + 7,(int)AuxPunta->MY + 7,EBFX,EBFY);
			return true;
		}
        Cantidad++;
    }
	return false;
}

bool Juanchi::ColisionRayoEnergia()
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
		SetRect(&R2,(long)ImagenJuanchiX     ,(long)ImagenJuanchiY,
			        (long)ImagenJuanchiX + 30,(long)ImagenJuanchiY + 30);
		if (ColisionGeneral(R1,R2,imglstEnergia[enerRayo],imglstEnemigos[ImagenJuanchiPicture]))
		{
			ERE = true;
			CoordenadaCasilla((long)AuxPunta->MX + 12,(int)AuxPunta->MY + 4,EREX,EREY);
			return true;
		}
        Cantidad++;
    }
	return false;
}

bool Juanchi::RevisarMuerte()
{
    eTiposCol AuxCol;
	RECT R;
	SetRect(&R,(long)ImagenJuanchiX     ,(long)ImagenJuanchiY,
			   (long)ImagenJuanchiX + 30,(long)ImagenJuanchiY + 30);
    if (ColisionTortuga()	  ||
		ColisionBolaFuego()   ||
		ColisionRayoEnergia() ||
		(Nicho->AuraEscudo && 
		 ColisionaNicholas(R,imglstEnemigos[ImagenJuanchiPicture]) != ColNula))
	{
		PlayWav(sndCaida);
        ImagenJuanchiPicture = JuanchiRev;
        Puntaje += PuntajeMuerteJuanchi * Multiplicador;
        ObjetoBase *CartelPuntaje = new Cartel();
		Cartel	   *AuxCartel	  = (Cartel *)CartelPuntaje;
		char	   Cadena[MAX_LONG_CARTEL];
		ltoa(PuntajeMuerteJuanchi * Multiplicador,Cadena);
        AuxCartel->Inicializar(ImagenJuanchiX, ImagenJuanchiY, Cadena,RGB(255,255,255));
        Carteles->Agregar(CartelPuntaje);
		EstoyMuerto = true;
		VY     = -SaltoJuanchiImpacto;
        return true;
	}
	if ((AuxCol = ColisionaNicholas(R,imglstEnemigos[ImagenJuanchiPicture])) == ColSuperior ||
		 AuxCol == ColSuperiorIz || AuxCol == ColSuperiorDe)
	{
        PlayWav(sndAplastado);
        ImagenJuanchiPicture = JuanchiAp;
        Puntaje += PuntajeMuerteJuanchi * Multiplicador;
        ObjetoBase *CartelPuntaje = new Cartel();
		Cartel	   *AuxCartel	  = (Cartel *)CartelPuntaje;
		char	   Cadena[MAX_LONG_CARTEL];
		ltoa(PuntajeMuerteJuanchi * Multiplicador,Cadena);
		AuxCartel->Inicializar(ImagenJuanchiX, ImagenJuanchiY, Cadena,RGB(255,255,255));
		Carteles->Agregar(CartelPuntaje);
        Multiplicador++;
        Rebotar			= true;
		Nicho->VelSalto = NicholasRebote;
		Nicho->ImagenNicholasY = (long)Nicho->ImagenNicholasY - ((long)Nicho->ImagenNicholasY % 30 + 30) % 30;
		return true;
    }
	else if (AuxCol != ColNula)
		Hitazo = true;
    return false;
}
