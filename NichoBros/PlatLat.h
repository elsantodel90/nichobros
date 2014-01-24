// Este archivo de cabecera define la clase PlatLat, que maneja
// a dichos objetos. Una plataforma lateral inicia con velocidad
// y igual a 0, y luego se comporta siguiendo un movimiento armónico simple en el sentido de
// las x, centrado en el punto de partida.

// Evitamos doble inclusión.

#ifndef INC_PLATLAT

#define INC_PLATLAT

// Incluimos las librerías necesarias.

#include "Nicholas.h"
#include "Colec.h"
#include "InfoObj.h"
#include "DDInc.h"
#include "VarJuego.h"

// Definimos la clase.

class PlatLat : public ObjetoBase
{
public:
	// Constructor vacío!
	PlatLat() {}
	// Funciones de la clase...
	void Inicializar(int CX1, int CY1, bool horiz);
	virtual void Mostrar() {BufferPaint((long)(MX - Pos),(long)MY,imglstPlataformas[Horiz?platCyan:platVerde],90,8);}
	virtual void Check();
	void Matarme() {MuerteReciente = true;}
private:
	bool Activo;
	bool Engancho;
	bool Horiz;
    long ultTiempoEnganche;

	double MX, MY;
	double MCB;
	long   Tiempo;
};

#endif // INC_PLATLAT
