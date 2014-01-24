// En este archivo de cabecera se define la clase Puerta. Esta
// clase maneja a las puertas: estos son objetos con una
// puerta, que obstruyen el camino de Nicholas y de los malos,
// hasta que nicholas las abra tocando un botón

// Evitamos doble inclusion...

#ifndef INC_PUERTA

#define INC_PUERTA

// Incluimos librerías necesarias...
#include "DDInc.h"
#include "VarJuego.h"
#include "Colec.h"
#include "Nicholas.h"

// Definimos la clase Rebotador.

class Puerta : public ObjetoBase
{
public:
	// Constructor vacio. Inicializamos con inicializar().
	Puerta() {}
	// Funciones de la clase.
	void Inicializar(int CX, int CY,bool Roja);
	virtual void Mostrar() 
	{
		BufferPaint((long)MX - Pos,(long)MY,imglstAparejos[MiImagen],30,90);
	}
	virtual void Check();
	void Matarme() {MuerteReciente = true;}
private:
	double MBY;
	double MX;
	double MY;
	double VY;
	int MCX,MCY;

	bool Activo;
	bool Rojo;

	eAparejos MiImagen;
};

#endif // INC_PUERTA