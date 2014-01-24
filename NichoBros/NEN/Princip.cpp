#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include "Mapa.h"
#include "RutinasSDL/rutinasSDL.h"
#include <vector>

typedef SDL_Surface * LPDIRECTDRAWSURFACE;
typedef TTF_Font * HFONT;
typedef SDL_Color COLORREF;
typedef const char * LPCSTR;

struct RECT
{
    int top, left, right, bottom;
};

inline
void ltoa(long x, char *s)
{
    sprintf(s,"%ld",x);
}

inline
void itoa(int x, char *s)
{
    sprintf(s,"%d",x);
}

inline
COLORREF RGB(int r,int g,int b)
{
    COLORREF res;
    res.r = r;
    res.b = b;
    res.g = g;
    return res;
}

inline
SDL_Rect r2sdl(const RECT &r)
{
    SDL_Rect res;
    res.x = r.left;
    res.y = r.top;
    res.w = r.right - r.left;
    res.h = r.bottom - r.top;
    return res;
}

inline
void    DrawRectangle(LPDIRECTDRAWSURFACE Surf, RECT *FillR, COLORREF FillC)
{
    SDL_Rect rect = r2sdl(*FillR);
    SDL::fillRect(Surf, FillC, &rect);
	// Dibujamos un rectangulo relleno con FillC.
    /*
	DDBLTFX AuxFX;
	ZeroMemory(&AuxFX,sizeof(DDBLTFX));
	AuxFX.dwSize = sizeof(DDBLTFX);
	AuxFX.dwFillColor = FillC;
	if (FillR->right > 800) 
		FillR->right = 800;
	if (FillR->left < 0)
		FillR->left = 0;
	if (FillR->top < 0)
		FillR->top = 0;
	if (FillR->bottom > 600)
		FillR->bottom = 600;
	Surf->Blt(FillR,NULL,NULL,DDBLT_COLORFILL | DDBLT_WAIT,&AuxFX);
    */
}

inline
void BltFast(SDL_Surface *destination, int x,int y, SDL_Surface * source, RECT *recti)
{
    SDL_Rect rec = r2sdl(*recti);
    SDL::blit(x,y,source,destination,&rec);
}




using namespace std;

inline
COLORREF i2col(int x)
{
    COLORREF res;
    res.r = x & 0xFF;
    res.g = (x & 0XFF00) >> 8;
    res.b = (x & 0XFF0000) >> 16;
    return res;
}


//#define DEPURANDO

// Defines del editor de niveles.

#define TCOLOR 0x155B05
#define ALTOBARRA 3 // En tiles de 30x30
#define COLORBARRA 0x05558C
#define MAXNOMBRE 100
#define MAXETEXTO 100
#define TAMANO_TERRENO 49
#define NUM_EVENTOS 52
#define CANT_CATEGORIAS 4

const char NombreCat[CANT_CATEGORIAS][128] = {"Eventos :", 
                                              "Enemigos :",
                                              "Terrenos :",
                                              "Premios :"};

int tamCategoria[CANT_CATEGORIAS];
int catActual = 0;

// Superficies
LPDIRECTDRAWSURFACE lpDDSCursor  = NULL;
LPDIRECTDRAWSURFACE lpDDSRecRojo = NULL;
LPDIRECTDRAWSURFACE imglstTerreno    [TAMANO_TERRENO];

bool bEjecutandose = false;
HFONT hFuente;
HFONT hSmallFuente;

#ifdef DEPURANDO
//Variables para calcular cuadros por segundo
DWORD FPS_tUltimo = 0;
DWORD FPS_Suma    = 0;
DWORD FPS_Actual  = 0;
#endif // DEPURANDO

//Variables globales relacionadas con la ventana
const char* g_appName = "Editor de niveles NICHOBROS";
bool        g_active;

// Variables globales del editor
eExtraInfo DA = inQuesoRemachado;
int  CA = 0;
int  EinCat = 0;
int  MX = 0,MY = 0;
int  EX = 0,EY = 0;
bool Editando  = false;
bool Llenando  = false;
bool Eventeando  = false;
bool MasActivo = true;
bool Tapador   = false;
int  Ancho;
int  LX,LY;
char Nombre     [MAXNOMBRE] = "";
char NombreNivel[MAXNOMBRE] = "";
char FondoNivel [MAXNOMBRE] = "FONDO.GNB";
char ETexto     [MAXETEXTO] = "";
Mapa *Nivel = NULL;
enum eCampo {camNulo = 0, camNombreNivel = 1, 
			 camAncho = 2, camFondoNivel = 3};
eCampo Campo = camNulo;
char CadEventos [NUM_EVENTOS][64] = {"Inicio Nicho","Enemigo Juanchi",
							         "Zona Salida","Parar Enemigo",
									 "Piso Fragil","Piso Rigido",
									 "Premio Speed","Enemigo Tortuga",
									 "Bloque Moneda","Premio Moneda",
									 "Premio BolaFuego","Rebotador Simple",
									 "Solo Parriba","Premio Escudo",
									 "Bola Saltarina","Zona Duele",
									 "Premio Aureola","Zona DueRig",
									 "Puente Cae", "Enemigo VerdeLoco",
									 "Puente Flojo", "Zona Generador", 
									 "Jefe Robots", "Marcador Candado",
									 "Aparejo Escalera", "Plataforma Cae",
									 "Pinche Palo", "Prensa Triturin",
									 "Plataforma Lateral", "Caecucho Izquierda",
									 "Caecucho Derecha", "Aparejo Teleporter",
									 "Boton Rojo", "Boton Cyan", 
									 "Puerta Roja", "Puerta Cyan",
									 "Plataforma Vertical", "Enemigo Sauriolo",
									 "Premio Laser", "Premio Cereza",
									 "Premio Frutilla", "Premio Juguito", 
									 "Premio Banana", "Premio PizzaVeggy",
									 "Premio Licuado", "Premio Pizza", 
									 "Premio PizzaCompleta", "Premio Radio",
									 "Enemigo Caparatuzo", "Premio LiqVerde",
									 "Enemigo Bichoto","Pez Saltarin"};

int eventoCategoria[NUM_EVENTOS+1] =
   {1,1,2,1,1,3,3,4,2,4,4,4,1,3,4,2,1,4,1,3,2,3,2,2,1,3,3,2,2,3,2,2,1,1,1,1,1,3,2,4,4,4,4,4,4,4,4,4,4,2,4,2,2};

COLORREF ColDif[4][2];
                         
char NomDif[4][25] = {"Queso Remachado",
			    	  "Newby",
		    		  "Jueguero Decente",
		    		  "Cyber Adicto"};

//Prototipos de funciones relacionadas con DirectDraw
//HRESULT             InicializarDD();
bool             InicializarSuperficies();
LPDIRECTDRAWSURFACE CrearSuperficieDesdeBitmap(LPCSTR);
void				ActualizarJuego();
void				InicializarJuego();
void                Blt();


void Guardar()
{
	fstream k;
	char auxcad[200] = "Niveles/Usuario/";
	strcat(auxcad,Nombre);
	strcat(auxcad,".NNB");
	k.open(auxcad,ios::out | ios::binary);
	k.write((char *)&Ancho,2);
	k.write(NombreNivel,strlen(NombreNivel));
	k.put('\0');
	k.write(FondoNivel,strlen(FondoNivel));
	k.put('\0');
	unsigned char Valor;
	// Ahora, el peso pesado: escribiremos el archivo.
	int X, Y;
	for (X = 0; X < Ancho; X++)
		for (Y = 0; Y < 20; Y++)
		{
			Valor = Nivel->Terreno(X,Y);
			if (Valor >= 26)
				Valor++;
			k.put(Valor);

			Valor = Nivel->Eventos(X,Y);
			if (Valor >= 26)
				Valor++;
			k.put(Valor);

			Valor = Nivel->ExtraInfo(X,Y);
			if (Valor >= 26)
				Valor++;
			k.put(Valor);
		}
	k.close();
}

void BufferPaint(long X , long Y , LPDIRECTDRAWSURFACE Surf, int Ancho, int Alto)
{	
	// Esta subrutina nos hace la vida mas facil, ya que se
    // encarga de operaciones de bajo nivel automaticamente,
    // pero sobre todo se encarga de la tediosa tarea del
    // CLIPPING.
	
    RECT J;
	long Resta;
	
	// Clipping izquierdo
	if (X < 0)
	{
		J.left = abs(X);
		X = 0;
	}
	else
	    J.left = 0;
	// Clipping superior
	if (Y < 0)
	{
		J.top = abs(Y);
	    Y = 0;
	}
	else
	    J.top = 0;
	// Clipping Derecho
	Resta = X + Ancho - 800;
	if (Resta > 0)
		J.right = Ancho - Resta;
	else
		J.right = Ancho;
	// Clipping Inferior
	Resta = Y + Alto - 600;
	if (Resta > 0)
		J.bottom = Alto - Resta;
	else
		J.bottom = Alto;
	// Finalmente, dibujamos en el backbuffer
	BltFast(SDL::screen(),X, Y, Surf, &J);
}

void DescargarSuperficie(LPDIRECTDRAWSURFACE &Surf)
{
	if (Surf != NULL)
	{
		SDL::freeSurface(Surf);
		Surf = NULL;
	}
}

bool InicializarSDL()
{ 
    if (!SDL::init(800, 600, 16,true, "Nichobros editor de niveles"))
	{
		std::cerr << "SDL::init() trouble." << std::endl;
		return false;
	}
    
    if (!SDL::initTTF())
    {
        std::cerr << "SDL::initTTF() trouble." << std::endl;
        return false;
    }
    
    hFuente = NULL;
    hSmallFuente = NULL;
    
    //Open the fonts
	if ((hFuente = SDL::openFont( "Fuentes/COMIC.TTF", 20 )) == NULL ||
        (hSmallFuente = SDL::openFont( "Fuentes/COMIC.TTF", 11 )) == NULL)
    {
        std::cerr << "SDL::openFont() trouble." << std::endl;
        return false;
    }
    
    SDL_ShowCursor(SDL_DISABLE);
    
    return true;
} 



void FinalizarSDL()
{
 	if (Nivel != NULL)
	{
		delete Nivel;
		Nivel = NULL;
	}

    // Asignamos a NULL aquellos punteros que en realidad
    // apuntan a otro fijo, es decir, son variables y no
    // sería correcto descargar la superficie a la que
    // apuntan, ya que esta se descargaría dos veces, pues
    // existe un puntero fijo apuntando siempre a esta
    // superficie.
    imglstTerreno   [caBloqueInterr - 1] = NULL;
    imglstTerreno   [caFuegoLava    - 1] = NULL;
    imglstTerreno   [caCalentador   - 1] = NULL;
    int k;
  
    // Empezamos a descargar las verdaderas imagenes

    // imglstTerreno
    for (k = 0; k < TAMANO_TERRENO; k++)
        DescargarSuperficie(imglstTerreno[k]);

     // Limpieza TTF
    
    SDL::closeFont(hFuente);
    SDL::closeFont(hSmallFuente);
    
    SDL::quitTTF();
    
    // Nos vamos final!
    
    SDL::quit();
}



bool InicializarSuperficies()
{ 
    lpDDSCursor  = CrearSuperficieDesdeBitmap("Graficos/Cursores/Cursor.gnb");
	lpDDSRecRojo = CrearSuperficieDesdeBitmap("Graficos/Recuadros/RecRojo.gnb");
    
// Creacion de superficies del juego.
    
	// Barras con el cansancio.

    #define check(x) if ((x) == NULL) {std::cerr << "Error al cargar la imagen." << std::endl; std::cerr << #x << std::endl; return false;}

    // imglstTerreno
    check(imglstTerreno[0]  = CrearSuperficieDesdeBitmap("Graficos/Pisos/Comun.gnb"));
    check(imglstTerreno[1]  = CrearSuperficieDesdeBitmap("Graficos/Pisos/Metal.gnb"));
    check(imglstTerreno[2]  = CrearSuperficieDesdeBitmap("Graficos/Pisos/Piedra.gnb"));
    check(imglstTerreno[3]  = CrearSuperficieDesdeBitmap("Graficos/Carteles/Salida.gnb"));
    check(imglstTerreno[4]  = CrearSuperficieDesdeBitmap("Graficos/Bloques/Interrogacion.gnb"));
    check(imglstTerreno[5]  = CrearSuperficieDesdeBitmap("Graficos/Bloques/Simple.gnb"));
	check(imglstTerreno[6]  = CrearSuperficieDesdeBitmap("Graficos/Bloques/Anulado.gnb"));
    check(imglstTerreno[7]  = CrearSuperficieDesdeBitmap("Graficos/Fuego/Lava.gnb"));
	check(imglstTerreno[8]  = CrearSuperficieDesdeBitmap("Graficos/Pasto/PastoIzquierda.gnb"));
	check(imglstTerreno[9]  = CrearSuperficieDesdeBitmap("Graficos/Pasto/PastoMedio.gnb"));
	check(imglstTerreno[10] = CrearSuperficieDesdeBitmap("Graficos/Pasto/PastoDerecha.gnb"));
    check(imglstTerreno[11] = CrearSuperficieDesdeBitmap("Graficos/CosasMalas/Calentador.gnb"));
	check(imglstTerreno[12] = CrearSuperficieDesdeBitmap("Graficos/Bloques/Puente.gnb"));
	check(imglstTerreno[13] = CrearSuperficieDesdeBitmap("Graficos/Bloques/Puente2.gnb"));
	check(imglstTerreno[14] = CrearSuperficieDesdeBitmap("Graficos/Nubes/Nube1.gnb"));
	check(imglstTerreno[15] = CrearSuperficieDesdeBitmap("Graficos/Nubes/Nube2.gnb"));
	check(imglstTerreno[16] = CrearSuperficieDesdeBitmap("Graficos/Nubes/Nube3.gnb"));
	check(imglstTerreno[17] = CrearSuperficieDesdeBitmap("Graficos/Nubes/Nube4.gnb"));
	check(imglstTerreno[18] = CrearSuperficieDesdeBitmap("Graficos/Nubes/Nube5.gnb"));
	check(imglstTerreno[19] = CrearSuperficieDesdeBitmap("Graficos/Nubes/Nube6.gnb"));
	check(imglstTerreno[20] = CrearSuperficieDesdeBitmap("Graficos/CosasMalas/GeneradorIz.gnb"));
	check(imglstTerreno[21] = CrearSuperficieDesdeBitmap("Graficos/CosasMalas/GeneradorMed.gnb"));
	check(imglstTerreno[22] = CrearSuperficieDesdeBitmap("Graficos/CosasMalas/GeneradorDe.gnb"));
	check(imglstTerreno[23] = CrearSuperficieDesdeBitmap("Graficos/Aparejos/EscaleraArI.gnb"));
	check(imglstTerreno[24] = CrearSuperficieDesdeBitmap("Graficos/Aparejos/EscaleraI.gnb"));
	check(imglstTerreno[25] = CrearSuperficieDesdeBitmap("Graficos/Aparejos/EscaleraAbI.gnb"));
	check(imglstTerreno[26] = CrearSuperficieDesdeBitmap("Graficos/Aparejos/EscaleraArD.gnb"));
	check(imglstTerreno[27] = CrearSuperficieDesdeBitmap("Graficos/Aparejos/EscaleraD.gnb"));
	check(imglstTerreno[28] = CrearSuperficieDesdeBitmap("Graficos/Aparejos/EscaleraAbD.gnb"));
    check(imglstTerreno[29] = CrearSuperficieDesdeBitmap("Graficos/Pasto/PalmeraDER.gnb"));
    check(imglstTerreno[30] = CrearSuperficieDesdeBitmap("Graficos/Pasto/PalmeraIZQ.gnb"));
    check(imglstTerreno[31] = CrearSuperficieDesdeBitmap("Graficos/Pasto/PalmeraCEN.gnb"));
    check(imglstTerreno[32] = CrearSuperficieDesdeBitmap("Graficos/Pasto/TroncoPalmera.gnb"));
    
    check(imglstTerreno[33] = CrearSuperficieDesdeBitmap("Graficos/Playa/Arena.gnb"));
    check(imglstTerreno[34] = CrearSuperficieDesdeBitmap("Graficos/Playa/Arena2.gnb"));
    check(imglstTerreno[35] = CrearSuperficieDesdeBitmap("Graficos/Playa/PiedraFragil.gnb"));
    check(imglstTerreno[36] = CrearSuperficieDesdeBitmap("Graficos/Playa/PiedraFragilFUEGO.gnb"));
    check(imglstTerreno[37] = CrearSuperficieDesdeBitmap("Graficos/Playa/AGUA1.gnb"));
    check(imglstTerreno[38] = CrearSuperficieDesdeBitmap("Graficos/Pasto/Pastito.gnb"));
    check(imglstTerreno[39] = CrearSuperficieDesdeBitmap("Graficos/Pasto/PastitoCLARO.gnb"));
    check(imglstTerreno[40] = CrearSuperficieDesdeBitmap("Graficos/Pasto/ARBOLCEN.gnb"));
    check(imglstTerreno[41] = CrearSuperficieDesdeBitmap("Graficos/Pasto/ARBOLDER.gnb"));
    check(imglstTerreno[42] = CrearSuperficieDesdeBitmap("Graficos/Pasto/ARBOLIZQ.gnb"));
    check(imglstTerreno[43] = CrearSuperficieDesdeBitmap("Graficos/Pasto/ARBOLTRONCO.gnb"));
    check(imglstTerreno[44] = CrearSuperficieDesdeBitmap("Graficos/Pasto/MADERA.gnb"));
    
    check(imglstTerreno[45] = CrearSuperficieDesdeBitmap("Graficos/Tela/Tela.gnb"));
    check(imglstTerreno[46] = CrearSuperficieDesdeBitmap("Graficos/Tela/TelaABAJO.gnb"));
    check(imglstTerreno[47] = CrearSuperficieDesdeBitmap("Graficos/Tela/TelaARRIBA.gnb"));
    check(imglstTerreno[48] = CrearSuperficieDesdeBitmap("Graficos/Premios/Machete.gnb"));
    
	// Nos vamos...
    return true;
    #undef check
}

LPDIRECTDRAWSURFACE CrearSuperficieDesdeBitmap(LPCSTR pathBMP)
{
    return SDL::loadImage(pathBMP,true,0,0,0);
}

void EscribirEnSuperficie(LPDIRECTDRAWSURFACE lpSup, int x, int y, LPCSTR texto, COLORREF color, HFONT fuente,COLORREF colorsombra= (COLORREF){10,10,10})
{ 
    SDL_Surface *imgTexto = SDL::renderTextSolid(fuente,texto,color);
    SDL_Surface *imgSombra = SDL::renderTextSolid(fuente,texto,colorsombra);
    SDL::blit(x+3,y+3,imgSombra,lpSup);
    SDL::blit(x,y,imgTexto,lpSup);
    SDL::freeSurface(imgTexto);
    SDL::freeSurface(imgSombra);
    /*
    HDC hdcSup;

    lpSup->GetDC(&hdcSup);
    SelectObject(hdcSup, fuente);
    SetBkMode(hdcSup, TRANSPARENT);
	SetTextColor(hdcSup, colorsombra);
	TextOut(hdcSup, x + 3, y + 3, texto, lstrlen(texto));
    SetTextColor(hdcSup, color);
    TextOut(hdcSup, x, y, texto, lstrlen(texto));
    lpSup->ReleaseDC(hdcSup); 
    */
}

void InicializarJuego()
{
    catActual = 0;
	Llenando = false;
    Eventeando = false;
	Editando = true;
	strcpy(ETexto,"");
	EX = 50;
	EY = ALTOBARRA * 30 + 20;
}

int EA()
{
    int c = 0;
    for(int i=0;;i++)
    {
        if (eventoCategoria[i] == catActual)
        {
            if (c == EinCat)
                return i;
            c++;
        }
    }
}

void ActualizarJuego()
{

	if (!Editando && strlen(Nombre) == 0)
	{
		strcpy(Nombre,ETexto);
		Editando = true;
		strcpy(ETexto,"");
		EX = 50;
		EY = ALTOBARRA * 30 + 20;
	}
	else if (!Editando && strlen(NombreNivel) == 0)
	{
		fstream k;
		char auxcad[200] = "Niveles/Usuario/";
		switch (ETexto[0])
		{
			case 'S':
			case 's':
				Ancho = 60; // Default values!
				strcpy(NombreNivel,"NivelNichoBros");
				if (Nivel != NULL)
				{
					delete Nivel;
					Nivel = NULL;
				}
				Nivel = new Mapa(Ancho);
				LX = 0;
				LY = 0;
				break;
			case 'N':
			case 'n':
				strcat(auxcad,Nombre);
				strcat(auxcad,".NNB");
				k.open(auxcad,ios::in | ios::binary);
                if (k.fail())
                {
                    cerr << "Error al abrir el archivo " << auxcad << endl;
                    bEjecutandose = false;
                    return;
                }
				k.read((char *)&Ancho,2);
				strcpy(NombreNivel,"");
				k.get(NombreNivel,MAXNOMBRE,'\0');
				k.ignore();
				strcpy(FondoNivel,"");
				k.get(FondoNivel,MAXNOMBRE,'\0');
				k.ignore();
				// Ahora, el peso pesado: leeremos el archivo.
				char Valor;
				if (Nivel != NULL)
				{
					delete Nivel;
					Nivel = NULL;
				}
				Nivel = new Mapa(Ancho);
				int X, Y;
			    for (X = 0; X < Ancho; X++)
			        for (Y = 0; Y < 20; Y++)
					{
			            k.get(Valor);
						if (Valor > 26)
							Valor--;
			            Nivel->SetTerreno(X, Y,(eCasillas)Valor);
						
						k.get(Valor);
						if (Valor > 26)
							Valor--;
						Nivel->SetEventos(X, Y,(eEventos)Valor);
						
						k.get(Valor);
						if (Valor > 26)
							Valor--;
						Nivel->SetExtraInfo(X,Y,(eExtraInfo)Valor);
					}
			    k.close();
                Nivel->storyClear();
				LX = 0;
				LY = 0;
				break;
			default:
				Editando = true;
				strcpy(ETexto,"");
				EX = 50;
				EY = ALTOBARRA * 30 + 20;
				break;
		}
	}
	else if (!Editando && Campo != camNulo)
		switch (Campo)
		{
			case camNombreNivel:
				Campo    = camAncho;
				Editando = true;
				strcpy(NombreNivel,ETexto);
				strcpy(ETexto,"");
				break;
			case camAncho:
                int LastAncho;
                LastAncho = Ancho;
                if (sscanf(ETexto,"%d",&Ancho) == 1)
                {
					Campo    = camFondoNivel;
                    Editando = true;
                    Mapa *AuxNivel;
                    AuxNivel = new Mapa(Ancho);
                    int X,Y;
                    for (X = 0;X < Ancho;X++)
                        for (Y = 0;Y < 20; Y++)
                            if (X < LastAncho)
                            {
                                AuxNivel->SetTerreno  (X,Y,Nivel->Terreno  (X,Y));
                                AuxNivel->SetEventos  (X,Y,Nivel->Eventos  (X,Y));
                                AuxNivel->SetExtraInfo(X,Y,Nivel->ExtraInfo(X,Y));
                            }
                            else
                            {
                                AuxNivel->SetTerreno  (X,Y,caVacia);
                                AuxNivel->SetEventos  (X,Y,evNulo);
                                AuxNivel->SetExtraInfo(X,Y,(eExtraInfo)(inQuesoRemachado | inDifMas));
                            }
                    delete Nivel;
                    Nivel = AuxNivel;
                }
                else
                    Ancho = LastAncho;
				strcpy(ETexto,"");
				break;
			case camFondoNivel:
				Campo    = camNulo;
				Editando = false;
				strcpy(FondoNivel,ETexto);
                strcat(FondoNivel,".GNB");
				strcpy(ETexto,"");
				break;
            default:
                break;
		}

	if (Llenando)
	{
		int CX,CY;
		CX = MX / 30 + LX;
		CY = (MY - ALTOBARRA * 30) / 30 + LY;
		if (CX >= 0 && CY >= 0 && CX < Ancho && CY < 20)
			Nivel->SetTerreno(CX,CY,(eCasillas)CA);
	}

    if (Eventeando)
    {
        int CX,CY;
        
        CX = MX / 30 + LX;
        CY = (MY - ALTOBARRA * 30) / 30 + LY;
        if (CX >= 0 && CY >= 0 && CX < Ancho && CY < 20)
        {
            Nivel->SetEventosAndExtraInfo(CX,CY,(eEventos)EA(),(eExtraInfo)(((MasActivo)?(inDifMas ):(0)) | 
                                                                         ((Tapador  )?(inTapador):(0)) | DA));
        }
    }

#ifdef DEPURANDO
	// Avanzamos la cuenta de cuadros por segundo
	if (TiempoDesde(FPS_tUltimo) >= 1000)
	{ 
		FPS_Actual = FPS_Suma;
		FPS_Suma = 0;
		FPS_tUltimo = GetTickCount();
	} 
	FPS_Suma++;
#endif // DEPURANDO
}

void Dibujar()
{
	int X,Y;
	for (X = LX; X < LX + 27; X++)
		for (Y = LY;Y < LY + 20; Y++)
			if (X < Ancho && Y < 20)
			{
                
				if (Nivel->Terreno(X,Y) != caVacia)
					BufferPaint((X - LX) * 30,ALTOBARRA * 30 + (Y - LY) * 30,
						        imglstTerreno[Nivel->Terreno(X,Y) - 1],
								30,30);
				if ((Nivel->ExtraInfo(X,Y) & inTapador) != 0)
					BufferPaint((X - LX) * 30,ALTOBARRA * 30 + (Y - LY) * 30,
								lpDDSRecRojo,
								30,30);
                
				if (Nivel->Eventos(X,Y) != evNulo)
                {
					if (Nivel->Eventos(X,Y) <= NUM_EVENTOS)
					{
						char *auxchar;
						auxchar  = strchr(CadEventos[Nivel->Eventos(X,Y) - 1],' ');
						*auxchar = '\0';
						EscribirEnSuperficie(SDL::screen(),
										     (X - LX) * 30 + 1,ALTOBARRA * 30 + (Y - LY) * 30 + 1,
											 CadEventos[Nivel->Eventos(X,Y) - 1],
											 ColDif[Nivel->ExtraInfo(X,Y) % 4][((Nivel->ExtraInfo(X,Y) & inDifMas) != 0)?(1):(0)],
											 hSmallFuente);
                        COLORREF colorcitojejejeje = ColDif[Nivel->ExtraInfo(X,Y) % 4][((Nivel->ExtraInfo(X,Y) & inDifMas) != 0)?(1):(0)];
						if (colorcitojejejeje.r != 255 ||
                            colorcitojejejeje.b != 255 ||
                            colorcitojejejeje.g != 255)
							EscribirEnSuperficie(SDL::screen(),
												(X - LX) * 30 + 11,ALTOBARRA * 30 + (Y - LY) * 30 + 5,
												((Nivel->ExtraInfo(X,Y) & inDifMas) != 0)?("+"):("-"),RGB(255,255,255),hFuente);
						EscribirEnSuperficie(SDL::screen(),
										     (X - LX) * 30 + 1,ALTOBARRA * 30 + (Y - LY) * 30 + 15,
											 auxchar + 1,
											 ColDif[Nivel->ExtraInfo(X,Y) % 4][((Nivel->ExtraInfo(X,Y) & inDifMas) != 0)?(1):(0)],
											 hSmallFuente);
						*auxchar = ' ';
					}
					else
					{
						char auxcad[50];
						itoa(Nivel->Eventos(X,Y),auxcad);
						EscribirEnSuperficie(SDL::screen(),
										     (X - LX) * 30 + 1,ALTOBARRA * 30 + (Y - LY) * 30 + 1,
											 auxcad,
											 ColDif[Nivel->ExtraInfo(X,Y) % 4][((Nivel->ExtraInfo(X,Y) & inDifMas) != 0)?(1):(0)],
											 hSmallFuente);
                        COLORREF colorcitojejejeje = ColDif[Nivel->ExtraInfo(X,Y) % 4][((Nivel->ExtraInfo(X,Y) & inDifMas) != 0)?(1):(0)];
                        if (colorcitojejejeje.r != 255 ||
                            colorcitojejejeje.b != 255 ||
                            colorcitojejejeje.g != 255)
							EscribirEnSuperficie(SDL::screen(),
									            (X - LX) * 30 + 11,ALTOBARRA * 30 + (Y - LY) * 30 + 5,
												((Nivel->ExtraInfo(X,Y) & inDifMas) != 0)?("+"):("-"),RGB(255,255,255),hFuente);
					}
                }
			}
			else
			{
				// Dibujamos un rectangulo negro
				RECT    AuxR;
				AuxR.top    = ALTOBARRA * 30 + (Y - LY) * 30;
				AuxR.left   = (X - LX) * 30;
				AuxR.bottom = AuxR.top + 30;
				AuxR.right  = AuxR.left + 30;
				if (AuxR.right > 800) 
					AuxR.right = 800;
                DrawRectangle(SDL::screen(),&AuxR,RGB(0,0,0));
			}
}

void Blt()
{ 
    // Dibujamos la escena...

	// Barras...
	RECT    AuxR;
	
	AuxR.top    = AuxR.left = 0;
	AuxR.bottom = ALTOBARRA * 30;
	AuxR.right  = 800;
    DrawRectangle(SDL::screen(),&AuxR,i2col(COLORBARRA));
    
    AuxR.left = 0;
    AuxR.right = 800;
	AuxR.top = ALTOBARRA * 30;
	AuxR.bottom = 600;
    DrawRectangle(SDL::screen(),&AuxR,i2col(TCOLOR));

	// Hay que elegir nivel?

	if (strlen(Nombre) == 0)
		EscribirEnSuperficie(SDL::screen(),50,ALTOBARRA * 30 + 5,"Introduzca el nombre del archivo de nivel:",RGB(255,255,255),hFuente);
	else if (strlen(NombreNivel) == 0)
		EscribirEnSuperficie(SDL::screen(),50,ALTOBARRA * 30 + 5,"¿Desea crear un nivel nuevo?",RGB(255,255,255),hFuente);
	else if (Editando && Campo != camNulo)
		switch (Campo)
		{
			case camNombreNivel:
				EscribirEnSuperficie(SDL::screen(),50,ALTOBARRA * 30 + 5,"Introduzca el nombre del nivel",RGB(255,255,255),hFuente);
				break;
			case camAncho:
				EscribirEnSuperficie(SDL::screen(),50,ALTOBARRA * 30 + 5,"Introduzca el ancho del nivel",RGB(255,255,255),hFuente);
				break;
			case camFondoNivel:
				EscribirEnSuperficie(SDL::screen(),50,ALTOBARRA * 30 + 5,"Introduzca el nombre del archivo de fondo del nivel",RGB(255,255,255),hFuente);
				break;
            default:
                break;
		}
	else
		Dibujar();

	// Muestra el texto que se esta entrando.

	if (Editando)
		EscribirEnSuperficie(SDL::screen(),EX,EY,ETexto,RGB(255,20,20),hFuente);

	// Eventos ,casilla y dificultad activos.

	if (CA != caVacia)
		BufferPaint(5,5,imglstTerreno[CA - 1],30,30);
	else
	{
		RECT    AuxR;
		AuxR.top    = AuxR.left = 5;
		AuxR.bottom = 35;
		AuxR.right  = 35;
        DrawRectangle(SDL::screen(),&AuxR,i2col(TCOLOR));
	}

    EscribirEnSuperficie(SDL::screen(),95,5,NombreCat[catActual],RGB(0,128,255),hFuente);

	if (EA() != evNulo)
		EscribirEnSuperficie(SDL::screen(),200,5,CadEventos[EA() - 1],RGB(0,0,255),hFuente);
	else
		EscribirEnSuperficie(SDL::screen(),200,5,"Evento Nulo",RGB(0,0,255),hFuente);

	EscribirEnSuperficie(SDL::screen(),500,5,NomDif[DA],RGB(0,0,255),hFuente);

	// Tapador? Más? SI/NO

	if (Tapador)
		EscribirEnSuperficie(SDL::screen(),200,25,"Tapador",RGB(0,0,255),hFuente);

	if (MasActivo)
		EscribirEnSuperficie(SDL::screen(),300,25,"Más",RGB(0,0,255),hFuente);

	// Propiedades del nivel.

	char CadenaDatos[250];
	char AuxCadAncho[10 ];

	strcpy(CadenaDatos,"Nombre:");
	strcat(CadenaDatos,NombreNivel);
	strcat(CadenaDatos,"  Ancho:");
	itoa(Ancho,AuxCadAncho);
	strcat(CadenaDatos,AuxCadAncho);
	strcat(CadenaDatos,"  Archivo de fondo:");
	strcat(CadenaDatos,FondoNivel);

	EscribirEnSuperficie(SDL::screen(),0,50,CadenaDatos,RGB(255,50,50),hFuente);

	// Cursor...

	BufferPaint(MX,MY,lpDDSCursor,21,21);

#ifdef DEPURANDO
	//Escribo Cuadros por segundo
    char Mensaje[25], FPSchar[3];
    _ultoa(FPS_Actual, FPSchar, 10);
    strcpy(Mensaje, "Cuadros por Segundo = ");
    strcat(Mensaje, FPSchar);
    EscribirEnSuperficie(lpDDSSec, 0, 0, LPCSTR(Mensaje), RGB(250,200,100), hFuente);
#endif // DEPURANDO

    //Intercambio las superficies Primaria y Secundaria
    //lpDDSPri->Flip(NULL, DDFLIP_WAIT);
    SDL::flip();
}

vector<vector<bool> > alcanzable;

const int dx[4] = {0,0,-1,1};
const int dy[4] = {1,-1,0,0};

eCasillas casFill;
eEventos evFill, evInitFill;
eExtraInfo extraFill;

void floodFill(int X, int Y)
{
    alcanzable[X][Y] = true;
    if (Nivel->Eventos(X,Y) == evInitFill)
        Nivel->SetEventosAndExtraInfo(X,Y,evFill,extraFill);
    for(int i=0;i<4;i++)
    {
        const int nx = X+dx[i];
        const int ny = Y+dy[i];
        if (0 <= nx && nx < Ancho && 0 <= ny && ny < 20 && !alcanzable[nx][ny] && Nivel->Terreno(nx,ny) == casFill)
            floodFill(nx,ny);
    }
}

void procesarEvento (const SDL_Event &ev)
{
    static bool shiftPressed = false;
    switch(ev.type)
    {
    case SDL_QUIT:
        FinalizarSDL();
        g_active = false;
        bEjecutandose = false;
        break;
    case SDL_KEYUP:
        switch(ev.key.keysym.sym)
        {
            case SDLK_LSHIFT:
            case SDLK_RSHIFT:
                shiftPressed = false;
                break;
            case SDLK_e:
                Eventeando = false;
                break;
            default:
                break;
        }
        break;
	case SDL_KEYDOWN:
        if (ev.key.keysym.sym == SDLK_PRINT)
        {
            char aux[256];
            const int MAX = 1000;
            struct stat st;
            int i;
            for(i=0;i<MAX;i++)
            {
                sprintf(aux,"pantallita-%d.bmp",i);
                if (stat(aux,&st) != 0) break;
            }
            if (i < MAX)
                SDL_SaveBMP(SDL::screen(),aux);
        }
        if (ev.key.keysym.sym == SDLK_LSHIFT ||
            ev.key.keysym.sym == SDLK_RSHIFT)
            shiftPressed = true;
		if (Editando)
		{
            int w = ev.key.keysym.sym;
			if ((w >= SDLK_a && w <= SDLK_z) ||
				(w >= SDLK_0 && w <= SDLK_9))
			{
				char AuxChar[2];
                if (shiftPressed)
                    AuxChar [0] = toupper((char)w);
                else
                    AuxChar [0] = (char)w;
				AuxChar [1] = '\0';
				strcat(ETexto,AuxChar);
			}
			else if (w == SDLK_SPACE)
				strcat(ETexto," ");
			else if (w == SDLK_BACKSPACE && strlen(ETexto) > 0)
				ETexto[strlen(ETexto) - 1] = '\0';
			else if (w == SDLK_RETURN)
				Editando = false;
		}
		else
		{
            int w = ev.key.keysym.sym;
			int CX,CY;
			switch (w)
			{
                case SDLK_1:
                    catActual = 0;
                    EinCat = 0;
                    break;
                case SDLK_2:
                    catActual = 1;
                    EinCat = 0;
                    break;
                case SDLK_3:
                    catActual = 2;
                    EinCat = 0;
                    break;
                case SDLK_4:
                    catActual = 3;
                    EinCat = 0;
                    break;
                case SDLK_k:
                    CX = MX / 30 + LX;
					CY = (MY - ALTOBARRA * 30) / 30 + LY;
                    if (CX >= 0 && CY >= 0 && CX < Ancho && CY < 20)
                    {
                        alcanzable = vector<vector<bool> >(Ancho,vector<bool>(20,false));
                        casFill = Nivel->Terreno(CX,CY);
                        evInitFill = Nivel->Eventos(CX,CY);
                        evFill = (eEventos)EA();
                        extraFill = (eExtraInfo)(((MasActivo)?(inDifMas ):(0)) | 
                                                 ((Tapador  )?(inTapador):(0)) | DA);
                        floodFill(CX,CY);
                    }
                    break;
                case SDLK_z:
                    Nivel->Undo();
                    break;
                case SDLK_x:
                    Nivel->Redo();
                    break;
                case SDLK_v:
                    for(int i=0;i<100;i++)
                        Nivel->Undo();
                    break;
                case SDLK_b:
                    for(int i=0;i<100;i++)
                        Nivel->Redo();
                    break;
                case SDLK_SPACE:
                    // Esto no compila en windows.
                    // Cambiar por lo siguiente:
                    //EscribirEnSuperficie(SDL::screen(),50,250,"Funcion no disponible en windows.",RGB(255,255,255),hFuente);
					//SDL::flip();
					//t0 = SDL::getTicks();
					//while (SDL::getTicks() - t0 < 1500);
                    Guardar();
                    FinalizarSDL();
                    execlp("./Nichobros","Nichobros",NULL);
                    cerr << "Che no pude correr el Nichobros" << endl;
                    exit(1);
                    break;
				case SDLK_ESCAPE:
					Guardar();
					bEjecutandose = false;
					break;
				case SDLK_RIGHT:
					if (LX < Ancho - 1)
						LX++;
					break;
				case SDLK_LEFT:
					if (LX > 0)
						LX--;
					break;
				case SDLK_PAGEDOWN:
					if (LX < Ancho - 30)
						LX += 30;
					else
						LX = Ancho - 1;
					break;
				case SDLK_PAGEUP:
					if (LX > 29)
						LX -= 30;
					else
						LX = 0;
					break;
				case SDLK_UP:
					if (LY > 0)
						LY--;
					break;
				case SDLK_DOWN:
					if (LY < 19)
						LY++;
					break;
				case SDLK_q:
					if (CA < TAMANO_TERRENO)
						CA++;
					break;
				case SDLK_a:
					if (CA > 0)
						CA--;
					break;
				case SDLK_w:
					if (EinCat+1 < tamCategoria[catActual])
						EinCat++;
					break;
				case SDLK_s:
					if (EinCat > 0)
						EinCat--;
					break;
				case SDLK_e:
					Eventeando = true;
					break;
				case SDLK_p:
					Editando = true;
					Campo	 = camNombreNivel;
					EX		 = 5;
					EY		 = ALTOBARRA * 30 + 50;
					break;
				case SDLK_m:
					MasActivo = !MasActivo;
					break;
				case SDLK_t:
					Tapador = !Tapador;
					break;
				case SDLK_d:
					if (DA != inCyberAdicto)
						DA = (eExtraInfo)(DA + 1);
					break;
				case SDLK_c:
					if (DA != inQuesoRemachado)
						DA = (eExtraInfo)(DA - 1);
					break;
				case SDLK_r:
					CX = MX / 30 + LX;
					CY = (MY - ALTOBARRA * 30) / 30 + LY;
					if (CX >= 0 && CY >= 0 && CX < Ancho && CY < 20)
                    {
                        int ev = Nivel->Eventos(CX,CY);
                        for(int i=0;i<CANT_CATEGORIAS;i++)
                        for(int j=0;j<tamCategoria[i];j++)
                        {
                            catActual = i;
                            EinCat = j;
                            if (EA() == ev)
                                goto salida;
                        }
salida:;
                    }
					break;
				case SDLK_f:
					CX = MX / 30 + LX;
					CY = (MY - ALTOBARRA * 30) / 30 + LY;
					if (CX >= 0 && CY >= 0 && CX < Ancho && CY < 20)
						CA = Nivel->Terreno(CX,CY);
					break;
				case SDLK_g:
					Guardar();
					break;
			}
		}
		break;
	case SDL_MOUSEMOTION:
		MX = ev.motion.x;
		MY = ev.motion.y;
		break;
	case SDL_MOUSEBUTTONDOWN:
        if (ev.button.button == SDL_BUTTON_LEFT)
        {
            MX = ev.button.x;
            MY = ev.button.y;
            Llenando = true;
        }
		break;
	case SDL_MOUSEBUTTONUP:
        if (ev.button.button == SDL_BUTTON_LEFT)
        {
            MX = ev.button.x;
            MY = ev.button.y;
            Llenando = false;
        }
		break;
    }
}




int main(int argc, char *args[])
{
    for(int i=0;i<= NUM_EVENTOS; i++)
        eventoCategoria[i]--;
        
    for(int i=0;i<CANT_CATEGORIAS;i++)
    {
        tamCategoria[i] = 0;
        for(int j=0;j<=NUM_EVENTOS;j++)
            tamCategoria[i] += eventoCategoria[j] == i;
    }
    g_active = true;
    
    #define zMem(c) memset((c),0,sizeof(c))
    zMem(imglstTerreno);
    
    ColDif[0][0] = i2col(0x00FF00);
    ColDif[0][1] = i2col(0xFFFFFF);
    ColDif[1][0] = i2col(0xFF0000);
    ColDif[1][1] = i2col(0xFF0000);
    ColDif[2][0] = i2col(0x00FFFF);
    ColDif[2][1] = i2col(0x00FFFF);
    ColDif[3][0] = i2col(0xFFFFFF);
    ColDif[3][1] = i2col(0x0000FF);
    
    
    //Inicio SDL
    if(!InicializarSDL())
    {
        std::cerr << "Fallo en la inicializacion de la SDL." << std::endl;
        FinalizarSDL();
        return -1;
    }

    // Inicio Superficies
    if(!InicializarSuperficies())
    {
        std::cerr << "Fallo en la inicializacion de las superficies." << std::endl;
        FinalizarSDL();
        return -1;
    }

    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY , SDL_DEFAULT_REPEAT_INTERVAL);

    //Bucle principal

	InicializarJuego();

    SDL_Event msg;

	bEjecutandose = true;
    while (bEjecutandose == true)
    {
        if (SDL::pollEvent(msg))
        {
            if (msg.type == SDL_QUIT) break;
            procesarEvento(msg);
        }
        else if (g_active == true)
        {
			
			ActualizarJuego();

			Blt();
        }
    }

    FinalizarSDL();
    return 0;
}
