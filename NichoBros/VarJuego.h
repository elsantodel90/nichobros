// Evitamos doble inclusion.

#ifndef INC_VARJUEGO

#define INC_VARJUEGO

// Incluimos las librerías necesarias...

#include "InfoObj.h"
#include "Mapa.h"
#include "DDInc.h"

// Constantes...

#define Pi 3.141592
#define EspacioFuente 15
#define MAX_LONG_NNIVEL 150

// Tamaño de los IMGLSTs...

#define TAMANO_TERRENO 49
#define TAMANO_NICHOLAS 10
#define TAMANO_PREMIOS 17
#define TAMANO_ENEMIGOS 37
#define TAMANO_EXPLOSIONES 5
#define TAMANO_ANIMACIONES 30
#define TAMANO_FUEGO 16
#define TAMANO_REBOTADOR 6
#define TAMANO_RENDERS 4
#define TAMANO_PARTICULAS 1
#define TAMANO_NICHOLASANI 100
#define TAMANO_GREENRARE 8
#define TAMANO_ROBOTS 5
#define TAMANO_JEFES 2
#define TAMANO_ENERGIA 1
#define TAMANO_PLATAFORMAS 3
#define TAMANO_COSASMALAS 2
#define TAMANO_APAREJOS 11

// Variables globales...

// Multiplicador.
extern int  Multiplicador;
// Algunos booleanos.
extern bool CambiaTrepa;
extern bool Hitazo;
extern bool MuerteReciente;
extern bool Rebotar;
extern bool EstoyVivo;
extern bool ReintentarNivel;
extern bool NivelTerminado;
// Informacion del nivel.
extern Mapa *Nivel;
// Para la velocidad, etc.
extern int  Ciclo;
// Ancho del mapa.
extern int  Ancho;
// Nombre del nivel.
extern char Nombre[];
// Nombre del fondo del nivel
extern char FondoNivel[];
// Posicion de la camara, junto con su máximo valor posible.
extern long Pos;
extern long MaxPosicion;
// Cansancio de Nicholas.
extern double Cansancio;
// Nº nivel.
extern int  NumNivel;
// Dificultad de juego.
extern eExtraInfo Dificultad;                      
// Información sobre teclas pulsadas.
extern bool Teclado[4096];
// Vidas restantes.
extern int  Vidas;
// Para las animaciones de tiles.
extern int  InterrN;
// Puntaje del jugador en este momento.
extern long Puntaje;
// Monedas del jugador en este momento.
extern int  Monedas;
// Bolas de fuego disponibles.
extern int  CantidadBF;
// Laseres disponibles.
extern int  CantidadLaser;
// Tiempo transcurrido desde el último frame...
extern long Delta;
// Tiempo en que ocurrio el último frame.
extern long LastFrameTime;
// Para activar plataformas que caen:
extern int  APCX,APCY;
extern bool APC;
// Para avisar al teleporter que se puede.
extern int  ATPX,ATPY;
extern bool ATP;
// Para mandarle a las puertas las señales de los botones.
extern bool ApretadoBotR;
extern bool ApretadoBotC;
// Para eliminar bolas de fuego...
extern int  EBFX, EBFY;
extern bool EBF;
// Para eliminar los rayos de energia...
extern int  EREX, EREY;
extern bool ERE;
// Para lograr los rebotes...
extern int  RebX,RebY;
extern bool ColRebotador, ColRebArr;
// Para manejar los marcadores...
extern int  CandadoX, CandadoY;
extern bool CerrarCandado;
extern int  ImagenCandado;
// Para los sauriolos y sus bolas de fuego.
extern int ValorBolas;
// Y fundamental... El tiempo!
extern int TiempoJuego;
// Tiempo de velocidad...
extern int TiempoLV;
// Imagen para la teletransportación
extern LPDIRECTDRAWSURFACE TeleImage;
// Para los puntos.
extern int PuntajePremio[9];
// Duplicados históricos para las grabaciones.
extern long PuntajeG;
extern int  VidasG;
extern int  MonedasG;
extern int  CantidadBFG;
extern int  CantidadLaserG;

// Que mierda que es el nichobros...
extern bool HayCandados;

// Superficies...

extern LPDIRECTDRAWSURFACE lpDDSFondo;
extern LPDIRECTDRAWSURFACE lpDDSBarraCansancio;
extern LPDIRECTDRAWSURFACE lpDDSBarraVerde;
extern LPDIRECTDRAWSURFACE imglstTerreno    [TAMANO_TERRENO    ];
extern LPDIRECTDRAWSURFACE imglstNicholas   [TAMANO_NICHOLAS   ];
extern LPDIRECTDRAWSURFACE imglstPremios    [TAMANO_PREMIOS	   ];
extern LPDIRECTDRAWSURFACE imglstEnemigos   [TAMANO_ENEMIGOS   ];
extern LPDIRECTDRAWSURFACE imglstExplosiones[TAMANO_EXPLOSIONES];
extern LPDIRECTDRAWSURFACE imglstAnimaciones[TAMANO_ANIMACIONES];
extern LPDIRECTDRAWSURFACE imglstFuego      [TAMANO_FUEGO      ];
extern LPDIRECTDRAWSURFACE imglstRebotador  [TAMANO_REBOTADOR  ];
extern LPDIRECTDRAWSURFACE imglstRenders    [TAMANO_RENDERS    ];
extern LPDIRECTDRAWSURFACE imglstParticulas [TAMANO_PARTICULAS ];
extern LPDIRECTDRAWSURFACE imglstNicholasAni[TAMANO_NICHOLASANI];
extern LPDIRECTDRAWSURFACE imglstGreenRare  [TAMANO_GREENRARE  ];
extern LPDIRECTDRAWSURFACE imglstRobots     [TAMANO_ROBOTS	   ];
extern LPDIRECTDRAWSURFACE imglstJefes		[TAMANO_JEFES	   ];
extern LPDIRECTDRAWSURFACE imglstEnergia	[TAMANO_ENERGIA	   ];
extern LPDIRECTDRAWSURFACE imglstPlataformas[TAMANO_PLATAFORMAS];
extern LPDIRECTDRAWSURFACE imglstCosasMalas [TAMANO_COSASMALAS ];
extern LPDIRECTDRAWSURFACE imglstAparejos   [TAMANO_APAREJOS   ];

// Funciones...

// Conversion.

inline void CoordenadaCasilla(long X, int Y, int &CX, int &CY)
{
    if (X < 0)
        CX = (X - 29) / 30;
    else
        CX = X / 30;
    if (Y < 0)
        CY = (Y - 29) / 30;
    else
        CY = Y / 30;
}

inline void CasillaCoordenada(long &X, int &Y, int CX, int CY)
{
    X = CX * 30;
    Y = CY * 30;
}

// Movimiento de cámara

inline void MoverCamara(int Velocidad)
{
    Pos += Velocidad;
	if (Pos > MaxPosicion)
		Pos = MaxPosicion;
}

// Tiempo desde un instante dado

inline long TiempoDesde(long Instante)
{
    return SDL::getTicks() - Instante;
}

inline eDireccion dirInv(eDireccion d)
{
    switch(d)
    {
        case Arriba:
            return Abajo;
            break;
        case Abajo:
            return Arriba;
            break;
        case Derecha:
            return Izquierda;
            break;
        case Izquierda:
            return Derecha;
            break;
        default:
            return Quieto;
            break;
    }
    
}

#endif // INC_VARJUEGO
