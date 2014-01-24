// Este archivo define la clase Explosion, que maneja una explosión
// de bloque (más que una explosión, podemos decir que lo que esta
// clase maneja son los 4 pedazos de bloque que salen volando).

// Evitamos doble inclusión.

#ifndef INC_EXPLO

#define INC_EXPLO

// Incluimos las librerías necesarias.

#include "InfoObj.h"
#include "Colec.h"
#include "VarJuego.h"
#include "DDInc.h"

// Definimos la clase Explosion.

class Explosion : public ObjetoBase
{
public:
	// Constructor nulo: el objeto se inicializará mediante
	// la función inicializar().
	Explosion() {}
	// Funciones propias del objeto.
	void Inicializar(int CX, int CY);
	virtual void Mostrar() 
	{
		BufferPaint((long)Pedazo1X - Pos, (long)Pedazo1Y, imglstExplosiones[NumeroImagen],15,15);
		BufferPaint((long)Pedazo2X - Pos, (long)Pedazo2Y, imglstExplosiones[NumeroImagen],15,15);
		BufferPaint((long)Pedazo3X - Pos, (long)Pedazo3Y, imglstExplosiones[NumeroImagen],15,15);
		BufferPaint((long)Pedazo4X - Pos, (long)Pedazo4Y, imglstExplosiones[NumeroImagen],15,15);
	}
	virtual void Check();
	void Matarme() {MuerteReciente = true;}
private:
	int    NumeroImagen;
	double Pedazo1X, Pedazo1Y;
	double Pedazo2X, Pedazo2Y;
	double Pedazo3X, Pedazo3Y;
	double Pedazo4X, Pedazo4Y;
	double VXS , VXI;
	double VYS , VYI;
};

#endif // INC_EXPLO