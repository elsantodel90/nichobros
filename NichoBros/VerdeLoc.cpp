// En este archivo de implementan las funciones más complejas de
// la clase VerdeLoco.

// Incluimos las librerías necesarias.

#include "RutinasSDL/rutinasSDL.h"
#include "VerdeLoc.h"

// Implementamos las funciones.

void VerdeLoco::Inicializar(int CX, int CY)
{
	SV2					 = 0;
	ImagenVerdeAnimacion = 7;
	SuperficieVerde = imglstGreenRare[ImagenVerdeAnimacion];
    long X;
	int  Y;
    CasillaCoordenada(X, Y, CX, CY);
    ImagenVerdeY   = Y;
    ImagenVerdeX   = X;
    EstoyMuerto	   = false;
    Activo		   = false;
	VY			   = 0;
	DeltaAcumulado = 0;
	TiempoFlotar   = 0;
}

void VerdeLoco::Check()
{
    if (ImagenVerdeY > 600 || ImagenVerdeX - Pos < -400)
	{
        Matarme();
        return;
    }
    if (!Activo)
    {
        if (ImagenVerdeX - Pos < 800)
            Activo = true;
        else
            return;
    }
    if (EstoyMuerto)
	{
        ImagenVerdeY += VY * Delta + Delta * Delta * AceleracionGravedad / 2;
        VY += AceleracionGravedad * Delta;
        return;
    }
    if (RevisarMuerte())
	{
        ImagenVerdeY -= 120;
		TiempoFlotar = TiempoVerdeFlota;
		SV2			 = 0;
		VY			 = 0;
        return;
    }
	if (labs((long)Nicho->ImagenNicholasX - (long)ImagenVerdeX) < DistanciaReaccionVerde)
		if (ImagenVerdeAnimacion > 0)
		{
			DeltaAcumulado += Delta;
			if (DeltaAcumulado / TiempoCambioVerdeFrame <= ImagenVerdeAnimacion)
				ImagenVerdeAnimacion -= DeltaAcumulado / TiempoCambioVerdeFrame;
			else
				ImagenVerdeAnimacion = 0;
			DeltaAcumulado = DeltaAcumulado % TiempoCambioVerdeFrame;
			SuperficieVerde = imglstGreenRare[ImagenVerdeAnimacion];
		}
		else
			DeltaAcumulado = 0;
	else 
		if (ImagenVerdeAnimacion < 7)
		{
			DeltaAcumulado += Delta;
			if (DeltaAcumulado / TiempoCambioVerdeFrame <= 7 - ImagenVerdeAnimacion)
				ImagenVerdeAnimacion += DeltaAcumulado / TiempoCambioVerdeFrame;
			else
				ImagenVerdeAnimacion = 7;
			DeltaAcumulado = DeltaAcumulado % TiempoCambioVerdeFrame;
			SuperficieVerde = imglstGreenRare[ImagenVerdeAnimacion];
		}
		else
			DeltaAcumulado = 0;
	if (EstoyMuerto)
        return;
	if (TiempoFlotar != 0)
	{
		if (TiempoFlotar >= Delta)
			TiempoFlotar -= Delta;
		else
			TiempoFlotar = 0;
		MoverVerde(Abajo, 0); // Parece loco: pero no lo es, ya que
		// chequeos muy importantes son realizados dentro de esa funcion.
		return;
	}
    MoverVerde(Abajo, VY * Delta + Delta * Delta * AceleracionGravedad / 2);
	VY += AceleracionGravedad * Delta;
}

bool VerdeLoco::MoverVerde(eDireccion Direccion, double Velocidad)
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
		if (!MoverVerde(Direccion,MaximoMovimiento))
			return false;
		// Ultimo detalle: si alguno de los sub-movimientos falla,
		// devolvemos falso. Esto gana velocidad, ya que no siempre
		// es necesario chequear todos los movimientos.
	}
    double LlegadaX, LlegadaY;
    switch (Direccion)
	{
		case Derecha:
		    LlegadaX = ImagenVerdeX + Velocidad;
			LlegadaY = ImagenVerdeY;
			break;
		case Izquierda:
			LlegadaX = ImagenVerdeX - Velocidad;
			LlegadaY = ImagenVerdeY;
			break;
		case Abajo:
			LlegadaX = ImagenVerdeX;
			LlegadaY = ImagenVerdeY + Velocidad;
			break;
		case Arriba:
			LlegadaX = ImagenVerdeX;
			LlegadaY = ImagenVerdeY - Velocidad;
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
				Nivel->Eventos(X, Y) == evZonaDueleRigido ||
				Nivel->Eventos(X,Y) == evPinchePalo		  ||
				Nivel->Eventos(X, Y) == evPuenteFlojo	  ||
				Nivel->Eventos(X, Y) == evPuenteCae 	  ||
				Nivel->Eventos(X,Y) == evBloqueMoneda     ||
			   (Nivel->Eventos(X,Y)  == evSoloParriba && 
			    Direccion == Abajo &&
				!(Y * 30 > ImagenVerdeY - 30 &&
				  Y * 30 < ImagenVerdeY + 29)))
                Sirve = false;
            else if (Nivel->Eventos(X, Y) == evExplotando)
                if (Direccion == Abajo)
				{
                    // Tenemos que morir: nos dieron en
                    // el piso donde estábamos.
					PlayWav(sndCaida);
                    Puntaje += PuntajeMuerteVerde * Multiplicador;
					ObjetoBase *CartelPuntaje = new Cartel();
					Cartel	   *AuxCartel	  = (Cartel * )CartelPuntaje;
					char	   Cadena[MAX_LONG_CARTEL];
					ltoa(PuntajeMuerteVerde * Multiplicador,Cadena);
                    AuxCartel->Inicializar(ImagenVerdeX, ImagenVerdeY, Cadena,RGB(255,255,255));
                    Carteles->Agregar(CartelPuntaje);
                    Multiplicador++;
                    EstoyMuerto = true;
					SuperficieVerde = imglstEnemigos[GreenRareRev];
                    VY = -SaltoVerdeImpacto;
                    return false;
				}
        }
    // Listo el for, ahora a trabajar!
    if (Sirve)
	{
        ImagenVerdeX = LlegadaX;
        ImagenVerdeY = LlegadaY;
    }
	else if (Direccion == Abajo)
		VY = 0;
	return Sirve;
}

bool VerdeLoco::ColisionTortuga()
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
			        (long)AuxPunta->ImagenTortugaX + 30,(long)AuxPunta->ImagenTortugaY + 30);
		SetRect(&R2,(long)ImagenVerdeX     ,(long)ImagenVerdeY,
			        (long)ImagenVerdeX + 30,(long)ImagenVerdeY + 30);
		if (AuxPunta->Resbalando &&
			ColisionGeneral(R1,R2,imglstEnemigos[AuxPunta->ImagenTortugaPicture],imglstGreenRare[0]))
			return true;
        Cantidad++;
    }
	return false;
}

bool VerdeLoco::ColisionBolaFuego()
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
		SetRect(&R2,(long)ImagenVerdeX     ,(long)ImagenVerdeY,
			        (long)ImagenVerdeX + 30,(long)ImagenVerdeY + 30);
		if (ColisionGeneral(R1,R2,imglstPremios[prBolaFuego],imglstGreenRare[0]))
		{
			EBF = true;
			CoordenadaCasilla((long)AuxPunta->MX + 7,(int)AuxPunta->MY + 7,EBFX,EBFY);
			return true;
		}
        Cantidad++;
    }
	return false;
}

bool VerdeLoco::ColisionRayoEnergia()
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
		SetRect(&R2,(long)ImagenVerdeX     ,(long)ImagenVerdeY,
			        (long)ImagenVerdeX + 30,(long)ImagenVerdeY + 30);
		if (ColisionGeneral(R1,R2,imglstEnergia[enerRayo],imglstGreenRare[0]))
		{
			ERE = true;
			CoordenadaCasilla((long)AuxPunta->MX + 12,(int)AuxPunta->MY + 4,EREX,EREY);
			return true;
		}
        Cantidad++;
    }
	return false;
}

bool VerdeLoco::RevisarMuerte()
{
	eTiposCol AuxCol;
	RECT R;
	SetRect(&R,(long)ImagenVerdeX     ,(long)ImagenVerdeY,
		       (long)ImagenVerdeX + 30,(long)ImagenVerdeY + 30);
    if (ColisionTortuga()     ||
		ColisionBolaFuego()   ||
		ColisionRayoEnergia() ||
		(Nicho->AuraEscudo && 
		ColisionaNicholas(R,imglstGreenRare[0]) != ColNula))
	{
		PlayWav(sndCaida);
		SuperficieVerde = imglstEnemigos[GreenRareRev];
        Puntaje += PuntajeMuerteVerde * Multiplicador;
        ObjetoBase *CartelPuntaje = new Cartel();
		Cartel	   *AuxCartel	  = (Cartel *)CartelPuntaje;
		char	   Cadena[MAX_LONG_CARTEL];
		ltoa(PuntajeMuerteVerde * Multiplicador,Cadena);
        AuxCartel->Inicializar(ImagenVerdeX, ImagenVerdeY, Cadena,RGB(255,255,255));
        Carteles->Agregar(CartelPuntaje);
		EstoyMuerto   = true;
		VY			  = -SaltoVerdeImpacto;
		ImagenVerdeY += 120;
        return true;
	}
	else if ((AuxCol = ColisionaNicholas(R,imglstGreenRare[0])) == ColSuperior ||
			  AuxCol == ColSuperiorIz || AuxCol == ColSuperiorDe)
		return true;
	else if (AuxCol != ColNula)
		Hitazo = true;
    return false;
}
