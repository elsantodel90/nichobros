// Evitamos doble inclusión.

#ifndef INC_NICHOBROS

#define INC_NICHOBROS

// Incluimos librerías necesarias...

// Standard

#include <cstring>
#include <fstream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

// Nuestras

#include "Cartel.h"
#include "Colec.h"
#include "DDInc.h"
#include "Debug.h"
#include "Explo.h"
#include "InfoObj.h"
#include "Juanchi.h"
#include "Mapa.h"
#include "Nicholas.h"
#include "VarJuego.h"
#include "Tortuga.h"
#include "Moneda.h"
#include "BolFuego.h"
#include "Reboter.h"
#include "BolFuSal.h"
#include "PuenCae.h"
#include "VerdeLoc.h"
#include "Robots.h"
#include "Rayo.h"
#include "PlatCae.h"
#include "PinPalo.h"
#include "Triturin.h"
#include "PlatLat.h"
#include "CaeCucho.h"
#include "Teleport.h"
#include "Puerta.h"
#include "Sauriolo.h"
#include "Caparatuzo.h"
#include "Bichoto.h"

#ifdef DEPURANDO
// Variables para calcular cuadros por segundo
int FPS_tUltimo = 0;
int FPS_Suma    = 0;
int FPS_Actual  = 0;
#endif // DEPURANDO

//Variables globales relacionadas con la ventana
const char* g_appName = "Nichobros Versión Demo";
bool        g_active;

// Funcion para descargar una coleccion, pero si es null no hace
// nada.

void DescargarColeccion(Coleccion *MiColec);
void DescargarSuperficie(LPDIRECTDRAWSURFACE &Surf);

#endif // INC_NICHOBROS
