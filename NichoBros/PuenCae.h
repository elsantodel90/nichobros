// Este archivo de cabecera define la clase PuenteCae, que maneja
// a dichos objetos. Un puente que cae inicia con velocidades
// x e y iniciales e iguales a 0, y luego se comporta como 
// cualquier cuerpo: sigue un movimiento uniformemente variado 
// para las y, y no se mueve en las x 
// (tiro vertical sin rozamiento).

// Evitamos doble inclusión.

#ifndef INC_PUENTECAE

#define INC_PUENTECAE

// Incluimos las librerías necesarias.

#include "Colec.h"
#include "InfoObj.h"
#include "DDInc.h"
#include "VarJuego.h"

// Definimos la clase.

class PuenteCae : public ObjetoBase
{
public:
	// Constructor vacío!
	PuenteCae() {}
	// Funciones de la clase...
	void Inicializar(int CX, int CY, long MiRetardo);
	virtual void Mostrar() {BufferPaint((long)(MX - Pos),(long)MY,MiSuperficie,30,30);}
	virtual void Check();
	void Matarme() {MuerteReciente = true;}
private:
	double				MX, MY;
	double				VY;
	int					MCX, MCY;
	long				TiempoRestante;
	LPDIRECTDRAWSURFACE MiSuperficie;
};

#endif // INC_PUENTECAE