// Este archivo define la clase Secuencia, que maneja una animación
// cuadro a cuadro. Esta clase genérica resulta extremadamente útil,
// por ejemplo, para producir explosiones.

// Evitamos doble inclusión.

#ifndef INC_SECUEN

#define INC_SECUEN

// Incluimos las librerías necesarias.

#include "InfoObj.h"
#include "Colec.h"
#include "VarJuego.h"
#include "DDInc.h"

// Definimos la clase Explosion.

class Secuencia : public ObjetoBase
{
public:
	// Constructor nulo: el objeto se inicializará mediante
	// la función inicializar().
	Secuencia() {}
	// Funciones propias del objeto.
	void Inicializar(long X, long Y, long CAn, long CAl, long TI, long AnFila, LPDIRECTDRAWSURFACE S);
	virtual void Mostrar() 
	{
		MX = MX - Pos;
		RECT J; 
		long Resta;
	
		// Clipping izquierdo
		if (MX < 0)
		{
			J.left = (NumeroImagen % AnchoFila) * CuadroAncho + abs(MX);
			MX = 0;
		}
		else
			J.left = (NumeroImagen % AnchoFila) * CuadroAncho;
		// Clipping superior
		if (MY < 0)
		{
			J.top = (NumeroImagen / AnchoFila) * CuadroAlto + abs(MY);
			MY = 0;
		}
		else
		    J.top = (NumeroImagen / AnchoFila) * CuadroAlto;
		// Clipping Derecho
		Resta = MX + CuadroAncho - 800;
		if (Resta > 0)
			J.right = (NumeroImagen % AnchoFila) * CuadroAncho + CuadroAncho - Resta;
		else
			J.right = (NumeroImagen % AnchoFila) * CuadroAncho + CuadroAncho;
		// Clipping Inferior
		Resta = MY + CuadroAlto - 600;
		if (Resta > 0)
			J.bottom = (NumeroImagen / AnchoFila) * CuadroAlto + CuadroAlto - Resta;
		else
			J.bottom = (NumeroImagen / AnchoFila) * CuadroAlto + CuadroAlto;
		// Finalmente, dibujamos en el backbuffer
		BltFast(SDL::screen(),MX, MY, Surf, &J);
		MX = MX + Pos;
	}
	virtual void Check();
	void Matarme() {MuerteReciente = true;}
private:
	int  NumeroImagen;
	int  AnchoFila;
	int  NImagen;
	long CuadroAncho, CuadroAlto;
	long MX, MY;

	LPDIRECTDRAWSURFACE Surf;
};

#endif // INC_SECUEN
