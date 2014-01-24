// Este archivo de cabecera define la clase Triturin, que maneja
// a dichos objetos. Un Triturin es levantado por una cuerda, y luego
// cae violentamente, aplastando lo que pueda haber bajo el mismo.

// Evitamos doble inclusión.

#ifndef INC_TRITURIN

#define INC_TRITURIN

// Incluimos las librerías necesarias.

#include "Colec.h"
#include "InfoObj.h"
#include "DDInc.h"
#include "VarJuego.h"
#include "PuenCae.h"

// Definiciones utiles para la clase...

#define ColorCuerdaTriturin 0xDD22

// Definimos la clase.

class Triturin : public ObjetoBase
{
friend class Nicholas;
public:
	// Constructor vacío!
	Triturin() {}
	// Funciones de la clase...
	void Inicializar(int X, int Y);
	bool HuboChoque();
	virtual void Mostrar() 
	{
		RECT J;
		if (CuantoAfuera <= 90)
		{
			J.top    = 90 - (long)CuantoAfuera;
			J.left   = 0;
			J.right  = 30;
			J.bottom = 90;
			BltFast(SDL::screen(),(long)(MX - Pos), (long)MY, imglstCosasMalas[cmTriturin], &J);
		}
		else
		{
			J.top	 = (long)MY;
			J.left	 = (long)(MX - Pos + 12);
			J.right  = (long)(MX - Pos + 13);
			J.bottom = (long)(MY + CuantoAfuera - 90);
			DrawRectangle(SDL::screen(),&J,i2col(ColorCuerdaTriturin));
			J.left	 = (long)(MX - Pos + 14);
			J.right  = (long)(MX - Pos + 16);
			DrawRectangle(SDL::screen(),&J,i2col(ColorCuerdaTriturin));
			J.left	 = (long)(MX - Pos + 17);
			J.right  = (long)(MX - Pos + 18);
			DrawRectangle(SDL::screen(),&J,i2col(ColorCuerdaTriturin));
			BufferPaint((long)(MX - Pos),(long)(MY + CuantoAfuera - 90),imglstCosasMalas[cmTriturin],30,90);
		}
	}
	virtual void Check();
	void Matarme() {MuerteReciente = true;}
private:
    void CheckCol();
	bool   Activo;
	double CuantoAfuera; // Numero de pixeles afuera. :-)
	long   Falta, Falta2;
	double MX, MY;
	int    MCX,MCY;
	int    MCY2;
	double VY;
};

#endif // INC_TRITURIN
