// En este archivo implementamos la clase cartel, utilizada para
// manejar los carteles en movimiento que desaparecen en pocos
// segundos y muestran, casi siempre, el puntaje obtenido de una
// acción.

// Incluímos las librerías necesarias...

#include "RutinasSDL/rutinasSDL.h"
#include "Cartel.h"

void Cartel::Inicializar(double X, double Y, const char *Mensaje,COLORREF MiColor)
{
    MY = OY = Y - 10; // Distancia prudencial...
    MX = X;
    
    lpDDSCartel       = SDL::renderTextSolid(hFuente,Mensaje,MiColor);
    lpDDSCartelSombra = SDL::renderTextSolid(hFuente,Mensaje,RGB(10,10,10));
}

void Cartel::Check()
{
    if (OY - MY <= ElevacionCartel)
        MY -= VelocidadCartel * Delta;
    else
        // Terminamos la existencia.
        Matarme();
}
