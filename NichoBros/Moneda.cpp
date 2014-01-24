// En este archivo implementamos las funciones más complejas de la
// clase moneda, definiendo así su comportamiento.

// Incluímos las librerías necesarias...

#include "RutinasSDL/rutinasSDL.h"
#include "Moneda.h"

// Funciones de la clase

void Moneda::Inicializar(double X, double Y)
{
    MY = Y - 10; // Distancia prudencial...
    MX = X;
	VY = VelocidadMoneda;
	// Sonido!
	PlayWav(sndMoneda);
}

void Moneda::Check()
{
	MY -= VY * Delta - Delta * Delta * AceleracionGravedad / 2;
	VY -= Delta * AceleracionGravedad;
    if (VY < -AceleracionGravedad * 50)
	{
        // Terminamos la existencia.
        Matarme();
		// Creamos un cartelito!
		ObjetoBase *NuevoCartel = new Cartel();
		Cartel	   *ElCartel	= (Cartel *)NuevoCartel;
		ElCartel->Inicializar(MX,MY,MENSAJE_GANAR_MONEDA,RGB(255,255,0));
		Carteles->Agregar(NuevoCartel);
	}
}
