// En este archivo se implementan las funciones más complejas de
// la clase explosión.

// Incluimos las librerías.

#include <cmath>
#include "RutinasSDL/rutinasSDL.h"
#include "Explo.h"

// Implementamos las funciones de la clase.

void Explosion::Inicializar(int CX, int CY)
{
    NumeroImagen = 0;
    VXS	= cos(AnguloSuperiores * Pi / 180) * VelocidadExplosion;
    VXI = cos(AnguloInferiores * Pi / 180) * VelocidadExplosion;
    VYS = sin(AnguloSuperiores * Pi / 180) * VelocidadExplosion;
    VYI = sin(AnguloInferiores * Pi / 180) * VelocidadExplosion;
    long X;
	int  Y;
    CasillaCoordenada(X, Y, CX, CY);
    // Superior izquierdo
    Pedazo1X = X;
    Pedazo1Y = Y;
    // Superior derecho
    Pedazo2X = X + 15;
    Pedazo2Y = Y;
    // Inferior izquierdo
    Pedazo3X = X;
    Pedazo3Y = Y + 15;
    // Inferior derecho
    Pedazo4X = X + 15;
    Pedazo4Y = Y + 15;
}

void Explosion::Check()
{
	if (Ciclo % 2 == 0)
		NumeroImagen++;
	if (NumeroImagen >= 4)
        NumeroImagen = 0;
	double Delta2Acel = Delta * Delta * AceleracionGravedad / 2;
	double CambioEspacioYS = VYS * Delta - Delta2Acel;
	double CambioEspacioYI = VYI * Delta - Delta2Acel;
	// Superiores
    Pedazo1X -= VXS * Delta;
    Pedazo2X += VXS * Delta;
    Pedazo1Y -= CambioEspacioYS;
    Pedazo2Y -= CambioEspacioYS;
    // Inferiores
    Pedazo3X -= VXI * Delta;
    Pedazo4X += VXI * Delta;
    Pedazo3Y -= CambioEspacioYI;
    Pedazo4Y -= CambioEspacioYI;
	VYI -= AceleracionGravedad * Delta;
	VYS -= AceleracionGravedad * Delta;
	if (Pedazo1Y > 600 && Pedazo2Y > 600 && 
		Pedazo3Y > 600 && Pedazo4Y > 600)
                    Matarme();
}
