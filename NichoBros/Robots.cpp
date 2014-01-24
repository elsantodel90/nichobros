// En este archivo de implementan las funciones más complejas de
// la clase Robots, definiendo así su comportamiento.

// Incluimos las librerías necesarias.

#include "RutinasSDL/rutinasSDL.h"
#include "Robots.h"

// Implementamos las funciones.

void Robots::Inicializar(int CX, int CY)
{
    Intermitente = 0;
    ImagenRobotsPicture = RobotsNormal;
    long X;
	int  Y;
    CasillaCoordenada(X, Y, CX, CY);
    ImagenRobotsY   = Y;
    ImagenRobotsX   = X;
    Sentido		    = Izquierda;
    EstoyMuerto	    = false;
    Activo		    = false;
	VY			    = 0;
	SinAccion		= 0;
	switch (Dificultad)
	{
		case inQuesoRemachado:
			GolpesRestantes = RobotsGolpesSoportables - 6;
			break;
		case inNewby:
			GolpesRestantes = RobotsGolpesSoportables - 3;
			break;
		case inJuegueroDecente:
			GolpesRestantes = RobotsGolpesSoportables;
			break;
		case inCyberAdicto:
			GolpesRestantes = RobotsGolpesSoportables + 6;
			break;
		default: // Esto no debería pasar nunca...
			GolpesRestantes = RobotsGolpesSoportables;
			break;
	}
	ExactoH		    = false;
	ExactoV		    = false;
	CorreccionH     = 0;
	CorreccionV     = 0;
}

bool DamosGiro()
{
	int k;
	for (k = 0; k < Delta; k++)
		if (rand() % ProbabilidadRobotsGiro == 0)
			return true;
	return false;
}

bool Disparamos()
{
	int k;
	for (k = 0; k < Delta; k++)
		if (rand() % ProbabilidadRobotsTiro == 0)
			return true;
	return false;
}

void Robots::Check()
{
    if (ImagenRobotsY > 600)
	{
        Matarme();
        return;
    }
    if (!Activo)
    {
        if (ImagenRobotsX - Pos < 800)
            Activo = true;
        else
            return;
    }
    if (EstoyMuerto)
	{
        if (Sentido == Derecha)
            ImagenRobotsX += 0.1 * Delta;
        else
            ImagenRobotsX -= 0.1 * Delta;
        ImagenRobotsY += VY * Delta + Delta * Delta * AceleracionGravedad / 2;
        VY += AceleracionGravedad * Delta;
        return;
    }
	if (SinAccion > Delta)
		SinAccion -= Delta;
	else
		SinAccion = 0;
	if (DamosGiro())
	{
		Sentido = (Sentido == Derecha)?(Izquierda):(Derecha);
		SinAccion = TiempoSinAccionRobots;
	}
	if (Disparamos())
	{
		SinAccion = TiempoSinAccionRobots;
		ObjetoBase *NuevoRayo = new Rayo();
		Rayo	   *AuxRayo   = (Rayo *)NuevoRayo;
		double RayoX,RayoY;
		double RayoVX;
		RayoX  = (Sentido == Derecha)?(ImagenRobotsX + 55):(ImagenRobotsX - 20);
		RayoY  = ImagenRobotsY + 15 + rand() % 20;
		RayoVX = (rand() % 2 == 0)?(VelocidadRayoEnergia / (rand() % 4 + 1)):(-VelocidadRayoEnergia / (rand() % 4 + 1));
		AuxRayo->Inicializar(RayoX,RayoY,RayoVX,true,false);
		Rayos->Agregar(NuevoRayo);
	}
	if (Intermitente > Delta)
		Intermitente -= Delta;
	else
		Intermitente = 0; // Simplemente para evitar problemas
    if (RevisarMuerte())
        return;
	MoverRobots(Sentido, VelocidadRobots * Delta);
    if (EstoyMuerto)
        return;
    MoverRobots(Abajo, VY * Delta + Delta * Delta * AceleracionGravedad / 2);
	VY += AceleracionGravedad * Delta;
}

bool Robots::MoverRobots(eDireccion Direccion, double Velocidad)
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
		if (!MoverRobots(Direccion,MaximoMovimiento))
			return false;
		// Ultimo detalle: si alguno de los sub-movimientos falla,
		// devolvemos falso. Esto gana velocidad, ya que no siempre
		// es necesario chequear todos los movimientos.
	}
    double LlegadaX, LlegadaY;
    switch (Direccion)
	{
		case Derecha:
		    LlegadaX = ImagenRobotsX + Velocidad;
			LlegadaY = ImagenRobotsY;
			break;
		case Izquierda:
			LlegadaX = ImagenRobotsX - Velocidad;
			LlegadaY = ImagenRobotsY;
			break;
		case Abajo:
			LlegadaX = ImagenRobotsX;
			LlegadaY = ImagenRobotsY + Velocidad;
			break;
		case Arriba:
			LlegadaX = ImagenRobotsX;
			LlegadaY = ImagenRobotsY - Velocidad;
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
				Nivel->Eventos(X,Y) == evPinchePalo       ||
				Nivel->Eventos(X, Y) == evPuenteFlojo     ||
				Nivel->Eventos(X, Y) == evZonaDueleRigido ||
			   (Nivel->Eventos(X, Y) == evPararEne && Direccion != Abajo) ||
			   (Nivel->Eventos(X, Y) == evSoloParriba && 
			    Direccion == Abajo &&
				!(Y * 30 > ImagenRobotsY - 30 &&
				  Y * 30 < ImagenRobotsY + 59)))
			{
                if (Direccion != Abajo && Sirve) // No queremos 2 intercambios...
                {
                    if (Sentido == Derecha)
                        Sentido = Izquierda;
                    else
                        Sentido = Derecha;
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
				if (ImagenRobotsX > LlegadaX)
				{
					A = LlegadaX;
					B = ImagenRobotsX;
				}
				else
				{
					A = ImagenRobotsX;
					B = LlegadaX;
				}
				ExactoH = (CorreccionH = ((long)(A / 30) + 1) * 30) < B;
				break;
			case Arriba:
			case Abajo:
				ExactoH = false;
				if (ImagenRobotsY > LlegadaY)
				{
					A = LlegadaY;
					B = ImagenRobotsY;
				}
				else
				{
					A = ImagenRobotsY;
					B = LlegadaY;
				}
				ExactoV = (CorreccionV = ((long)(A / 30) + 1) * 30) < B;
				break;
            default:
                break;
		}
        ImagenRobotsX = LlegadaX;
        ImagenRobotsY = LlegadaY;
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
					ViejoValor        = ImagenRobotsX;
					ImagenRobotsX    = CorreccionH;
					if (!MoverRobots(Direccion,Velocidad))
						ImagenRobotsX = ViejoValor;
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
					ViejoValor		  = ImagenRobotsY;
					ImagenRobotsY    = CorreccionV;
					if (!MoverRobots(Direccion,Velocidad))
						ImagenRobotsY = ViejoValor;
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

bool Robots::ColisionTortuga()
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
		SetRect(&R2,(long)ImagenRobotsX     ,(long)ImagenRobotsY,
			        (long)ImagenRobotsX + 60,(long)ImagenRobotsY + 60);
		if (!AuxPunta->EstoyMuerto && AuxPunta->Resbalando &&
			ColisionGeneral(R1,R2,imglstEnemigos[AuxPunta->ImagenTortugaPicture],imglstJefes[ImagenRobotsPicture]))
		{
			PlayWav(sndCaida);
            Puntaje += PuntajeMuerteTortuga * Multiplicador;
            ObjetoBase *CartelPuntaje = new Cartel();
			Cartel	   *AuxCartel	  = (Cartel *)CartelPuntaje;
			char	   Cadena[MAX_LONG_CARTEL];	
			ltoa(PuntajeMuerteTortuga * Multiplicador,Cadena);
            AuxCartel->Inicializar(AuxPunta->ImagenTortugaX, AuxPunta->ImagenTortugaY, Cadena, RGB(255,255,255));
            Carteles->Agregar(CartelPuntaje);
            AuxPunta->EstoyMuerto = true;
			AuxPunta->ImagenTortugaPicture = TortugaRev;
            AuxPunta->VY = -SaltoTortugaImpacto;
			return true;
		}
        Cantidad++;
    }
	return false;
}

bool Robots::ColisionBolaFuego()
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
		SetRect(&R2,(long)ImagenRobotsX     ,(long)ImagenRobotsY,
			        (long)ImagenRobotsX + 60,(long)ImagenRobotsY + 60);
		if (ColisionGeneral(R1,R2,imglstPremios[prBolaFuego],imglstJefes[ImagenRobotsPicture]))
		{
			EBF = true;
			CoordenadaCasilla((long)AuxPunta->MX + 7,(int)AuxPunta->MY + 7,EBFX,EBFY);
			return true;
		}
        Cantidad++;
    }
	return false;
}

bool Robots::ColisionRayoEnergia()
{
	long Cantidad, Total;
	Rayo *AuxPunta;
	Cantidad = 0;
    Total    = Rayos->Tamano();
	Rayos->SelPrimero();
    while (Cantidad < Total)
	{
        AuxPunta = ((Rayo *)Rayos->Avanzar());
        if (AuxPunta->MR)
        {
            RECT R1,R2;
            SetRect(&R1,(long)AuxPunta->MX     ,(long)AuxPunta->MY,
                        (long)AuxPunta->MX + 24,(long)AuxPunta->MY + 8);
            SetRect(&R2,(long)ImagenRobotsX     ,(long)ImagenRobotsY,
                        (long)ImagenRobotsX + 60,(long)ImagenRobotsY + 60);
            if (ColisionGeneral(R1,R2,imglstEnergia[enerRayo],imglstJefes[ImagenRobotsPicture]))
            {
                ERE = true;
                CoordenadaCasilla((long)AuxPunta->MX + 12,(int)AuxPunta->MY + 4,EREX,EREY);
                return true;
            }
        }
        Cantidad++;
    }
	return false;
}

bool Robots::RevisarMuerte()
{
	if (ColisionBolaFuego())
		PlayWav(sndMetalRebote); 

	if (Intermitente <= 0 && ColisionRayoEnergia())
    {
		if (GolpesRestantes > 0)
		{
			PlayWav(sndGolpeRobots);
			GolpesRestantes--;
			Intermitente = TiempoIntermitenciaRobots;
		}
		else
		{
			PlayWav(sndCaida);
	        ImagenRobotsPicture = RobotsRev;
		    Puntaje += PuntajeMuerteRobots * Multiplicador;
			ObjetoBase *CartelPuntaje = new Cartel();
			Cartel	   *AuxCartel	  = (Cartel *)CartelPuntaje;
			char	   Cadena[MAX_LONG_CARTEL];
			ltoa(PuntajeMuerteRobots * Multiplicador,Cadena);
	        AuxCartel->Inicializar(ImagenRobotsX, ImagenRobotsY, Cadena,RGB(255,255,255));
		    Carteles->Agregar(CartelPuntaje);
			EstoyMuerto = true;
			VY     = -SaltoRobotsImpacto;
		    return true;
		}
    }

    if (Intermitente <= 0 && ColisionTortuga())
    {
		if (GolpesRestantes > 2)
		{
			PlayWav(sndGolpeRobots);
			GolpesRestantes -= 3;
			Intermitente = TiempoIntermitenciaRobots;
		}
		else
		{
			PlayWav(sndCaida);
	        ImagenRobotsPicture = RobotsRev;
		    Puntaje += PuntajeMuerteRobots * Multiplicador;
			ObjetoBase *CartelPuntaje = new Cartel();
			Cartel	   *AuxCartel	  = (Cartel *)CartelPuntaje;
			char	   Cadena[MAX_LONG_CARTEL];
			ltoa(PuntajeMuerteRobots * Multiplicador,Cadena);
	        AuxCartel->Inicializar(ImagenRobotsX, ImagenRobotsY, Cadena,RGB(255,255,255));
		    Carteles->Agregar(CartelPuntaje);
			EstoyMuerto = true;
			VY     = -SaltoRobotsImpacto;
		    return true;
		}
    }
	RECT R;
	SetRect(&R,(long)ImagenRobotsX     ,(long)ImagenRobotsY,
		       (long)ImagenRobotsX + 60,(long)ImagenRobotsY + 60);
	if ((!Nicho->AuraEscudo) && ColisionaNicholas(R,imglstJefes[ImagenRobotsPicture]) != ColNula)
		Hitazo = true;
    return false;
}
