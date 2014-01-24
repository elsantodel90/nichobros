// Este archivo de cabecera define la clase Rayo, que maneja
// a dichos objetos. Un rayo es disparado con velocidad
// x inicial, y luego sigue un movimiento rectilíneo uniforme, donde
// la gravedad no lo afecta en absoluto.

// Evitamos doble inclusión.

#ifndef INC_RAYO

#define INC_RAYO

// Incluimos las librerías necesarias.

#include "Colec.h"
#include "InfoObj.h"
#include "DDInc.h"
#include "VarJuego.h"
#include "Explo.h"

// Definimos la clase.

class Rayo : public ObjetoBase
{
friend class Juanchi;
friend class Tortuga;
friend class VerdeLoco;
friend class Sauriolo;
friend class Robots;
friend class Nicholas;
friend class Caparatuzo;
friend class Bichoto;
public:
	// Constructor vacío!
	Rayo() {}
	// Funciones de la clase...
	void Inicializar(double X, double Y, double iVX, bool iMN, bool iMR = true);
	virtual void Mostrar() {BufferPaint((long)(MX - Pos),(long)MY,imglstEnergia[enerRayo],24,8);}
	virtual void Check();
	bool MoverRayo(eDireccion Direccion, double Velocidad);
	bool Pegamos();
	void Matarme() {MuerteReciente = true;}
private:
	long   GolpesRestantes;
	double MX, MY;
	double VX;
	bool   MN,MR;
};

#endif // INC_RAYO
