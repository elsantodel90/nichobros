// En este archivo de cabecera se define la clase Rebotador. Esta
// clase maneja a los rebotadores: estos son objetos con un resorte
// y una plataforma, que le dan a nuestro protagonista la capacidad
// para saltar más alto de lo normal.

// Evitamos doble inclusion...

#ifndef INC_REBOTER

#define INC_REBOTER

// Incluimos librerías necesarias...
#include "DDInc.h"
#include "VarJuego.h"
#include "Colec.h"
#include "Nicholas.h"

// Definimos la clase Rebotador.

class Rebotador : public ObjetoBase
{
public:
	// Constructor vacio. Inicializamos con inicializar().
	Rebotador() {}
	// Funciones de la clase.
	void Inicializar(int CX, int CY);
	virtual void Mostrar() {BufferPaint(MX - Pos,MY,imglstRebotador[NumeroImagen],30,30);}
	virtual void Check();
	void Matarme() {MuerteReciente = true;}
private:
	int  NumeroImagen;
	long MX;
	int  MY, MCX,MCY;
};

#endif // INC_REBOTER