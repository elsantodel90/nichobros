// En este archivo se implementan las funciones más complejas de
// la clase Rayo.

// Incluimos librerías...

#include "RutinasSDL/rutinasSDL.h"
#include "Rayo.h"

// Funciones de la clase.

void Rayo::Inicializar(double X, double Y, double iVX,bool iMN, bool iMR)
{
	MX = X;
	MY = Y;
	VX = iVX;
	MN = iMN;
    MR = iMR;

	GolpesRestantes = GolpesRayoEnergia;
}

void Rayo::Check()
{
	if (Pegamos())
	{
		ERE = false;
		if (GolpesRestantes > 0)
			GolpesRestantes--;
		else
			Matarme();
		return;
	}
	RECT R;
	SetRect(&R,(long)MX     ,(long)MY,
		       (long)MX + 24,(long)MY + 8);
	if (MN && ColisionaNicholas(R,imglstEnergia[enerRayo]) != ColNula)
	{
		Hitazo = true; // Metodo practico para asar a Nicholas. :-)
		Matarme();
		return;
	}
	// La velocidad x es constante: realizamos el movimento x.
	if (!MoverRayo(Derecha,VX * Delta) ||
		MY > 600 || MX < Pos - 300)
	{
		Matarme();
		return;
	}
}

bool Rayo::MoverRayo(eDireccion Direccion, double Velocidad)
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
		if (!MoverRayo(Direccion,MaximoMovimiento))
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
    CoordenadaCasilla((long)LlegadaX + 23, (int)LlegadaY + 7, X2, Y2);
    Sirve = true;
    for (X = X1; X <= X2; X++)
        for (Y = Y1; Y <= Y2; Y++)
          {
            if (Nivel->Eventos(X, Y) == evPisoFragil)
			{
				PlayWav(sndRupturaBloque);
				Nivel->SetEventos(X, Y, evExplotando);
				// Modificamos los valores del nivel.
				Nivel->SetTerreno(X, Y, caVacia);
				// Creamos una explosion.
				ObjetoBase *LaExplosion  = new Explosion();
				((Explosion*)LaExplosion)->Inicializar(X, Y);
				Explosiones->Agregar(LaExplosion);
				return false;
			}
			else if (Nivel->Eventos(X, Y) == evPisoRigido	   ||
					 Nivel->Eventos(X, Y) == evPinchePalo	   ||
					 Nivel->Eventos(X, Y) == evZonaDueleRigido ||
					 Nivel->Eventos(X, Y) == evBloqueMoneda	   ||
					(Nivel->Eventos(X, Y) == evSoloParriba &&
					 !(Y * 30 > MY - 30 &&
					 Y * 30 < MY + 14)))
                return false;
          }
    // Listo el for, ahora a trabajar!
    if (Sirve)
	{
        MX = LlegadaX;
        MY = LlegadaY;
    }
	return Sirve;
}

bool Rayo::Pegamos()
{
	if (ERE)
	{
		int X,  Y;
		CoordenadaCasilla((long)MX + 12, (int)MY + 4, X, Y);
		return X == EREX && Y == EREY;
	}
	else
		return false;
}
