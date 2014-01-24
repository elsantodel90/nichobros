// Este archivo de cabecera define la clase PinchePalo, que maneja
// a dichos objetos. Un Pinche en forma de palo sale y entra de una
// casilla, y si colisiona con Nicholas lo da�a. Es indestructible.

// Evitamos doble inclusi�n.

#ifndef INC_PINPALO

#define INC_PINPALO

// Incluimos las librer�as necesarias.

#include "Colec.h"
#include "InfoObj.h"
#include "DDInc.h"
#include "VarJuego.h"

// Definimos la clase.

class PinchePalo : public ObjetoBase
{
public:
	// Constructor vac�o!
	PinchePalo() {}
	// Funciones de la clase...
	void Inicializar(int X, int Y);
	virtual void Mostrar() 
	{
		// Recordar que el sprite tiene:
		// ANCHO = 22
		// ALTO  = 57

		BufferPaint((long)(MX - Pos),(long)(MY - CuantoAfuera),imglstCosasMalas[cmPinchePalo],22,(long)CuantoAfuera);
	}
	virtual void Check();
	void Matarme() {MuerteReciente = true;}
private:
	bool   Activo;
	double CuantoAfuera; // Numero de pixeles afuera. :-)
	long   Falta;
	double MX, MY;
};

#endif // INC_PINPALO