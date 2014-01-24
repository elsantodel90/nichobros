// En este archivo declaramos la clase cartel, utilizada para
// manejar los carteles en movimiento que desaparecen en pocos
// segundos y muestran, casi siempre, el puntaje obtenido de una
// acción.

// Evitamos la doble inclusión

#ifndef INC_CARTEL

#define INC_CARTEL

// Incluimos librerías necesarias.

#include "DDInc.h"
#include "Colec.h"
#include "InfoObj.h"
#include "VarJuego.h"

// Definimos constantes útiles.

#define MAX_LONG_CARTEL 100

// Definimos la clase cartel.

class Cartel : public ObjetoBase
{
public:
	// Constructor
	Cartel() {} // El constructor no hace nada! Todo está en inicializar.
	// Funciones de la clase.
	void Inicializar(double X, double Y, const char *Mensaje, COLORREF MiColor);
	virtual void Mostrar() {BufferPaint((long)MX - Pos+3,(long)MY+3,lpDDSCartelSombra,lpDDSCartel->w,lpDDSCartel->h);
                            BufferPaint((long)MX - Pos,(long)MY,lpDDSCartel,lpDDSCartelSombra->w,lpDDSCartelSombra->h);}
	virtual void Check();
	void Matarme() 
	{
		MuerteReciente = true;
		if (lpDDSCartel != NULL)
		{
            SDL::freeSurface(lpDDSCartel);
			lpDDSCartel = NULL;
		}
	}
private:
	double MX;
	double MY;
	double OY;
	LPDIRECTDRAWSURFACE lpDDSCartel;
    LPDIRECTDRAWSURFACE lpDDSCartelSombra;
};

#endif // INC_CARTEL
