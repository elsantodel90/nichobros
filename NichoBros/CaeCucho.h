// Este archivo de cabecera define la clase CaeCucho, que maneja
// a dichos objetos. Hay dos tipos de caecuchos: uno se activa al 
// estar cerca nicholas por la izquierda una constante, y otro cuando
// Nicholas está justo abajo. Al caer, explota, y puede matar.

// Evitamos doble inclusión.

#ifndef INC_CAECUCHO

#define INC_CAECUCHO

// Incluimos las librerías necesarias.

#include "Nicholas.h"
#include "Colec.h"
#include "InfoObj.h"
#include "DDInc.h"
#include "VarJuego.h"
#include "PuenCae.h"

// Definimos la clase.

class CaeCucho : public ObjetoBase
{
friend class Nicholas;
public:
	// Constructor vacío!
	CaeCucho() {}
	// Funciones de la clase...
	void Inicializar(int X, int Y,eTipoCaeCucho T);
	bool HuboChoque();
	virtual void Mostrar() 
	{
		BufferPaint((long)MX - Pos,(long)MY,imglstEnemigos[ImagenCaeCucho],30,60);
	}
	virtual void Check();
	void Matarme() {MuerteReciente = true;}
private:
	bool   Activo;
	bool   Libre;
	double MX, MY;
	int    MCX,MCY;
	int    MICY;
	double VY;

	int    ImagenCaeCucho;
	eTipoCaeCucho Tipo;
};

#endif // INC_CAECUCHO