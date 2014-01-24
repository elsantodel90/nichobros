// Este archivo de cabecera define la clase PlatCae, que maneja
// a dichos objetos. Una plataforma que cae inicia con velocidades
// x e y iniciales e iguales a 0, y luego se comporta como 
// cualquier cuerpo: sigue un movimiento uniformemente variado 
// para las y, y no se mueve en las x 
// (tiro vertical sin rozamiento).

// Evitamos doble inclusión.

#ifndef INC_PLATCAE

#define INC_PLATCAE

// Incluimos las librerías necesarias.

#include "Nicholas.h"
#include "Colec.h"
#include "InfoObj.h"
#include "DDInc.h"
#include "VarJuego.h"

// Definimos la clase.

class PlatCae : public ObjetoBase
{
public:
	// Constructor vacío!
	PlatCae() {}
	// Funciones de la clase...
	void Inicializar(int CX1, int CY1);
	virtual void Mostrar() {BufferPaint((long)(MX - Pos),(long)MY,imglstPlataformas[platRoja],90,8);}
	virtual void Check();
	void Matarme() {MuerteReciente = true;}
private:
	bool Activo;

	int    MCX,MCY;
	double MX, MY;
	double VY;
	long   TiempoRestante;
};

#endif // INC_PLATCAE