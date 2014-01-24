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
				caEscaleraAbI = 26, caEscakeraArD = 27, caEscaleraD = 28,
				caEscaleraAbD = 29, caPalmeraDER = 30, caPalmeraIZQ = 31, caPalmeraCEN = 32,
                caTroncoPalmera = 33, caArena = 34, caArena2 = 35, caPiedraFragil = 36,
                caPiedraFragilFuego = 37, caAgua = 38};

// EVENTOS

enum eEventos  {evNulo = 0            , evInicioNicho = 1     , evEneJuan = 2,
				evZonaSalir = 3       , evPararEne = 4        , evPisoFragil = 5,
				evPisoRigido = 6      , evPremioSpeed = 7     ,	evEneTortuga = 8, 
				evBloqueMoneda = 9    ,	evPremioMoneda = 10   , evPremioBolaFuego = 11,
				evRebotadorSimple = 12, evSoloParriba = 13    ,	evPremioEscudo = 14, 
				evBolaSaltarina = 15  , evZonaDuele = 16      , evPremioAureola = 17,
				evZonaDueleRigido = 18, evPuenteCae = 19      ,	evEneVerdeLoco = 20,
				evPuenteFlojo = 21    ,	evGenerador = 22      , evJefeRobots = 23,
				evCandado = 24        , evEscalera = 25       , evPlatCae = 26,
				evPinchePalo = 27     , evTriturin = 28       , evPlatLat = 29,
				evCaeCuchoIz = 30     , evCaeCuchoDe = 31     , evTeleporter = 32,
				evBotRojo = 33        , evBotCyan = 34        , evPuertaRoja = 35,
				evPuertaCyan = 36     , evPlatVert = 37       , evEneSauriolo = 38,
				evPremioLaser = 39	  , evPremioCereza = 40	  , evPremioFrutilla = 41,
				evPremioJuguito = 42  , evPremioBanana = 43   , evPremioPizzaVeggy = 44,
				evPremioLicuado = 45  , evPremioPizza = 46	  , evPremioPizzaC = 47,
				evPremioRadio = 48, evEneCaparatuzo = 49, evPremioLiquidoVerde = 50,
				evEneBichoto = 51, evPezSaltarin = 52,
//Estos eventos no se pueden usar con el creador de niveles:
				evBotRojoIn = 253, evBotCyanIn = 254, evExplotando = 255};

// EXTRAINFO

enum eExtraInfo {inQuesoRemachado  = 0, inNewby = 1,
			     inJuegueroDecente = 2, inCyberAdicto = 3,
				 inTapador = 4,inDifMas = 8};

// PREMIOS

enum ePremios  {prSpeed = 0, prMoneda = 1, prBolaFuego = 2,
				prEscudo = 3, prAureola = 4, prCandado = 5,
				prLaser = 6, prCereza = 7, prFrutilla = 8,
				prJuguito = 9, prBanana = 10, prPizzaVeggy = 11,
				prLicuado = 12, prPizza = 13, prPizzaC = 14,
				prRadio = 15, prLiquidoVerde = 16 };

// PARTICULAS

enum eParticulas {partVerde = 0};

// ENERGIA

#define GolpesRayoEnergia 4
#define VelocidadRayoEnergia 0.8

enum eEnergia {enerRayo = 0};

// PLATAFORMAS

#define RetardoPlataformaCae 1225
#define AmplitudPlataformaLateral 120
#define PeriodoPlataformaLateral 2000
#define AmplitudPlataformaVertical 200
#define PeriodoPlataformaVertical 5000

enum ePlataformas {platRoja = 0, platCyan = 1, 
                   platVerde = 2}; // Esto es puramente visual: describe los distintos gráficos de plataformas.

// APAREJOS

#define VelocidadTeleport 0.025
#define AberturaTeleport 13
#define ColorHazTeleport 0x03CD
#define VelocidadLevantadaPuerta 0.04
enum eAparejos {apaTeleporter = 0, apaBotR    = 1, apaBotRIn    = 2,
				                   apaBotC    = 3, apaBotCIn    = 4,
								   apaPuertaR = 5, apaPuertaC   = 6,
								   apaBotRRev = 7, apaBotRInRev = 8,
				                   apaBotCRev = 9, apaBotCInRev = 10};

// COSASMALAS

#define RetardoSalidaPinchePalo 1500
#define VelocidadMovPinchePalo 0.030

#define VelocidadMovTriturin 0.040
#define RetardoCaidaTriturin 2000
#define RetardoLevantadaTriturin 750

enum eCosasMalas {cmPinchePalo = 0, cmTriturin = 1};

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
#define TasaCansancioSuper 0.009
#define TasaCansancioNormal 0.012
#define DistanciaColisionNicholasCasilla 23
enum eImagNicholas {NicholasDe = 0, NicholasIz = 1, 
					NicholasCaDe = 2, NicholasCaIz = 3, 
					NicholasSalDe = 4, NicholasSalIz = 5,
					NicholasTreDe = 6, NicholasTreIz = 7,
					NicholasBotDe = 8, NicholasBotIz = 9};

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

// SAURIOLO

#define PuntajeMuerteSauriolo 625
#define VelocidadSauriolo 0.1
#define SaltoSaurioloImpacto 0.4
#define SaurioloGolpesSoportables 3 // -2 en queso remachado, -1 en newby, +2 en cyber adicto
enum eImagSauriolo {SaurioloDe1 = 17, SaurioloDe2 = 18, SaurioloDe3 = 19,
					SaurioloIz1 = 20, SaurioloIz2 = 21, SaurioloIz3 = 22,
					SaurioloRevDe = 23, SaurioloRevIz = 24};

// ROBOTS

#define PuntajeMuerteRobots 5000
#define SaltoRobotsImpacto 1.4
#define VelocidadRobots 0.375
#define RobotsGolpesSoportables 12 // -6 en queso remachado, -3 en newby, +6 en cyber adicto
#define TiempoSinAccionRobots 500
#define ProbabilidadRobotsGiro 750
#define ProbabilidadRobotsTiro 1500

#define TiempoIntermitenciaRobots 2100 // En milisegundos, claro está!
enum eImagRobots {RobotsNormal = 0, RobotsRev = 1};

// CAECUCHO

#define DistanciaActivacionCaecucho 30
#define ImagenCaeCuchoBase 12
enum eTipoCaeCucho {caecuchoIzquierda = 0, caecuchoDerecha = 1};

// CAPARATUZO
#define SaltoCaparatuzoImpacto 0.8
#define VelocidadCaparatuzo 0.2
#define PuntajeMuerteCaparatuzo 400
enum eImagCaparatuzo {capDe1 = 25,capDe2 = 26, capIz1 = 27, capIz2 = 28, capRev = 29};

// BICHOTO
#define VelocidadBichoto 0.1
#define PuntajeMuerteBichoto 620
#define SaltoBichotoImpacto 0.4
enum eImagBichoto {biDe1 = 30,biDe2 = 31, biIz1 = 32, biIz2 = 33, biRev = 34};

// SONIDO!

enum eSonido {sndAplastado = 0, sndBebiendo = 1, sndCaida = 2,
			  sndChamelotCorto = 3, sndNivelCompleto = 4,
			  sndNoPirateriaDEPRECATED = 5, sndOhOh = 6, sndChoqueTecho = 7,
			  sndPlop = 8, sndResorte = 9, sndRupturaBloque = 10,
			  sndVueltaNormal = 11, sndMoneda = 12, 
			  sndBolaFuego = 13, sndAuraEscudo = 14,
			  sndAureola = 15, sndLanzarBF = 16, sndPuenteCae = 17,
			  sndEsoDolio = 18, sndMarchaFunebre = 19, sndToc = 20,
			  sndBoton = 21, sndVersionFull = 22, sndGenerador = 23,
			  sndGolpeRobots = 24, sndMetalRebote = 25, sndFinJuego = 26,
              sndSufuo = 27, sndPumPum = 28, sndMeAburro = 29, sndLaser = 30,
              sndGluGluGlu = 31, sndNiam = 32, sndDiscursoCreditos = 33,
              sndTiempo = 34};
#define TAMANO_SONIDO 35

// AMBIENTE

#define VelocidadJuego 0.85
#define MaxDeltaProc 17
#define MaxDelta 30
#define DuracionCiclo 25
#define MaximoMovimiento 10
#define VelocidadMovRelativoFondo 0.4
#define FranjaCambio 250
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
#define TiempoDisponible 200000 // En milisegundos.
#define DuracionLiquidoVerde 15000 // En milisegundos.

// MENSAJES

#define MENSAJE_GANAR_MONEDA "¡+1 oro!"
#define MENSAJE_GANAR_BF "¡+1 fuego!"
#define MENSAJE_GANAR_LASER "¡+1 laser!"
#define MENSAJE_GANAR_VIDA "¡+1 vida!"
#define MENSAJE_LIQUIDO_VERDE "¡X2 velocidad!"

// DIRECCION

enum eDireccion {Quieto = 0, Derecha = 1, Izquierda = 2,
				 Abajo = 3, Arriba = 4};


#endif // INC_INFOOBJ
