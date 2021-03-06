// En este archivo de implementan las funciones m�s complejas de
// la clase Bichoto.

// Incluimos las librer�as necesarias.

#include "RutinasSDL/rutinasSDL.h"
#include "Bichoto.h"

// Implementamos las funciones.

void Bichoto::Inicializar(int CX, int CY)
{
    ImagenBichotoPicture = biIz1;
    long X;
	int  Y;
    CasillaCoordenada(X, Y, CX, CY);
    ImagenBichotoY = Y;
    ImagenBichotoX = X;
    Sentido		   = Izquierda;
    EstoyMuerto	   = false;
    Aplastado	   = false;
    Activo		   = false;
	VY			   = 0;
	Ahora		   = true;
	ExactoH		   = false;
	ExactoV		   = false;
	CorreccionH    = 0;
	CorreccionV    = 0;
}

void Bichoto::Check()
{
    if (ImagenBichotoY > 600 || ImagenBichotoX - Pos < -400)
	{
        Matarme();
        return;
    }
    if (!Activo)
    {
        if (ImagenBichotoX - Pos < 800)
            Activo = true;
        else
            return;
    }
    if (EstoyMuerto)
	{
        if (Sentido == Derecha)
            ImagenBichotoX += 0.1 * Delta;
        else
            ImagenBichotoX -= 0.1 * Delta;
        ImagenBichotoY += VY * Delta + Delta * Delta * AceleracionGravedad / 2;
        VY += AceleracionGravedad * Delta;
        return;
    }
	if (RevisarMuerte())
	{
        Aplastado = !Aplastado;
        return;
    }
    if (Aplastado)
	{
        // Aunque este aplastado, la gravedad sigue
        // afectando al caparazon, que caer� si est� suspendido.
        MoverBichoto(Abajo, VY * Delta + Delta * Delta * AceleracionGravedad / 2);
		VY += AceleracionGravedad * Delta;
		return;
    }
    if (Ciclo == 0)
	{
		if (Ahora)
		{
			if (Sentido == Derecha)
				if (ImagenBichotoPicture == biDe1)
					ImagenBichotoPicture = biDe2;
				else
					ImagenBichotoPicture = biDe1;
			else if (ImagenBichotoPicture == biIz1)
				ImagenBichotoPicture = biIz2;
			else
				ImagenBichotoPicture = biIz1;
			Ahora = false;
		}
	}
	else
		Ahora = true;
	MoverBichoto(Sentido, VelocidadBichoto * Delta);
    if (EstoyMuerto)
        return;
    MoverBichoto(Abajo, VY * Delta + Delta * Delta * AceleracionGravedad / 2);
	VY += AceleracionGravedad * Delta;
}

bool Bichoto::MoverBichoto(eDireccion Direccion, double Velocidad)
{
	while (Velocidad > MaximoMovimiento)
	{
		// El objetivo de esto es descomponer movimientos muy
		// grandes en varios m�s chicos. En efecto, es necesario
		// hacerlo. Pensemos en el dibujo:
		//			    #|
		// Nicholas es el # y el | es un bloque. Si nuestro
		// movimiento es de 3 casillas, la posicion final ser�
		//				 | #
		// que es perfectamente v�lida. Conclusi�n: debemos evitar
		// movimientos muy grandes.
		// Ah, me olvidava. Para simplificar el c�digo, utilizamos
		// la magia de la recursividad. :-)-)-)
		Velocidad -= MaximoMovimiento;
		if (!MoverBichoto(Direccion,MaximoMovimiento))
			return false;
		// Ultimo detalle: si alguno de los sub-movimientos falla,
		// devolvemos falso. Esto gana velocidad, ya que no siempre
		// es necesario chequear todos los movimientos.
	}
    double LlegadaX, LlegadaY;
    switch (Direccion)
	{
		case Derecha:
		    LlegadaX = ImagenBichotoX + Velocidad;
			LlegadaY = ImagenBichotoY;
			break;
		case Izquierda:
			LlegadaX = ImagenBichotoX - Velocidad;
			LlegadaY = ImagenBichotoY;
			break;
		case Abajo:
			LlegadaX = ImagenBichotoX;
			LlegadaY = ImagenBichotoY + Velocidad;
			break;
		case Arriba:
			LlegadaX = ImagenBichotoX;
			LlegadaY = ImagenBichotoY - Velocidad;
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
				!(Y * 30 > ImagenBichotoY - 30 &&
				  Y * 30 < ImagenBichotoY + 29)))
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
                Puntaje += PuntajeMuerteBichoto * Multiplicador;
				ObjetoBase *CartelPuntaje = new Cartel();
				Cartel	   *AuxCartel	  = (Cartel * )CartelPuntaje;
				char	   Cadena[MAX_LONG_CARTEL];
				ltoa(PuntajeMuerteBichoto * Multiplicador,Cadena);
                AuxCartel->Inicializar(ImagenBichotoX, ImagenBichotoY, Cadena,RGB(255,255,255));
                Carteles->Agregar(CartelPuntaje);
                Multiplicador++;
                EstoyMuerto = true;
				ImagenBichotoPicture = biRev;
				VY = -SaltoBichotoImpacto;
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
            {
                if (Direccion == Abajo)
				{
                    // Tenemos que morir: nos dieron en
                    // el piso donde est�bamos.
                    if (Aplastado == 0)
					{
						PlayWav(sndCaida);
                        Puntaje += PuntajeMuerteBichoto * Multiplicador;
						ObjetoBase *CartelPuntaje = new Cartel();
						Cartel	   *AuxCartel	  = (Cartel * )CartelPuntaje;
						char	   Cadena[MAX_LONG_CARTEL];
						ltoa(PuntajeMuerteBichoto * Multiplicador,Cadena);
                        AuxCartel->Inicializar(ImagenBichotoX, ImagenBichotoY, Cadena,RGB(255,255,255));
                        Carteles->Agregar(CartelPuntaje);
                        Multiplicador++;
                        EstoyMuerto = true;
						ImagenBichotoPicture = biRev;
                        VY = -SaltoBichotoImpacto;
					}
                    return false;
				}
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
				if (ImagenBichotoX > LlegadaX)
				{
					A = LlegadaX;
					B = ImagenBichotoX;
				}
				else
				{
					A = ImagenBichotoX;
					B = LlegadaX;
				}
				ExactoH = (CorreccionH = ((long)(A / 30) + 1) * 30) < B;
				break;
			case Arriba:
			case Abajo:
				ExactoH = false;
				if (ImagenBichotoY > LlegadaY)
				{
					A = LlegadaY;
					B = ImagenBichotoY;
				}
				else
				{
					A = ImagenBichotoY;
					B = LlegadaY;
				}
				ExactoV = (CorreccionV = ((long)(A / 30) + 1) * 30) < B;
				break;
            default:
                break;
		}
        ImagenBichotoX = LlegadaX;
        ImagenBichotoY = LlegadaY;
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
					ViejoValor        = ImagenBichotoX;
					ImagenBichotoX    = CorreccionH;
					if (!MoverBichoto(Direccion,Velocidad))
						ImagenBichotoX = ViejoValor;
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
					ViejoValor		  = ImagenBichotoY;
					ImagenBichotoY    = CorreccionV;
					if (!MoverBichoto(Direccion,Velocidad))
						ImagenBichotoY = ViejoValor;
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

bool Bichoto::ColisionTortuga()
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
		SetRect(&R2,(long)ImagenBichotoX     ,(long)ImagenBichotoY,
			        (long)ImagenBichotoX + 30,(long)ImagenBichotoY + 30);
		if (AuxPunta->Resbalando &&
			ColisionGeneral(R1,R2,imglstEnemigos[AuxPunta->ImagenTortugaPicture],imglstEnemigos[ImagenBichotoPicture]))
		{
			// Chau tortuga
			return true;
		}
        Cantidad++;
    }
	return false;
}

bool Bichoto::ColisionBolaFuego()
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
		SetRect(&R2,(long)ImagenBichotoX     ,(long)ImagenBichotoY,
			        (long)ImagenBichotoX + 30,(long)ImagenBichotoY + 30);
		if (ColisionGeneral(R1,R2,imglstPremios[prBolaFuego],imglstEnemigos[ImagenBichotoPicture]))
		{
			EBF = true;
			CoordenadaCasilla((long)AuxPunta->MX + 7,(int)AuxPunta->MY + 7,EBFX,EBFY);
			return true;
		}
        Cantidad++;
    }
	return false;
}

bool Bichoto::ColisionRayoEnergia()
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
		SetRect(&R2,(long)ImagenBichotoX     ,(long)ImagenBichotoY,
			        (long)ImagenBichotoX + 30,(long)ImagenBichotoY + 30);
		if (ColisionGeneral(R1,R2,imglstEnergia[enerRayo],imglstEnemigos[ImagenBichotoPicture]))
		{
			ERE = true;
			CoordenadaCasilla((long)AuxPunta->MX + 12,(int)AuxPunta->MY + 4,EREX,EREY);
			return true;
		}
        Cantidad++;
    }
	return false;
}

bool Bichoto::RevisarMuerte()
{
    eTiposCol AuxCol = ColNula;
	RECT R;
	SetRect(&R,(long)ImagenBichotoX     ,(long)ImagenBichotoY,
			   (long)ImagenBichotoX + 30,(long)ImagenBichotoY + 30);
    if (ColisionRayoEnergia() ||
		(Nicho->AuraEscudo && 
		 ColisionaNicholas(R,imglstEnemigos[ImagenBichotoPicture]) != ColNula))
	{
		PlayWav(sndCaida);
        ImagenBichotoPicture = biRev;
        Puntaje += PuntajeMuerteBichoto * Multiplicador;
        ObjetoBase *CartelPuntaje = new Cartel();
		Cartel	   *AuxCartel	  = (Cartel *)CartelPuntaje;
		char	   Cadena[MAX_LONG_CARTEL];
		ltoa(PuntajeMuerteBichoto * Multiplicador,Cadena);
        AuxCartel->Inicializar(ImagenBichotoX, ImagenBichotoY, Cadena,RGB(255,255,255));
        Carteles->Agregar(CartelPuntaje);
		EstoyMuerto = true;
		VY     = -SaltoBichotoImpacto;
        return true;
	}
    AuxCol = ColisionaNicholas(R,imglstEnemigos[ImagenBichotoPicture]);
	if (ColisionTortuga()	  ||
		ColisionBolaFuego()   || AuxCol == ColSuperior ||
		 AuxCol == ColSuperiorIz || AuxCol == ColSuperiorDe)
	{
        PlayWav(sndPlop);
        ImagenBichotoPicture = biRev;
        Multiplicador++;
		if (AuxCol != ColNula)
		{
			Rebotar			= true;
			Nicho->VelSalto = NicholasRebote;
			Nicho->ImagenNicholasY = (long)Nicho->ImagenNicholasY - ((long)Nicho->ImagenNicholasY % 30 + 30) % 30;
		}
		return true;
    }
	else if (AuxCol != ColNula)
		Hitazo = true;
    return false;
}
