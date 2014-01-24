// Este archivo de cabecera define la clase Caparatuzo, que maneja
// los enemigos correspondientes (los Caparatuzos son esos enemigos
// chiquititos, con caparaz�n de pinches, que mueven las patitas y no 
// se aplastan con facilidad...).

// Evitamos doble inclusi�n.

#ifndef INC_CAPARATUZO

#define INC_CAPARATUZO

// Incluimos las librer�as que necesitemos.

#include "InfoObj.h"
#include "Colec.h"
#include "Cartel.h"
#include "Nicholas.h"
#include "BolFuego.h"
#include "Tortuga.h"
#include "Rayo.h"

// Definimos la clase.

class Caparatuzo : public ObjetoBase
{
public:
	// El constructor no hace nada, ya que la clase se inicializa
	// gracias a la funci�n Inicializar().
	Caparatuzo() {}
	// Funciones de la clase.
	void Inicializar(int CX,int CY);
	virtual void Mostrar() {BufferPaint((long)(ImagenCaparatuzoX - Pos),(long)ImagenCaparatuzoY, imglstEnemigos[ImagenCaparatuzoPicture],30,30);}
	virtual void Check();
	bool MoverCaparatuzo(eDireccion Direccion, double Velocidad);
	bool ColisionTortuga();
	bool ColisionBolaFuego();
	bool ColisionRayoEnergia();
	bool RevisarMuerte();
	void Matarme() {MuerteReciente = true;}
private:
	double		    ImagenCaparatuzoX, ImagenCaparatuzoY;
	eImagCaparatuzo ImagenCaparatuzoPicture;
	bool		    Activo;
	bool		    Ahora;
	eDireccion      Sentido;
	bool	        EstoyMuerto;
	double		    VY;
	bool            ExactoV, ExactoH;
	long	        CorreccionH, CorreccionV;
};

#endif // INC_CAPARATUZO