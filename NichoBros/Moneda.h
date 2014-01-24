// En este archivo definimos la clase moneda, utilizada para
// manejar las monedas que saltan al impactar con el evento
// bloque moneda, y desaparecen en pocos instantes.

// Evitamos la doble inclusión

#ifndef INC_MONEDA

#define INC_MONEDA

// Incluimos librerías necesarias.

#include "DDInc.h"
#include "Colec.h"
#include "InfoObj.h"
#include "VarJuego.h"
#include "Cartel.h"

// Definimos la clase moneda.

class Moneda : public ObjetoBase
{
public:
	// Constructor
	Moneda() {} // El constructor no hace nada! Todo está en inicializar.
	// Funciones de la clase.
	void Inicializar(double X, double Y);
	virtual void Mostrar() {BufferPaint((long)(MX - Pos),(long)MY,imglstPremios[prMoneda],30,30);}
	virtual void Check();
	void Matarme() {MuerteReciente = true;}
private:
	double MX;
	double MY;
	double VY;
};

#endif // INC_MONEDA
