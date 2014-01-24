// Este archivo de cabecera define la clase Tortuga, que maneja
// a estos enemigos. Las tortugas (cosa extraña) son velocidad
// rapida, y al aplastarlas su caparazón queda.

// Evitamos doble inclusión.

#ifndef INC_TORTUGA

#define INC_TORTUGA

// Incluimos las librerías necesarias...

#include "InfoObj.h"
#include "VarJuego.h"
#include "Colec.h"
#include "Cartel.h"
#include "Nicholas.h"
#include "Rayo.h"

// Definimos la clase Tortuga

class Tortuga : public ObjetoBase
{
friend class Juanchi;
friend class VerdeLoco;
friend class Sauriolo;
friend class Robots;
friend class Caparatuzo;
friend class Bichoto;
public:
	// Constructor nulo: la clase se inicializa mediante la
	// funcion inicializar.
	Tortuga() {}
	// Funciones de la clase
	void Inicializar(int CX, int CY);
	virtual void Mostrar() {BufferPaint((long)ImagenTortugaX - Pos, (long)ImagenTortugaY, imglstEnemigos[ImagenTortugaPicture],90,30);}
	virtual void Check();
	bool MoverTortuga(eDireccion Direccion, double Velocidad);
	bool ColisionBolaFuego();
	bool ColisionTortuga();
	bool ColisionRayoEnergia();
	bool RevisarMuerte();
	bool RevisarEmpuje();
	void Matarme() {MuerteReciente = true;}
private:
	double		 ImagenTortugaX, ImagenTortugaY;
	eImagTortuga ImagenTortugaPicture;
	bool		 Activo;
	bool		 Ahora;
	eDireccion   Sentido;
	bool		 EstoyMuerto;
	bool		 Aplastado;
	bool		 Resbalando;
	bool		 MataNicholas;
	double		 VY;
};

#endif // INC_TORTUGA