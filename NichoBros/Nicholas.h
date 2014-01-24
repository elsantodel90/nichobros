// Este archivo define la clase Nicholas, para su utilización en 
// el programa. Esta clase maneja al personaje.

// Evitamos doble inclusión.

#ifndef INC_NICHOLAS

#define INC_NICHOLAS

// Inclusión de librerías...

#include <cmath>
#include "InfoObj.h"
#include "VarJuego.h"
#include "Colec.h"
#include "Cartel.h"
#include "Moneda.h"
#include "BolFuego.h"
#include "Explo.h"
#include "PuenCae.h"
#include "Triturin.h"
#include "Rayo.h"


// Definimos nuestras propias constantes útiles...

#define NoSoyInmune -1
#define NoHuboUltimo -100

// Definimos la clase Nicholas.

class Nicholas : public ObjetoBase
{
friend void ActualizarJuego();
friend eTiposCol ColisionaNicholas(RECT &OR, LPDIRECTDRAWSURFACE lpSprite,int surStart);
friend void Blt();
friend bool ColNicholasCasilla(int X, int Y);
friend class Juanchi;
friend class Tortuga;
friend class Sauriolo;
friend class Rebotador;
friend class VerdeLoco;
friend class Robots;
friend class PlatCae;
friend class PlatLat;
friend class CaeCucho;
friend class Teleporter;
friend class Caparatuzo;
friend class Bichoto;
public:
	// Constructores.
	Nicholas() {} // El constructor no hace nada! Todo está en inicializar.
	// Destructor
	~Nicholas()
	{
		if (AuxSurf != NULL)
		{
            SDL::freeSurface(AuxSurf);
			AuxSurf = NULL;
		}
	}
	// Funciones de la clase
	void Inicializar(int CX, int CY);
	virtual void Mostrar() 
	{
		if (Teleportando)
			return;
		if (EstoyVivo)
			BufferPaint((long)ImagenNicholasX - Pos, (long)ImagenNicholasY, imglstNicholas[Estado],60,90);
		else
			BufferPaint((long)ImagenNicholasX - Pos, (long)ImagenNicholasY, AuxSurf,60,90);
		if (AuraEscudo)
		{
			if (Inmunidad <= NicholasInmunidadAE - NicholasTYaSeVaAE ||
				Ciclo % 2 == 0)
			{
				BufferPaint((long)(ImagenNicholasX - Pos + 14 + 25 * cos(AnguloAE))						  , (long)(ImagenNicholasY + 29 + 37 * sin(AnguloAE))						, imglstParticulas[partVerde],30,30);
				BufferPaint((long)(ImagenNicholasX - Pos + 14 + 25 * cos(AnguloAE + 3.141592 / 2.0))	  , (long)(ImagenNicholasY + 29 + 37 * sin(AnguloAE + 3.141592 / 2.0))		, imglstParticulas[partVerde],30,30);
				BufferPaint((long)(ImagenNicholasX - Pos + 14 + 25 * cos(AnguloAE + 3.141592))			  , (long)(ImagenNicholasY + 29 + 37 * sin(AnguloAE + 3.141592))			, imglstParticulas[partVerde],30,30);
				BufferPaint((long)(ImagenNicholasX - Pos + 14 + 25 * cos(AnguloAE + 3.0 * 3.141592 / 2.0)), (long)(ImagenNicholasY + 29 + 37 * sin(AnguloAE + 3.0 * 3.141592 / 2.0)), imglstParticulas[partVerde],30,30);
			}
			AnguloAE += IncrementoAnguloAE * Delta;
			while (AnguloAE >= 3.141592)
				AnguloAE -= 3.141592;
		}
	}
	void CargarAuxSurf();
	virtual void Check();
	void RefrescarPS();
	bool MoverNicholas(eDireccion Direccion, double Velocidad,bool Rompe = true);
	bool Colision(eEventos QueEvento);
	eTiposCol ColisionAlta(eEventos QueEvento);
	eTiposCol ColisionBaja(eEventos QueEvento);
	// Manejo de variables.
	// Manejo externo de supernicholas.
	bool Super ()        {return SuperNicholas;}
	void Super (bool nv) {SuperNicholas = nv; RefrescarPS();}
private:
	bool Trepando;
	bool Disparando;
	bool DisparandoL;
	bool Rebotando;
	bool AuraEscudo;
	bool Cayendo, Subiendo;
	bool SuperNicholas;
	bool Enganchado;
	bool Teleportando;

	bool		  ExactoV, ExactoH;
	long		  CorreccionH, CorreccionV;

	double        IVX,IVY; // La magia de la inercia.
	double		  AnguloAE;
	long  		  Inmunidad;
	int           BotX,BotY;
	int			  LCX, LCY;
	int			  LastCiclo;
	int           ValBot;
	double		  LineaAQuemar;
	eImagNicholas Estado;
	double		  VelSalto;
	
	double		  ImagenNicholasX, ImagenNicholasY;
	double        LINX,LINY;
	double        EngancheX;

	LPDIRECTDRAWSURFACE AuxSurf;
};

extern Nicholas *Nicho;

#endif // INC_NICHOLAS
