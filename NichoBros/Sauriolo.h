// Este archivo de cabecera define la clase Sauriolo, que maneja
// los enemigos correspondientes (los Sauriolos son esos enemigos
// cuadradotes, tipo koopa, que se mueven lento pero resultan letales)

// Evitamos doble inclusión.

#ifndef INC_SAURIOLO

#define INC_SAURIOLO

// Incluimos las librerías que necesitemos.

#include "InfoObj.h"
#include "Colec.h"
#include "Cartel.h"
#include "Nicholas.h"
#include "BolFuego.h"
#include "Tortuga.h"
#include "Rayo.h"

// Definimos la clase.

class Sauriolo : public ObjetoBase
{
public:
	// El constructor no hace nada, ya que la clase se inicializa
	// gracias a la función Inicializar().
	Sauriolo() {}
	// Funciones de la clase.
	void Inicializar(int CX,int CY);
	virtual void Mostrar() {BufferPaint((long)(MX - Pos),(long)MY, imglstEnemigos[ImagenSaurioloPicture],
		                                                  (ImagenSaurioloPicture == SaurioloRevDe || ImagenSaurioloPicture == SaurioloRevIz)?52:60,(ImagenSaurioloPicture == SaurioloRevDe || ImagenSaurioloPicture == SaurioloRevIz)?21:60);}
	virtual void Check();
	bool MoverSauriolo(eDireccion Direccion, double Velocidad);
	void ColisionTortuga();
	bool ColisionBolaFuego();
	bool ColisionRayoEnergia();
	bool RevisarMuerte();
	void Matarme() {MuerteReciente = true;}
private:
	double		  MX, MY;
	eImagSauriolo ImagenSaurioloPicture;
	bool		  Activo;

	eDireccion    Sentido;

	int           Salud;

	bool		  Ahora;
	bool          Subiendo; // Esto es para manejar las imágenes, nada más.
	bool		  EstoyMuerto;
	double		  VY;

	bool          ExactoV, ExactoH;
	long	      CorreccionH, CorreccionV;
};

#endif // INC_SAURIOLO