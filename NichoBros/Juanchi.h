// Este archivo de cabecera define la clase Juanchi, que maneja
// los enemigos correspondientes (los Juanchis son esos enemigos
// chiquititos, redondos, que mueven las patitas y se aplastan
// relativamente fácil).

// Evitamos doble inclusión.

#ifndef INC_JUANCHI

#define INC_JUANCHI

// Incluimos las librerías que necesitemos.

#include "InfoObj.h"
#include "Colec.h"
#include "Cartel.h"
#include "Nicholas.h"
#include "BolFuego.h"
#include "Tortuga.h"
#include "Rayo.h"

// Definimos la clase.

class Juanchi : public ObjetoBase
{
public:
	// El constructor no hace nada, ya que la clase se inciacializa
	// gracias a la función Inicializar().
	Juanchi() {}
	// Funciones de la clase.
	void Inicializar(int CX,int CY);
	virtual void Mostrar() {BufferPaint((long)(ImagenJuanchiX - Pos),(long)ImagenJuanchiY, imglstEnemigos[ImagenJuanchiPicture],30,30);}
	virtual void Check();
	bool MoverJuanchi(eDireccion Direccion, double Velocidad);
	bool ColisionTortuga();
	bool ColisionBolaFuego();
	bool ColisionRayoEnergia();
	bool RevisarMuerte();
	void Matarme() {MuerteReciente = true;}
private:
	double		 ImagenJuanchiX, ImagenJuanchiY;
	eImagJuanchi ImagenJuanchiPicture;
	bool		 Activo;
	bool		 Ahora;
	eDireccion   Sentido;
	bool		 EstoyMuerto;
	int			 Aplastado;
	double		 VY;
	bool         ExactoV, ExactoH;
	long	     CorreccionH, CorreccionV;
};

#endif // INC_JUANCHI