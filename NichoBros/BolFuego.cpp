// En este archivo se implementan las funciones más complejas de
// la clase BolaFuego.

// Incluimos librerías...

#include "RutinasSDL/rutinasSDL.h"
#include "BolFuego.h"

// Funciones de la clase.

void BolaFuego::Inicializar(double X, double Y, double iVX, double iVY, bool MN)
{
	MX		 = X;
	MY		 = Y;
	VX		 = iVX;
	VY		 = iVY;
	NRebotes = RebotesBolaFuego;
	MataNicholas = MN;
}

void BolaFuego::Check()
{
	if (Pegamos())
	{
		EBF = false;
		Matarme();
		return;
	}
	// La velocidad x es constante: realizamos el movimento x.
	if (!MoverBolaFuego(Derecha,VX * Delta) ||
		MY > 600 || MX < Pos - 300)
	{
		Matarme();
		return;
	}
	// Ahora realizamos el movimiento y, que es uniformemente
	// variado, lo que lo hace un poco más complejo.
	MoverBolaFuego(Abajo,- VY * Delta + Delta * Delta * AceleracionGravedad / 2);
	VY -= Delta * AceleracionGravedad;
	//  Y ahora vamos a hacer que nicholas se queme, cosa que se fría. Eso claro, si
	// no fue nicholas quien tiró la bola de fuego... Controlemos eso.
	if (MataNicholas)
	{
		RECT R;
		SetRect(&R,(long)MX,(long)MY,(long)MX + 15,(long)MY + 15);
		if (ColisionaNicholas(R,imglstPremios[prBolaFuego]) != ColNula)
			Hitazo = true;
	}
}

bool BolaFuego::MoverBolaFuego(eDireccion Direccion, double Velocidad)
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
		if (!MoverBolaFuego(Direccion,MaximoMovimiento))
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
    CoordenadaCasilla((long)LlegadaX + 14, (int)LlegadaY + 14, X2, Y2);
    Sirve = true;
    for (X = X1; X <= X2; X++)
        for (Y = Y1; Y <= Y2; Y++)
            if (Nivel->Eventos(X, Y) == evPisoFragil	  || 
				Nivel->Eventos(X, Y) == evPisoRigido	  ||
				Nivel->Eventos(X,Y) == evPinchePalo		  ||
				Nivel->Eventos(X, Y) == evZonaDueleRigido ||
				Nivel->Eventos(X, Y) == evBloqueMoneda	  ||
				(Nivel->Eventos(X, Y) == evSoloParriba &&
				!(Y * 30 > MY - 30 &&
				  Y * 30 < MY + 14)))
			{
                if (Direccion != Abajo)
					if (NRebotes-- > 0)
					{
						VX = -VX;
						return true;
					}
					else
						return false;
				else
				{
					// Saltito!
					VY = VelocidadReboteBolaFuego;
					return true;
				}
			}
    // Listo el for, ahora a trabajar!
    if (Sirve)
	{
        MX = LlegadaX;
        MY = LlegadaY;
    }
	return Sirve;
}

bool BolaFuego::Pegamos()
{
	if (EBF)
	{
		int X,  Y;
		CoordenadaCasilla((long)MX + 7, (int)MY + 7, X, Y);
		return X == EBFX && Y == EBFY;
	}
	else
		return false;
}
