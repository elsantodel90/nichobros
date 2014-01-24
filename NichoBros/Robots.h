// Este archivo de cabecera define la clase Robots, que maneja
// al jefe correspondientes (Robots es un robot cuadrado, veloz,
// que dispara rayos de enrgía o algo así, y solo lo daña el impacto
// de una tortuga sobre su cuerpo).

// Evitamos doble inclusión.

#ifndef INC_ROBOTS

#define INC_ROBOTS

// Incluimos las librerías que necesitemos.

#include "InfoObj.h"
#include "Colec.h"
#include "Cartel.h"
#include "Nicholas.h"
#include "BolFuego.h"
#include "Tortuga.h"

// Definimos la clase.

class Robots : public ObjetoBase
{
public:
	// El constructor no hace nada, ya que la clase se inciacializa
	// gracias a la función Inicializar().
	Robots() {}
	// Funciones de la clase.
	void Inicializar(int CX,int CY);
	virtual void Mostrar() 
	{
		static bool CuandoCambio = true, BlancoAla = false;
		if (Intermitente > 0)
		{
			if (Ciclo == 0)
			{
				if (CuandoCambio)
				{
					CuandoCambio = false;
					BlancoAla    = !BlancoAla;
				}
			}
			else
				CuandoCambio = true;
			if (BlancoAla)
				WhitePaint((long)(ImagenRobotsX - Pos),(long)ImagenRobotsY, imglstJefes[ImagenRobotsPicture],64,60);
			else
				BufferPaint((long)(ImagenRobotsX - Pos),(long)ImagenRobotsY, imglstJefes[ImagenRobotsPicture],60,60);
		}
		else
		{
			BlancoAla    = false;
			Intermitente = 0;
			BufferPaint((long)(ImagenRobotsX - Pos),(long)ImagenRobotsY, imglstJefes[ImagenRobotsPicture],60,60);
		}
	}
	virtual void Check();
	bool MoverRobots(eDireccion Direccion, double Velocidad);
	bool ColisionTortuga();
	bool ColisionBolaFuego();
	bool ColisionRayoEnergia();
	bool RevisarMuerte();
	void Matarme() {NivelTerminado = true; MuerteReciente = true;}
private:
	double		 ImagenRobotsX, ImagenRobotsY;
	eImagRobots  ImagenRobotsPicture;
	bool		 Activo;
	long		 Intermitente;
	long		 SinAccion;
	eDireccion   Sentido;
	bool		 EstoyMuerto;
	double		 VY;
	int			 GolpesRestantes;
	bool         ExactoV, ExactoH;
	long	     CorreccionH, CorreccionV;
};

#endif // INC_ROBOTS
