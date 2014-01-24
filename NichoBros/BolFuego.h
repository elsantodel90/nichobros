// Este archivo de cabecera define la clase BolaFuego, que maneja
// a dichos objetos. Una bola de fuego es arrojada con velocidades
// x e y iniciales, y luego se comporta como cualquier cuerpo:
// sigue un movimiento uniformemente variado para las y, y otro
// uniforme para las x (tiro oblicuo sin rozamiento).

// Evitamos doble inclusi�n.

#ifndef INC_BOLFUEGO

#define INC_BOLFUEGO

// Incluimos las librer�as necesarias.

#include "Colec.h"
#include "InfoObj.h"
#include "DDInc.h"
#include "VarJuego.h"

// Definimos la clase.

class BolaFuego : public ObjetoBase
{
friend class Juanchi;
friend class Tortuga;
friend class VerdeLoco;
friend class Sauriolo;
friend class Robots;
friend class Caparatuzo;
friend class Bichoto;
public:
	// Constructor vac�o!
	BolaFuego() {}
	// Funciones de la clase...
	void Inicializar(double X, double Y, double iVX, double iVY, bool MN);
	virtual void Mostrar() {BufferPaint((long)(MX - Pos),(long)MY,imglstPremios[prBolaFuego],15,15);}
	virtual void Check();
	bool MoverBolaFuego(eDireccion Direccion, double Velocidad);
	bool Pegamos();
	void Matarme() {MuerteReciente = true;}
private:
	double MX, MY;
	double VX, VY;
	bool   MataNicholas;
	int    NRebotes;
};

#endif // INC_BOLFUEGO