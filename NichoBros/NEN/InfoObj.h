// Evitamos doble inclusión

#ifndef INC_INFOOBJ

#define INC_INFOOBJ

// Informacion para el creador de niveles.

#define NoRedibujar 500

// TIPOS DE COLISION

enum eTiposCol {ColNula = 0, ColSuperior = 1, ColSuperiorDe = 2,
				ColSuperiorIz = 3, ColIzquierda = 4, 
				ColDerecha = 5, ColInferiorIz = 6, 
				ColInferior = 7, ColInferiorDe = 8,
				ColTotalInterna = 9, ColTotalExterna = 10};

// CASILLAS

enum eCasillas {caVacia = 0, caPisoComun = 1, caPisoMetal = 2,
				caPisoPiedra = 3, caCartelSalida = 4,
				caBloqueInterr = 5, caBloqueSimple = 6,
				caBloqueAnulado = 7, caFuegoLava = 8, 
				caPastoIzquierda = 9, caPastoMedio = 10,
				caPastoDerecha = 11, caCalentador = 12,
				caBloquePuente = 13, caBloquePuenteAzul = 14,
				caNube1 = 15,caNube2 = 16,caNube3 = 17,caNube4 = 18,
				caNube5 = 19,caNube6 = 20, caGeneradorIz = 21, caGeneradorMed = 22,
				caGeneradorDe = 23, caEscaleraArI = 24, caEscaleraI = 25,
				caEscaleraAbI = 26};

// EVENTOS

enum eEventos  {evNulo = 0, evInicioNicho = 1, evEneJuan = 2,
				evZonaSalir = 3, evPararEne = 4, evPisoFragil = 5,
				evPisoRigido = 6, evPremioSpeed = 7,
				evEneTortuga = 8, evBloqueMoneda = 9,
				evPremioMoneda = 10, evPremioBolaFuego = 11,
				evRebotadorSimple = 12, evSoloParriba = 13,
				evPremioEscudo = 14, evBolaSaltarina = 15, 
				evZonaDuele = 16, evPremioAureola = 17,
				evZonaDueleRigido = 18, evPuenteCae = 19,
				evEneVerdeLoco = 20, evPuenteFlojo = 21,
				evGenerador = 22, evJefeRobots = 23,
				evCandado = 24, evEscalera = 25, evPlatCae = 26,
				evPinchePalo = 27,evTriturin = 28, evPlatLat = 29,
//Este evento no se puede usar con el creador de niveles:
				evExplotando = 255};

// EXTRAINFO

enum eExtraInfo {inQuesoRemachado  = 0, inNewby = 1,
			     inJuegueroDecente = 2, inCyberAdicto = 3,
				 inTapador = 4,inDifMas = 8};

// PREMIOS

enum ePremios  {prSpeed = 0, prMoneda = 1, prBolaFuego = 2,
				prEscudo = 3, prAureola = 4};

// PARTICULAS

enum eParticulas {partVerde = 0};

// FUEGO

enum eFuego {fuegArribaPico = 11, fuegAbajoPico = 12};

// NICHOLAS

#define NicholasInmunidad 3000    // En milisegundos
#define NicholasInmunidadAE 9000 // En milisegundos
#define NicholasTYaSeVaAE 2500
#define VelocidadNicholas 0.3
#define VelocidadQuemarNicholas 0.06
#define NicholasPotenciaSalto 1.25
#define NicholasRebote 0.65
#define IncrementoAnguloAE 0.005
#define TasaRecuperacionNormal 0.01
#define TasaRecuperacionSuper 0.015
#define DistanciaColisionNicholasCasilla 15
enum eImagNicholas {NicholasDe = 0, NicholasIz = 1, 
					NicholasCaDe = 2, NicholasCaIz = 3, 
					NicholasSalDe = 4, NicholasSalIz = 5};

// JUANCHI

#define PuntajeMuerteJuanchi 100
#define VelocidadJuanchi 0.15
#define SaltoJuanchiImpacto 0.6
enum eImagJuanchi {Juanchi1 = 0, Juanchi2 = 1,
				   JuanchiAp = 2, JuanchiRev = 3};

// TORTUGA

#define PuntajeMuerteTortuga 250
#define VelocidadTortuga 0.25
#define VelocidadTortugaResbala 1.5
#define SaltoTortugaImpacto 0.6
enum eImagTortuga {TortugaDe = 4, TortugaCaDe1 = 5, TortugaCaDe2 = 6,
				   TortugaIz = 7, TortugaCaIz1 = 8, TortugaCaIz2 = 9,
				   TortugaRev = 10};

// VERDELOCO

#define PuntajeMuerteVerde 375
#define DistanciaReaccionVerde 100
#define TiempoCambioVerdeFrame 50
#define TiempoVerdeFlota 1600
#define SaltoVerdeImpacto 0.9
enum eImagVerde {GreenRareRev = 11};
// SONIDO!

enum eSonido {sndAplastado = 0, sndBebiendo = 1, sndCaida = 2, 
			  sndChamelotCorto = 3, sndNivelCompleto = 4, 
			  sndNoPirateria = 5, sndOhOh = 6, sndChoqueTecho = 7,
			  sndPlop = 8, sndResorte = 9, sndRupturaBloque = 10, 
			  sndVueltaNormal = 11, sndMoneda = 12, 
			  sndBolaFuego = 13, sndAuraEscudo = 14, 
			  sndAureola = 15, sndLanzarBF = 16, sndPuenteCae = 17,
			  sndEsoDolio = 18, sndMarchaFunebre = 19, sndToc = 20,
			  sndBoton = 21, sndVersionFull = 22, sndGenerador = 23};
#define TAMANO_SONIDO 24

// AMBIENTE

#define VelocidadJuego 0.85
#define MaxDeltaProc 17
#define MaxDelta 30
#define DuracionCiclo 25
#define MaximoMovimiento 10
#define VelocidadMovRelativoFondo 0.4
#define FranjaCambio 250
#define VelocidadGravedad 0.25
#define ElevacionCartel 150
#define VelocidadCartel 0.1
#define PuntajeRomperCasilla 50
#define PuntajeEnergizante 1000
#define AnguloSuperiores 70
#define AnguloInferiores 40
#define VelocidadExplosion 0.7
#define VelocidadMoneda 1
#define VelocidadReboteBolaFuego 0.5
#define VelocidadBolaFuego 0.4
#define RebotesBolaFuego 2
#define AnguloBolaFuego 25
#define AceleracionGravedad 0.005
#define MonedasParaVida 50
#define ToleranciaXSalto 4
#define CoefReboteSimple 1.1
#define MaxReboteSimple 2
#define RetardoBolaPico 2500
#define RetardoCaidaPuente 275
#define RetardoPuenteFlojo 25
#define TopeSaltoFlojo 1.2

// MENSAJES

#define MENSAJE_GANAR_MONEDA "¡+1 oro!"
#define MENSAJE_GANAR_BF "¡+1 fuego!"
#define MENSAJE_GANAR_VIDA "¡+1 vida!"

// DIRECCION

enum eDireccion {Quieto = 0, Derecha = 1, Izquierda = 2,
				 Abajo = 3, Arriba = 4};


#endif // INC_INFOOBJ
