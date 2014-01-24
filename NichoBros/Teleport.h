// En este archivo de cabecera se define la clase Teleporter. Esta
// clase maneja a los teletransportadores: estos son objetos con una
// plataforma, que le dan a nuestro protagonista la capacidad
// de teletransportarse instantaneamente de un lado a otro.

// Evitamos doble inclusion...

#ifndef INC_TELEPORT

#define INC_TELEPORT

// Incluimos librerías necesarias...
#include <set>
#include "DDInc.h"
#include "VarJuego.h"
#include "Colec.h"
#include "Nicholas.h"

// Definimos la clase Rebotador.

class Teleporter : public ObjetoBase
{
public:
    static std::set<std::pair<int,int> > casillasConTeleporters;
	// Constructor vacio. Inicializamos con inicializar().
	Teleporter() {}
	// Funciones de la clase.
	void Inicializar(int CX, int CY);
	virtual void Mostrar() 
	{
		BufferPaint(MX - Pos,MY,imglstAparejos[apaTeleporter],120,30);
		if (Teleporto)
		{
			BufferPaint((long)MX - Pos + 30, 
				        (long)MY - 90, imglstNicholas[Nicho->Estado],60,(long)NH);
			BufferPaint((long)MX - Pos + 30, 
				        (long)MY - 90 + (long)NH, TeleImage,60,90 - (long)NH,0,(long)NH);
		}
		else if (Recibo)
		{
			BufferPaint((long)MX - Pos + 30, 
				        (long)MY - (long)NH, imglstNicholas[Nicho->Estado],60,(long)NH,0,90 - (long)NH);
			BufferPaint((long)MX - Pos + 30, 
				        (long)MY - 90, TeleImage,60,90 - (long)NH,0,0);
		}
	}
	virtual void Check();
	void Matarme() {MuerteReciente = true;}
	void SetCompa(Teleporter *C) {Compa = C;}
	void Recibir() {Recibo = true; NH = 0;}
private:
	long   MX;
	int    MY, MCX,MCY;
	double NH;

	bool Activo;
	bool Teleporto;
	bool Recibo;

	Teleporter *Compa;
};

#endif // INC_TELEPORT
