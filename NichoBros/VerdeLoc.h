// Este archivo de cabecera define la clase VerdeLoco, que maneja
// los enemigos correspondientes (los Verdes Locos son esos
// enemigos chiquititos, que aparecen y desaparecen, y que no se
// pueden matar si no se utilizan bolas de fuego).

// Evitamos doble inclusión.

#ifndef INC_VERDELOCO

#define INC_VERDELOCO

// Incluimos las librerías que necesitemos.

#include "InfoObj.h"
#include "Colec.h"
#include "Cartel.h"
#include "Nicholas.h"
#include "Tortuga.h"
#include "Rayo.h"

// Definimos la clase.

class VerdeLoco : public ObjetoBase
{
public:
	// El constructor no hace nada, ya que la clase se inciacializa
	// gracias a la función Inicializar().
	VerdeLoco() {}
	// Funciones de la clase.
	void Inicializar(int CX,int CY);
	virtual void Mostrar() 
	{
		BufferPaint((long)(ImagenVerdeX - Pos),(long)ImagenVerdeY, SuperficieVerde,30,30);
		if (TiempoFlotar != 0 && SV2 < 8)
			BufferPaint((long)(ImagenVerdeX - Pos),(long)ImagenVerdeY + 120, imglstGreenRare[SV2++],30,30);
	}
	virtual void Check();
	bool MoverVerde(eDireccion Direccion, double Velocidad);
	bool ColisionTortuga();
	bool ColisionBolaFuego();
	bool ColisionRayoEnergia();
	bool RevisarMuerte();
	void Matarme() {MuerteReciente = true;}
private:
	LPDIRECTDRAWSURFACE SuperficieVerde;
	int			 SV2;
	long		 DeltaAcumulado;
	long		 TiempoFlotar;
	double		 ImagenVerdeX, ImagenVerdeY;
	int			 ImagenVerdeAnimacion;
	bool		 Activo;
	bool		 EstoyMuerto;
	double		 VY;
};

#endif // INC_VERDELOCO