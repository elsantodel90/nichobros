// Este archivo de cabecera define la clase Bichoto, que maneja
// los enemigos correspondientes (los Bichotos son esos enemigos
// chiquititos, redondos, que mueven las patitas y se aplastan
// relativamente fácil).

// Evitamos doble inclusión.

#ifndef INC_BICHOTO

#define INC_BICHOTO

// Incluimos las librerías que necesitemos.

#include "InfoObj.h"
#include "Colec.h"
#include "Cartel.h"
#include "Nicholas.h"
#include "BolFuego.h"
#include "Tortuga.h"
#include "Rayo.h"

// Definimos la clase.

class Bichoto : public ObjetoBase
{
public:
	// El constructor no hace nada, ya que la clase se inciacializa
	// gracias a la función Inicializar().
	Bichoto() {}
	// Funciones de la clase.
	void Inicializar(int CX,int CY);
	virtual void Mostrar() {BufferPaint((long)(ImagenBichotoX - Pos),(long)ImagenBichotoY, imglstEnemigos[ImagenBichotoPicture],30,30);}
	virtual void Check();
	bool MoverBichoto(eDireccion Direccion, double Velocidad);
	bool ColisionTortuga();
	bool ColisionBolaFuego();
	bool ColisionRayoEnergia();
	bool RevisarMuerte();
	void Matarme() {MuerteReciente = true;}
private:
	double		 ImagenBichotoX, ImagenBichotoY;
	eImagBichoto ImagenBichotoPicture;
	bool		 Activo;
	bool		 Ahora;
	eDireccion   Sentido;
	bool		 EstoyMuerto;
	bool		 Aplastado;
	double		 VY;
	bool         ExactoV, ExactoH;
	long	     CorreccionH, CorreccionV;
};

#endif // INC_BICHOTO