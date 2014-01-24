// En este archivo de implementan las funciones más complejas de
// la clase Caparatuzo.

// Incluimos las librerías necesarias.

#include "RutinasSDL/rutinasSDL.h"
#include "Caparatuzo.h"

// Implementamos las funciones.

void Caparatuzo::Inicializar(int CX, int CY)
{
    ImagenCaparatuzoPicture = capDe1;
    long X;
	int  Y;
    CasillaCoordenada(X, Y, CX, CY);
    ImagenCaparatuzoY = Y;
    ImagenCaparatuzoX = X;
    Sentido			  = Derecha;
    EstoyMuerto		  = false;
    Activo		      = false;
	VY			      = 0;
	Ahora		      = true;
	ExactoH		      = false;
	ExactoV		      = false;
	CorreccionH       = 0;
	CorreccionV       = 0;
}

void Caparatuzo::Check()
{
    if (ImagenCaparatuzoY > 600 || ImagenCaparatuzoX - Pos < -400)
	{
        Matarme();
        return;
    }
    if (!Activo)
    {
        if (ImagenCaparatuzoX - Pos < 800)
            Activo = true;
        else
            return;
    }
    if (EstoyMuerto)
	{
        if (Sentido == Derecha)
            ImagenCaparatuzoX += 0.1 * Delta;
        else
            ImagenCaparatuzoX -= 0.1 * Delta;
        ImagenCaparatuzoY += VY * Delta + Delta * Delta * AceleracionGravedad / 2;
        VY += AceleracionGravedad * Delta;
        return;
    }
    if (RevisarMuerte())
        return;
    if (Ciclo == 0)
	{
		if (Ahora)
		{
			if (Sentido == Derecha)
				ImagenCaparatuzoPicture = (ImagenCaparatuzoPicture == capDe1)?capDe2:capDe1;
			else
				ImagenCaparatuzoPicture = (ImagenCaparatuzoPicture == capIz1)?capIz2:capIz1;
			Ahora = false;
		}
	}
	else
		Ahora = true;
	MoverCaparatuzo(Sentido, VelocidadCaparatuzo * Delta);
    if (EstoyMuerto)
        return;
    MoverCaparatuzo(Abajo, VY * Delta + Delta * Delta * AceleracionGravedad / 2);
	VY += AceleracionGravedad * Delta;
}

bool Caparatuzo::MoverCaparatuzo(eDireccion Direccion, double Velocidad)
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
		if (!MoverCaparatuzo(Direccion,MaximoMovimiento))
			return false;
		// Ultimo detalle: si alguno de los sub-movimientos falla,
		// devolvemos falso. Esto gana velocidad, ya que no siempre
		// es necesario chequear todos los movimientos.
	}
    double LlegadaX, LlegadaY;
    switch (Direccion)
	{
		case Derecha:
		    LlegadaX = ImagenCaparatuzoX + Velocidad;
			LlegadaY = ImagenCaparatuzoY;
			break;
		case Izquierda:
			LlegadaX = ImagenCaparatuzoX - Velocidad;
			LlegadaY = ImagenCaparatuzoY;
			break;
		case Abajo:
			LlegadaX = ImagenCaparatuzoX;
			LlegadaY = ImagenCaparatuzoY + Velocidad;
			break;
		case Arriba:
			LlegadaX = ImagenCaparatuzoX;
			LlegadaY = ImagenCaparatuzoY - Velocidad;
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
				!(Y * 30 > ImagenCaparatuzoY - 30 &&
				  Y * 30 < ImagenCaparatuzoY + 29)))
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
                Puntaje += PuntajeMuerteCaparatuzo * Multiplicador;
				ObjetoBase *CartelPuntaje = new Cartel();
				Cartel	   *AuxCartel	  = (Cartel * )CartelPuntaje;
				char	   Cadena[MAX_LONG_CARTEL];
				ltoa(PuntajeMuerteCaparatuzo * Multiplicador,Cadena);
                AuxCartel->Inicializar(ImagenCaparatuzoX, ImagenCaparatuzoY, Cadena,RGB(255,255,255));
                Carteles->Agregar(CartelPuntaje);
                Multiplicador++;
                EstoyMuerto = true;
				ImagenCaparatuzoPicture = capRev;
				VY = -SaltoCaparatuzoImpacto;
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
                    Puntaje += PuntajeMuerteCaparatuzo * Multiplicador;
					ObjetoBase *CartelPuntaje = new Cartel();
					Cartel	   *AuxCartel	  = (Cartel * )CartelPuntaje;
					char	   Cadena[MAX_LONG_CARTEL];
					ltoa(PuntajeMuerteCaparatuzo * Multiplicador,Cadena);
                    AuxCartel->Inicializar(ImagenCaparatuzoX, ImagenCaparatuzoY, Cadena,RGB(255,255,255));
                    Carteles->Agregar(CartelPuntaje);
                    Multiplicador++;
                    EstoyMuerto = true;
					ImagenCaparatuzoPicture = capRev;
                    VY = -SaltoCaparatuzoImpacto;
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
				if (ImagenCaparatuzoX > LlegadaX)
				{
					A = LlegadaX;
					B = ImagenCaparatuzoX;
				}
				else
				{
					A = ImagenCaparatuzoX;
					B = LlegadaX;
				}
				ExactoH = (CorreccionH = ((long)(A / 30) + 1) * 30) < B;
				break;
			case Arriba:
			case Abajo:
				ExactoH = false;
				if (ImagenCaparatuzoY > LlegadaY)
				{
					A = LlegadaY;
					B = ImagenCaparatuzoY;
				}
				else
				{
					A = ImagenCaparatuzoY;
					B = LlegadaY;
				}
				ExactoV = (CorreccionV = ((long)(A / 30) + 1) * 30) < B;
				break;
            default:
                break;
		}
        ImagenCaparatuzoX = LlegadaX;
        ImagenCaparatuzoY = LlegadaY;
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
					ViejoValor        = ImagenCaparatuzoX;
					ImagenCaparatuzoX = CorreccionH;
					if (!MoverCaparatuzo(Direccion,Velocidad))
						ImagenCaparatuzoX = ViejoValor;
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
					ViejoValor		  = ImagenCaparatuzoY;
					ImagenCaparatuzoY = CorreccionV;
					if (!MoverCaparatuzo(Direccion,Velocidad))
						ImagenCaparatuzoY = ViejoValor;
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

bool Caparatuzo::ColisionTortuga()
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
		SetRect(&R2,(long)ImagenCaparatuzoX     ,(long)ImagenCaparatuzoY,
			        (long)ImagenCaparatuzoX + 30,(long)ImagenCaparatuzoY + 30);
		if (AuxPunta->Resbalando &&
			ColisionGeneral(R1,R2,imglstEnemigos[AuxPunta->ImagenTortugaPicture],imglstEnemigos[ImagenCaparatuzoPicture]))
			return true;
        Cantidad++;
    }
	return false;
}

bool Caparatuzo::ColisionBolaFuego()
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
		SetRect(&R2,(long)ImagenCaparatuzoX     ,(long)ImagenCaparatuzoY,
			        (long)ImagenCaparatuzoX + 30,(long)ImagenCaparatuzoY + 30);
		if (ColisionGeneral(R1,R2,imglstPremios[prBolaFuego],imglstEnemigos[ImagenCaparatuzoPicture]))
		{
			EBF = true;
			CoordenadaCasilla((long)AuxPunta->MX + 7,(int)AuxPunta->MY + 7,EBFX,EBFY);
			return true;
		}
        Cantidad++;
    }
	return false;
}

bool Caparatuzo::ColisionRayoEnergia()
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
		SetRect(&R2,(long)ImagenCaparatuzoX     ,(long)ImagenCaparatuzoY,
			        (long)ImagenCaparatuzoX + 30,(long)ImagenCaparatuzoY + 30);
		if (ColisionGeneral(R1,R2,imglstEnergia[enerRayo],imglstEnemigos[ImagenCaparatuzoPicture]))
		{
			ERE = true;
			CoordenadaCasilla((long)AuxPunta->MX + 12,(int)AuxPunta->MY + 4,EREX,EREY);
			return true;
		}
        Cantidad++;
    }
	return false;
}

bool Caparatuzo::RevisarMuerte()
{
	RECT R;
	SetRect(&R,(long)ImagenCaparatuzoX     ,(long)ImagenCaparatuzoY,
			   (long)ImagenCaparatuzoX + 30,(long)ImagenCaparatuzoY + 30);
    if (ColisionTortuga()	  ||
		ColisionBolaFuego()   ||
		ColisionRayoEnergia() ||
		(Nicho->AuraEscudo && 
		 ColisionaNicholas(R,imglstEnemigos[ImagenCaparatuzoPicture]) != ColNula))
	{
		PlayWav(sndCaida);
        ImagenCaparatuzoPicture = capRev;
        Puntaje += PuntajeMuerteCaparatuzo * Multiplicador;
        ObjetoBase *CartelPuntaje = new Cartel();
		Cartel	   *AuxCartel	  = (Cartel *)CartelPuntaje;
		char	   Cadena[MAX_LONG_CARTEL];
		ltoa(PuntajeMuerteCaparatuzo * Multiplicador,Cadena);
        AuxCartel->Inicializar(ImagenCaparatuzoX, ImagenCaparatuzoY, Cadena,RGB(255,255,255));
        Carteles->Agregar(CartelPuntaje);
		EstoyMuerto = true;
		VY     = -SaltoCaparatuzoImpacto;
        return true;
	}
	if (ColisionaNicholas(R,imglstEnemigos[ImagenCaparatuzoPicture]) != ColNula)
		Hitazo = true;
    return false;
}
