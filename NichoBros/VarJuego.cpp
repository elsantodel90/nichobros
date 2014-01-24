// Incluimos las librerías necesarias...

#include "RutinasSDL/rutinasSDL.h"
#include "VarJuego.h"

// Variables globales...
// Multiplicador.
int  Multiplicador;
// Algunos booleanos.
bool CambiaTrepa;
bool Hitazo;
bool MuerteReciente;
bool Rebotar;
bool EstoyVivo;
bool ReintentarNivel;
bool NivelTerminado;
// Informacion del nivel.
Mapa *Nivel = NULL;
// Para la velocidad, etc.
int  Ciclo;
// Ancho del mapa.
int  Ancho;
// Nombre del nivel.
char Nombre[MAX_LONG_NNIVEL];
// Nombre del fondo del nivel.
char FondoNivel[50];
// Posicion de la camara.
long Pos;
long MaxPosicion;
// Cansancio de Nicholas.
double Cansancio;
// Nº nivel.
int  NumNivel;
// Dificultad de juego.
eExtraInfo Dificultad = inJuegueroDecente;
// Información sobre teclas pulsadas.
bool Teclado[4096];
// Vidas restantes.
int  Vidas;
// Para las animaciones de tiles.
int  InterrN;
// Puntaje del jugador en este momento.
long Puntaje;
// Monedas del jugador en este momento.
int  Monedas;
// Bolas de fuego disponibles.
int  CantidadBF;
// Laseres disponibles.
int  CantidadLaser;
// Tiempo transcurrido desde el último frame.
long Delta;
// Tiempo en que ocurrio el último frame.
long LastFrameTime;
// Para activar plataformas que caen:
int  APCX,APCY;
bool APC;
// Para avisar al teleporter que se puede.
int  ATPX,ATPY;
bool ATP;
// Para mandarle a las puertas las señales de los botones.
bool ApretadoBotR;
bool ApretadoBotC;
// Para eliminar bolas de fuego...
int  EBFX, EBFY;
bool EBF;
// Para eliminar los rayos de energia...
int  EREX, EREY;
bool ERE;
// Para lograr los rebotes...
int  RebX,RebY;
bool ColRebotador, ColRebArr;
// Para manejar los marcadores...
int  CandadoX, CandadoY;
bool CerrarCandado;
int  ImagenCandado;
// Para los sauriolos y sus bolas de fuego.
int ValorBolas;
// Y fundamental... El tiempo!
int TiempoJuego;
// Tiempo de velocidad...
int TiempoLV;
// Imagen para la teletransportación.
LPDIRECTDRAWSURFACE TeleImage = NULL;
// Para los puntos.
int PuntajePremio[9];
// Duplicados históricos para las grabaciones.
long PuntajeG;
int  VidasG;
int  MonedasG;
int  CantidadBFG;
int  CantidadLaserG;

// Que mierda que es el nichobros...
bool HayCandados;


// Superficies...

LPDIRECTDRAWSURFACE lpDDSFondo = NULL;
LPDIRECTDRAWSURFACE lpDDSBarraCansancio = NULL;
LPDIRECTDRAWSURFACE lpDDSBarraVerde = NULL;

LPDIRECTDRAWSURFACE imglstTerreno    [TAMANO_TERRENO    ];
LPDIRECTDRAWSURFACE imglstNicholas   [TAMANO_NICHOLAS	];
LPDIRECTDRAWSURFACE imglstPremios    [TAMANO_PREMIOS	];
LPDIRECTDRAWSURFACE imglstEnemigos   [TAMANO_ENEMIGOS	];
LPDIRECTDRAWSURFACE imglstExplosiones[TAMANO_EXPLOSIONES];
LPDIRECTDRAWSURFACE imglstAnimaciones[TAMANO_ANIMACIONES];
LPDIRECTDRAWSURFACE imglstFuego      [TAMANO_FUEGO      ];
LPDIRECTDRAWSURFACE imglstRebotador  [TAMANO_REBOTADOR  ];
LPDIRECTDRAWSURFACE imglstRenders    [TAMANO_RENDERS    ];
LPDIRECTDRAWSURFACE imglstParticulas [TAMANO_PARTICULAS ];
LPDIRECTDRAWSURFACE imglstNicholasAni[TAMANO_NICHOLASANI];
LPDIRECTDRAWSURFACE imglstGreenRare  [TAMANO_GREENRARE  ];
LPDIRECTDRAWSURFACE imglstRobots     [TAMANO_ROBOTS	    ];
LPDIRECTDRAWSURFACE imglstJefes		 [TAMANO_JEFES		];
LPDIRECTDRAWSURFACE imglstEnergia	 [TAMANO_ENERGIA    ];
LPDIRECTDRAWSURFACE imglstPlataformas[TAMANO_PLATAFORMAS];
LPDIRECTDRAWSURFACE imglstCosasMalas [TAMANO_COSASMALAS ];
LPDIRECTDRAWSURFACE imglstAparejos   [TAMANO_APAREJOS   ];
