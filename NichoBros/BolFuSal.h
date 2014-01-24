// Este archivo de cabecera define la clase BolaFuegoSalta, que 
// maneja a dichos objetos. Una bola de fuego que salta sale desde
// la parte inferior de la pantalla, sube y cae. Si toca al prota-
// gonista, caput. Sigue un movimiento uniformemente variado para 
// las y, y no se mueve en x (tiro vertical sin rozamiento).

// Evitamos doble inclusión.

#ifndef INC_BOLFUEGOSALTA

#define INC_BOLFUEGOSALTA

// Incluimos las librerías necesarias.

#include "Colec.h"
#include "InfoObj.h"
#include "DDInc.h"
#include "VarJuego.h"

// Definimos la clase.

class BolaFuegoSalta : public ObjetoBase
{
public:
	// Constructor vacío!
	BolaFuegoSalta() {}
	// Funciones de la clase...
	void Inicializar(int X, int Y,bool pez);
	virtual void Mostrar()
    {
        if (soypez)
            BufferPaint(MX - Pos,(long)MY,imglstEnemigos[(VY >= 0)?(36):(35)],30,30);
        else
            BufferPaint(MX - Pos,(long)MY,imglstFuego[(VY >= 0)?(fuegArribaPico):(fuegAbajoPico)],30,30);
    }
	virtual void Check();
	void Matarme() {MuerteReciente = true;}
private:
	double MY;
	long   MX;
	double VY, IVY;
	long   MTime;
	bool   Activo;
    bool soypez;
};

#endif // INC_BOLFUEGOSALTA
