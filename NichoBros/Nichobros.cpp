// Gracias!
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <dirent.h> 
#include <sys/stat.h>
#include "RutinasSDL/rutinasSDL.h"
#include "Nichobros.h"

const int TOTAL_SKINS = 4;

LPDIRECTDRAWSURFACE fuuu = NULL, parchment = NULL;

enum eSkin {SKIN1, SKIN2, SKIN3, SKIN4};

eSkin skin = SKIN1;

const char nombreSkin[TOTAL_SKINS][32] = {"Colegial","Sweater","Ario", "Negro"};

int volumenSonido = 100;
int volumenMusica = 100;

bool haySonido = true;
bool hayMusica = true;

#define CONCEPT_ART_SIZE 7

LPDIRECTDRAWSURFACE lpConceptArt[CONCEPT_ART_SIZE];

Mix_Music *musicaMenu = NULL;
Mix_Music *musicaJuego = NULL;

// La flechita pedorra de las grabaciones!
LPDIRECTDRAWSURFACE lpDDSFlecha = NULL;
// Buffers con todos los sonidos...
LPDIRECTSOUNDBUFFER lpDSBuffers[TAMANO_SONIDO];
bool bEjecutandose = false;
bool FinNivel = false;
bool GameOver = false;
bool MostrarCreditos = false;
bool MostrarAyuda    = false;
bool MostrarAcercaDe = false;

bool Cargado = false;

enum eBoton {botNulo = 0,botIzquierda = 1, botIntro = 2, botDerecha = 3};
int  VidasIniciales = 3;

#define MaxLongOpcion 35
#define MaxVidas 9
#define MinVidas 0
#define VelocidadColorFX 0.1

typedef bool (*FuncionDeMenu)(eBoton BotonPresionado);
typedef FuncionDeMenu *TablaFunciones;

struct stMenu 
{
	int  NumeroOpciones;
	char (*NombreOpciones)[MaxLongOpcion];
	TablaFunciones Funciones;
};

stMenu MenuActual;
char NOMainMenu   [6][MaxLongOpcion] = {"Jugar","Opciones","Créditos","Ayuda","Cargar","Salir"};
char NOJugarMenu  [3][MaxLongOpcion] = {"Modo historia","Nivel creado por el usuario","Volver"};
char NOOpcionMenu [6][MaxLongOpcion] = {"Vidas 3","Dificultad Jueguero Decente","Sonido","Galería de Arte", "Skin: Colegial" ,"Volver"};
char NOOpcionSonidoMenu [5][MaxLongOpcion] =  {"Sonido On","Volumen Sonido 100%","Música On","Volumen Música 100%","Volver"};

TablaFunciones TFJugarMenu;
TablaFunciones TFMainMenu;
TablaFunciones TFOpcionMenu;
TablaFunciones TFOpcionSonidoMenu;

int  Seleccion;

char NomDif[4][25] = {"Queso Remachado",
			    	  "Newby",
		    		  "Jueguero Decente",
		    		  "Cyber Adicto"};

HFONT hFuente;
HFONT hTitleFont;
HFONT hOptionFont;
HFONT hCreditsFont;
HFONT hAyudaFont;



LPDIRECTDRAWSURFACE CrearSuperficieDesdeBitmap(LPCSTR pathBMP,bool transparente = true)
{
    if (transparente)
        return SDL::loadImage(pathBMP,true,0,0,0);
    else
        return SDL::loadImage(pathBMP);
}



// MACRO MILAGROSA!
// No la voy a usar :D
// Sin embargo la dejo por si algun dia me pregunto como era, se que aca esta :D

#define ASM(x) {asm(".intel_syntax noprefix"); asm("\n\t"); asm(x); asm(".att_syntax");}

void RojoAPedido(LPDIRECTDRAWSURFACE Surf, long int *Lista, long int Velocidad, bool Avanzar)
{
	unsigned short int *pointer;
	unsigned short int Rojo;
	long Pit;

	SDL_LockSurface(Surf);

    pointer = (unsigned short int *) Surf->pixels;
	Pit     = Surf->pitch / 2;
	Rojo    = (unsigned short int)Surf->format->Rmask;
    
    
    // ebx = pointer
    long *pLista = Lista;
    long  vel = Velocidad;
	
    if (vel > 0)
    for (int ecx=800;ecx>0;ecx--)
    {
        int edi = *pLista;
        if (Avanzar) *pLista += vel;
        int eax = -vel;
        if (edi > eax)
        {
            if (edi < 0)
            {
                vel += edi;
                edi = 0;
            }
            eax = 600;
            if (edi < eax)
            {
                eax -= vel;
                if (edi > eax)
                    vel = 600 - edi;
                edi *= Pit;
                eax = vel;
                do
                {
                    pointer[edi] = Rojo;
                    edi += Pit;
                    eax--;
                } while (eax);
            }
        }
        
        // fin loop
        vel = Velocidad;

        pointer++;
        pLista++;
    }

	SDL_UnlockSurface(Surf);

}

void NegroIntercalado(LPDIRECTDRAWSURFACE Surf, int Ancho, int N, int Ini)
{
	unsigned short int *pointer;
	SDL_LockSurface(Surf);
    pointer = (unsigned short int *) Surf->pixels;
    pointer += N * (Surf->pitch/2) + Ini; 
    Ini = (int)pointer;
    
    for(Ancho /= 2;Ancho;Ancho--,pointer+=2) *pointer=0;

    SDL_UnlockSurface(Surf);
}

void WhitePaint(long X , long Y , LPDIRECTDRAWSURFACE Surf,int Ancho, int Alto)
{
	unsigned short int *pointer1, *pointer2;
	long AuxLong;
	long Pit1,Pit2;

	RECT J,J2; 
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

    SDL_LockSurface(Surf);
    
    pointer1 = (unsigned short int *) Surf->pixels + (J.top * (Surf->pitch / 2)) + J.left;
	Pit1     = Surf->pitch;

	SetRect(&J2,X + J.left,Y + J.top,X + J.right,Y + J.bottom);
    
    SDL_LockSurface(SDL::screen());
    
    pointer2 = (unsigned short int *) SDL::screen()->pixels + (J2.top * (SDL::screen()->pitch / 2)) + J2.left;;
	Pit2     = SDL::screen()->pitch;
	
    // edx = pointer1
    // ebx = pointer2
    AuxLong = J.bottom - J.top;

    for(int i=0;i<AuxLong;i++)
    {
        int edi,ecx;
        ecx = J.right - J.left;
        edi = 0;
        for(int j=0;j<ecx;j++)
        {
            if (pointer1[edi] != 0)
                pointer2[edi] = 0xFFFF;
            edi++;
        }
        pointer2 += Pit2 / 2;
        pointer1 += Pit1 / 2;
    }
    
    SDL_UnlockSurface(SDL::screen());
    SDL_UnlockSurface(Surf);
}

void QuemarLinea(LPDIRECTDRAWSURFACE Surf,int Ancho,int N)
{
	static unsigned short int ColorGris   = 0;
	static unsigned short int ColorBlanco = 0;
	unsigned short int *pointer;

    SDL_LockSurface(Surf);
	
	pointer = (unsigned short int *) Surf->pixels;
    pointer += N * (Surf->pitch / 2);

	if (!ColorGris)
	{
		unsigned short int rm,gm,bm;
		
		rm = (unsigned short int)Surf->format->Rmask;
		gm = (unsigned short int)Surf->format->Gmask;
		bm = (unsigned short int)Surf->format->Bmask;
		
		ColorBlanco = (unsigned short int)(rm | gm | bm);

		rm = (unsigned short int)(rm & (rm >> 3)); // Interesante: revisar en papel. Estas
		gm = (unsigned short int)(gm & (gm >> 3)); // operaciones producen un gris
		bm = (unsigned short int)(bm & (bm >> 3)); // al 12.5%

		ColorGris = (unsigned short int)(rm | gm | bm);
	}

    for(int ecx = Ancho;ecx>0;ecx--)
    {
        if (*pointer != ColorBlanco && *pointer != 0)
            *pointer = ColorGris;
        pointer++;
    }

	SDL_UnlockSurface(Surf);
}

void WhiteLine (LPDIRECTDRAWSURFACE S1,LPDIRECTDRAWSURFACE S2,int Ancho,int N)
{
	static unsigned short int ColorBlanco = 0;
	unsigned short int *p1,*p2;
	RECT R;

    SetRect(&R,0,N,Ancho,N + 1);
    
    SDL_LockSurface(S1);
    
	p1 = (unsigned short int *) S1->pixels + N * (S1->pitch / 2);

	SetRect(&R,0,N,Ancho,N + 1);
	SDL_LockSurface(S2);

	p2 = (unsigned short int *) S2->pixels + N * (S2->pitch / 2);

	if (!ColorBlanco)
	{
		unsigned short int rm,gm,bm;
		
		rm = (unsigned short int)S1->format->Rmask;
		gm = (unsigned short int)S1->format->Gmask;
		bm = (unsigned short int)S1->format->Bmask;
		
		ColorBlanco = (unsigned short int)(rm | gm | bm);
	}

    // ebx = p1
    // edx = p2
    // ecx = Ancho
    // ax = ColorBlanco
    for(int i=0;i<Ancho;i++)
    {
        if (*p1)
            *p2 = ColorBlanco;
        else
            *p2 = 0;
        p1++;
        p2++;
    }

	SDL_UnlockSurface(S1);
	SDL_UnlockSurface(S2);
}

void WhiteExtremes(LPDIRECTDRAWSURFACE S1,LPDIRECTDRAWSURFACE S2,int Ancho,int N)
{
	static unsigned short int ColorBlanco = 0;
	unsigned short int *p1,*p2;
	RECT R;

	SetRect(&R,0,N,Ancho,N + 1);
	SDL_LockSurface(S1);
	
	p1 = (unsigned short int *) S1->pixels + N * (S1->pitch / 2);

	SetRect(&R,0,N,Ancho,N + 1);
	SDL_LockSurface(S2);

	p2 = (unsigned short int *) S2->pixels + N * (S2->pitch / 2);

	if (!ColorBlanco)
	{
		unsigned short int rm,gm,bm;
		
		rm = (unsigned short int)S1->format->Rmask;
		gm = (unsigned short int)S1->format->Gmask;
		bm = (unsigned short int)S1->format->Bmask;
		
		ColorBlanco = (unsigned short int)(rm | gm | bm);
	}

    // EBX: Nicholas, p1
    // EDX: Teleimage, p2

    int esi = 0,edi = Ancho-1; // esi = izq, edi = der
    
    unsigned char al = 0, ah = 0;

    for(int i=0;i < (Ancho+1) / 2;i++)
    {
        if ((al != 1) && p1[esi] != 0)
        {
            p2[esi] = ColorBlanco;
            al = 1;
        }
        else
            p2[esi] = 0;
            
        if ((ah != 1) && p1[edi] != 0)
        {
            p2[edi] = ColorBlanco;
            ah = 1;
        }
        else
            p2[edi] = 0;
		esi++;
        edi--;
    }


	SDL_UnlockSurface(S1);
	SDL_UnlockSurface(S2);
}

void BufferPaint(long X , long Y , LPDIRECTDRAWSURFACE Surf, int Ancho, int Alto, int X0,int Y0)
{	
	// Esta subrutina nos hace la vida mas facil, ya que se
    // encarga de operaciones de bajo nivel automaticamente,
    // pero sobre todo se encarga de la tediosa tarea del
    // CLIPPING.

	// En DirectDraw podemos pedir objetos clippers que hagan esto, pero yo programe el mío y
	// ya está, así que dale que va.
	
    // ...
    // Y encima ahora en la SDL creo que no hay que pedir nada y ya el tipo clipea solo pero bueno...
    // Queda. :D
    
    RECT J; 
	long Resta;
	
	// Clipping izquierdo
	if (X < 0)
	{
		J.left = X0 + abs(X);
		X = 0;
	}
	else
	    J.left = X0;
	// Clipping superior
	if (Y < 0)
	{
		J.top = Y0 + abs(Y);
	    Y = 0;
	}
	else
	    J.top = Y0;
	// Clipping Derecho
	Resta = X + Ancho - 800;
	if (Resta > 0)
		J.right = X0 + Ancho - Resta;
	else
		J.right = X0 + Ancho;
	// Clipping Inferior
	Resta = Y + Alto - 600;
	if (Resta > 0)
		J.bottom = Y0 + Alto - Resta;
	else
		J.bottom = Y0 + Alto;
	// Finalmente, dibujamos en el backbuffer
	BltFast(SDL::screen(),X, Y, Surf, &J);
}

bool SpriteColCheck(LPDIRECTDRAWSURFACE S1,LPDIRECTDRAWSURFACE S2,long X1, long Y1, long X2, long Y2, RECT &R,int start1, int start2)
// Guau! Este assembler verifica que no solo los rectangulos contenedores, sino tambien los
// gráficos en pantalla con sus irregularidades y todo estén colisionanando.
// POR AHORA ANDA RARO...
{
    //return true; // Verifica ... esta !!!!
    
	unsigned short int *pointer1, *pointer2;
	long Pit1,Pit2;
	RECT AuxR;
		
	SetRect(&AuxR,R.left - X1,R.top - Y1,R.right - X1, R.bottom - Y1);
	SDL_LockSurface(S1);
    pointer1 = (unsigned short int *) S1->pixels + ((start1 + AuxR.top) * (S1->pitch / 2)) + AuxR.left;
	Pit1     = S1->pitch;

	SetRect(&AuxR,R.left - X2,R.top - Y2,R.right - X2, R.bottom - Y2);
	SDL_LockSurface(S2);
    pointer2 = (unsigned short int *) S2->pixels + ((start2 + AuxR.top) * (S2->pitch / 2)) + AuxR.left;
	Pit2     = S2->pitch;

	int Ancho, Alto;
	Ancho = R.right  - R.left;
	Alto  = R.bottom - R.top;

	bool Valor = false;

    // edx = pointer1
    // ebx = pointer2

    //mov  eax, Alto

    for(int i=0;i<Alto;i++)
    {

        for(int j=0;j<Ancho;j++)
        {
            if (pointer1[j] && pointer2[j])
            {
                Valor = true;
                goto SalidaFinal;
            }
        }
        
		// Aca termina el pequeño loop

		pointer2 += Pit2 / 2;
		pointer1 += Pit1 / 2;
		
    }
    
SalidaFinal:

		// Aca termina el loop grande.

	SDL_UnlockSurface(S2);
	SDL_UnlockSurface(S1);

	return Valor;
}

RECT ColR;

bool ColisionGeneral(RECT &R1,RECT &R2,LPDIRECTDRAWSURFACE S1,LPDIRECTDRAWSURFACE S2,int start1, int start2)
{
	// Esto es más elegante, y brinda más info que la versión anterior, que compara
	// la distancia entre centros con (L1 + L2) / 2
	ColR.left   = std::max(R1.left  ,R2.left);
	ColR.right  = std::min(R1.right ,R2.right);
	ColR.top    = std::max(R1.top   ,R2.top);
	ColR.bottom = std::min(R1.bottom,R2.bottom);
	return (ColR.left < ColR.right && ColR.top < ColR.bottom) &&
			SpriteColCheck(S1,S2,R1.left,R1.top,R2.left,R2.top,ColR,start1,start2);
}

bool ColNicholasCasilla(int X, int Y)
{
	double NX;
	long   CX;
	int	   CY;
	NX = Nicho->ImagenNicholasX + 30; // CentroX
	CasillaCoordenada(CX,CY,X,Y);
	CX += 15;
	return fabs(NX - CX) <= DistanciaColisionNicholasCasilla;
}

eTiposCol ColisionaNicholas(RECT &OR, LPDIRECTDRAWSURFACE lpSprite,int surStartH)
{
    RECT NR;
	SetRect(&NR,(long)Nicho->ImagenNicholasX,
		        (long)Nicho->ImagenNicholasY,
				(long)Nicho->ImagenNicholasX + 60,
				(long)Nicho->ImagenNicholasY + 90);

	if (!EstoyVivo || !ColisionGeneral(OR,NR,lpSprite,imglstNicholas[Nicho->Estado],surStartH))
		return ColNula;

	double CCX, CCY, OCX, OCY;
	bool U, D, L,R;

	CCY = (ColR.top  + ColR.bottom); // / 2; Nos ahorramos dividir por dos, pues se usan solo
	CCX = (ColR.left + ColR.right ); // / 2; para comparar, y a/2 > b/2 <=> a > b
	OCY = (OR.top    + OR.bottom  ); // / 2;
	OCX = (OR.left   + OR.right   ); // / 2;

	U = CCY < OCY;
	D = CCY > OCY;
	L = CCX < OCX;
	R = CCX > OCX;
	if (U)
		if (L)
			return ColSuperiorIz;
		else if (R)
			return ColSuperiorDe;
		else
			return ColSuperior;
	else if (D)
		if (L)
			return ColInferiorIz;
		else if (R)
			return ColInferiorDe;
		else
			return ColInferior;
	else if (L)
		return ColIzquierda;
	else if (R)
		return ColDerecha;
	else
		return ColTotalInterna;
}

void RedibujarTT()
{
	long	  RX;
	int		  RY;
    eCasillas ElTerreno;
    int		  X, Y;
	for (RX = 0; RX < 830; RX += 30)
        for (RY = 0; RY <= 570; RY += 30)
		{
            CoordenadaCasilla(Pos + RX, RY, X, Y);
			if (X < Ancho)
			{
                ElTerreno = Nivel->Terreno(X, Y);
				if (ElTerreno != caVacia && 
				   ((Nivel->ExtraInfo(X,Y) & inTapador) != 0 ||
				    Nivel->Eventos(X,Y) == evZonaDuele))
					BufferPaint(RX - Pos % 30, RY, imglstTerreno[ElTerreno - 1],30,30);
			}
		}
}

void Redibujar()
{
    long	  RX;
	int		  RY;
    eCasillas ElTerreno;
    int		  X, Y;
#ifdef FONDOMOVIL
	BufferPaint(-(long)(Pos * VelocidadMovRelativoFondo) % 800,0,lpDDSFondo,800,600);
	BufferPaint(800 - (long)(Pos * VelocidadMovRelativoFondo) % 800,0,lpDDSFondo,800,600);
#else  // FONDOMOVIL
	BufferPaint(0,0,lpDDSFondo,800,600);
#endif // FONDOMOVIL
	for (RX = 0; RX < 830; RX += 30)
        for (RY = 0; RY <= 570; RY += 30)
		{
            CoordenadaCasilla(Pos + RX, RY, X, Y);
			if (X < Ancho)
			{
				if ((Nivel->ExtraInfo(X,Y) & inTapador) == 0 &&
					Nivel->Eventos(X,Y) != evZonaDuele)
				{
				    ElTerreno = Nivel->Terreno(X, Y);
					if (ElTerreno != caVacia)
						BufferPaint(RX - Pos % 30, RY, imglstTerreno[ElTerreno - 1],30,30);
	                switch (Nivel->Eventos(X, Y))
					{
						case evPremioSpeed:
							BufferPaint(RX - Pos % 30, RY, imglstPremios[prSpeed],30,30);
							break;
						case evPremioMoneda:
							BufferPaint(RX - Pos % 30, RY, imglstPremios[prMoneda],30,30);
							break;
						case evPremioBolaFuego:
							BufferPaint(RX - Pos % 30 + 7, RY + 7, imglstPremios[prBolaFuego],15,15);
							break;
						case evPremioEscudo:
							BufferPaint(RX - Pos % 30, RY, imglstPremios   [prEscudo ],30,30);
							BufferPaint(RX - Pos % 30, RY, imglstParticulas[partVerde],30,30);
							break;
						case evPremioAureola:
							BufferPaint(RX - Pos % 30, RY, imglstPremios[prAureola],30,30);
							break;
						case evPremioLaser:
							BufferPaint(RX - Pos % 30, RY, imglstPremios[prLaser],30,30);
							break;
						case evPremioLiquidoVerde:
							BufferPaint(RX - Pos % 30, RY, imglstPremios[prLiquidoVerde],30,30);
							break;
						// Premios por puntos, sin funcionalida.
						case evPremioCereza:
							BufferPaint(RX - Pos % 30, RY, imglstPremios[prCereza],29,35);
							break;
						case evPremioFrutilla:
							BufferPaint(RX - Pos % 30, RY, imglstPremios[prFrutilla],35,35);
							break;
						case evPremioJuguito:
							BufferPaint(RX - Pos % 30, RY, imglstPremios[prJuguito],26,35);
							break;
						case evPremioBanana:
							BufferPaint(RX - Pos % 30, RY, imglstPremios[prCereza],31,33);
							break;
						case evPremioPizzaVeggy:
							BufferPaint(RX - Pos % 30, RY, imglstPremios[prPizzaVeggy],33,35);
							break;
						case evPremioLicuado:
							BufferPaint(RX - Pos % 30, RY, imglstPremios[prLicuado],32,35);
							break;
						case evPremioPizza:
							BufferPaint(RX - Pos % 30, RY, imglstPremios[prPizza],33,33);
							break;
						case evPremioPizzaC:
							BufferPaint(RX - Pos % 30, RY, imglstPremios[prPizzaC],31,35);
							break;
						case evPremioRadio:
							BufferPaint(RX - Pos % 30, RY, imglstPremios[prRadio],33,33);
							break;
						// Fin premios por puntos.
						case evCandado:
							BufferPaint(RX - Pos % 30, RY, imglstPremios[prCandado],30,30);
							break;
						case evBotRojo:
							if (Nivel->Eventos(X - 1,Y) == evPisoRigido)
								BufferPaint(RX - Pos % 30, RY, imglstAparejos[apaBotRRev],9,10);
							else
								BufferPaint(RX - Pos % 30 + 20, RY, imglstAparejos[apaBotR],9,10);
							break;
						case evBotCyan:
							if (Nivel->Eventos(X - 1,Y) == evPisoRigido)
								BufferPaint(RX - Pos % 30, RY, imglstAparejos[apaBotCRev],9,10);
							else
								BufferPaint(RX - Pos % 30 + 20, RY, imglstAparejos[apaBotC],9,10);
							break;
						case evBotRojoIn:
							if (Nivel->Eventos(X - 1,Y) == evPisoRigido)
								BufferPaint(RX - Pos % 30, RY, imglstAparejos[apaBotRInRev],9,10);
							else
								BufferPaint(RX - Pos % 30 + 20, RY, imglstAparejos[apaBotRIn],9,10);
							break;
						case evBotCyanIn:
							if (Nivel->Eventos(X - 1,Y) == evPisoRigido)
								BufferPaint(RX - Pos % 30, RY, imglstAparejos[apaBotCInRev],9,10);
							else
								BufferPaint(RX - Pos % 30 + 20, RY, imglstAparejos[apaBotCIn],9,10);
							break;
                        default:
                            break;
					}
				}
			}
		}
}

void EscribirDatos()
{

	char Cad [MAX_LONG_CARTEL];
	char Cad2[MAX_LONG_CARTEL];
	sprintf(Cad2,"%ld",Puntaje);
	strcpy(Cad,"Puntaje:");
	strcat(Cad,Cad2);
	#ifndef DEPURANDO
		EscribirEnSuperficie(SDL::screen(), 0, 0,Cad,RGB(255,0,0),hFuente);
	#else  // DEPURANDO
		EscribirEnSuperficie(SDL::screen(),0,550,Cad,RGB(255,0,0),hFuente);
	#endif // DEPURANDO
	sprintf(Cad2,"%d",Vidas);
	strcpy(Cad,"Vidas:");
	strcat(Cad,Cad2);
	#ifndef DEPURANDO
		EscribirEnSuperficie(SDL::screen(), 700, 0,Cad,RGB(255,0,0),hFuente);
	#else  // DEPURANDO
		EscribirEnSuperficie(SDL::screen(),700,550,Cad,RGB(255,0,0),hFuente);
	#endif // DEPURANDO
	sprintf(Cad2,"%d",Monedas);
	strcpy(Cad,"Monedas:");
	strcat(Cad,Cad2);
	#ifndef DEPURANDO
		EscribirEnSuperficie(SDL::screen(), 300, 0,Cad,RGB(255,0,0),hFuente);
	#else  // DEPURANDO
		EscribirEnSuperficie(SDL::screen(),300,550,Cad,RGB(255,0,0),hFuente);
	#endif // DEPURANDO
	sprintf(Cad,"%d",CantidadBF);
	#ifndef DEPURANDO
		BufferPaint(0,30,imglstPremios[prBolaFuego],15,15);
		EscribirEnSuperficie(SDL::screen(), 25, 25,Cad,RGB(255,0,0),hFuente);
	#else
		BufferPaint(5,575,imglstPremios[prBolaFuego],15,15);
		EscribirEnSuperficie(SDL::screen(), 25, 570,Cad,RGB(255,0,0),hFuente);
	#endif
	sprintf(Cad,"%d",CantidadLaser);
	#ifndef DEPURANDO
		BufferPaint(0,60,imglstEnergia[enerRayo],24,8);
		EscribirEnSuperficie(SDL::screen(), 25, 55,Cad,RGB(255,0,0),hFuente);
	#endif
	sprintf(Cad,"%d",TiempoJuego / 1000);
	strcpy(Cad2,"Tiempo:");
	strcat(Cad2,Cad);
	RECT AuxR;
	#ifndef DEPURANDO
		EscribirEnSuperficie(SDL::screen(), 20, 570,Cad2,RGB(255,0,0),hFuente);
	#else
		EscribirEnSuperficie(SDL::screen(), 285, 0,Cad2,RGB(255,0,0),hFuente);
	#endif
	AuxR.left   = 0;
	AuxR.top    = 0;
	AuxR.bottom = 20;
	if (TiempoLV)
	{
		
		AuxR.right  = (int)(100 * TiempoLV / DuracionLiquidoVerde);
		#ifndef DEPURANDO
			BltFast(SDL::screen(),675,570,lpDDSBarraVerde,&AuxR);
		#else
			BltFast(SDL::screen(),675,5,lpDDSBarraVerde,&AuxR);
		#endif
	}
	else
	{
		AuxR.right  = (int)Cansancio;
		#ifndef DEPURANDO
			BltFast(SDL::screen(),675,570,lpDDSBarraCansancio,&AuxR);
		#else
			BltFast(SDL::screen(),675,5,lpDDSBarraCansancio,&AuxR);
		#endif
	}
	#ifndef DEPURANDO
		EscribirEnSuperficie(SDL::screen(),550,570,"Resistencia:",RGB(255,0,0),hFuente);
	#else
		EscribirEnSuperficie(SDL::screen(),550,2,"Resistencia:",RGB(255,0,0),hFuente);
	#endif
}

void ChequearCasillas()
{
	long RX;
	int  RY;
	int  X, Y;
	ObjetoBase *ObjetoAuxiliar;
	Tortuga	   *TortugaAuxiliar;
	for (RX = 0; RX < 830; RX += 30)
        for (RY = 0; RY <= 570; RY += 30)
		{
            CoordenadaCasilla(Pos + RX, RY, X, Y);
			if (X < Ancho)
			{
				if (Nivel->Eventos(X,Y) == evGenerador && Tortugas->Tamano() == 0)
				{
					PlayWav(sndGenerador);
					ObjetoAuxiliar  = new Tortuga();
					TortugaAuxiliar = (Tortuga *)ObjetoAuxiliar;
                    TortugaAuxiliar->Inicializar(X, Y);
					Tortugas->Agregar(ObjetoAuxiliar);
				}
                else if (Nivel->Eventos(X,Y) == evExplotando)
                    Nivel->SetEventos(X,Y,evNulo);
			}
		}
}

void ChequearColeccion(Coleccion *Chequeando)
{
	long QueItem, Cantidad, Total;
	Cantidad = 0;
    QueItem  = 0;
    Total	 = Chequeando->Tamano();
	Chequeando->SelPrimero();
    while (Cantidad < Total)
	{
        MuerteReciente = false;
        Chequeando->Avanzar()->Check();
        Cantidad++;
        if (MuerteReciente)
			delete Chequeando->Eliminar(QueItem);
		else
			QueItem++;
	}
}

void ChequearTodo()
{
	ChequearCasillas();

	if (Nicho != NULL)
		Nicho->Check();
    ChequearColeccion(Rayos);

	ChequearColeccion(Juanchis);
    ChequearColeccion(Tortugas);
    ChequearColeccion(Carteles);
    ChequearColeccion(Explosiones);
	ChequearColeccion(ColecMonedas);
	ChequearColeccion(BolasFuego);
	ChequearColeccion(Rebotadores);
	ChequearColeccion(BolasPico);
	ChequearColeccion(PuentesCaen);
	ChequearColeccion(VerdesLocos);
	ChequearColeccion(ColecRobots);
	ChequearColeccion(Plataformas);
	ChequearColeccion(PinchesPalo);
	ChequearColeccion(Triturines);
	ChequearColeccion(Caecuchos);
	ChequearColeccion(Teleporters);
	ChequearColeccion(Puertas);
	ChequearColeccion(Sauriolos);
}

void MostrarColeccion(Coleccion *Mostrando)
{
	long Cantidad, Total;
	Cantidad = 0;
    Total    = Mostrando->Tamano();
	Mostrando->SelPrimero();
    while (Cantidad < Total)
	{
        Mostrando->Avanzar()->Mostrar();
        Cantidad++;
	}
}

void MostrarTodo()
{
	MostrarColeccion(Juanchis);
	MostrarColeccion(Tortugas);
	MostrarColeccion(Carteles);
	MostrarColeccion(Explosiones);
	MostrarColeccion(ColecMonedas);
	MostrarColeccion(BolasFuego);
	MostrarColeccion(Rebotadores);
	MostrarColeccion(BolasPico);
	MostrarColeccion(PuentesCaen);
	MostrarColeccion(VerdesLocos);
	MostrarColeccion(ColecRobots);
	MostrarColeccion(Rayos);
	MostrarColeccion(Plataformas);
	MostrarColeccion(PinchesPalo);
	MostrarColeccion(Triturines);
	MostrarColeccion(Caecuchos);
	MostrarColeccion(Teleporters);
	MostrarColeccion(Puertas);
	MostrarColeccion(Sauriolos);
}

void EliminarColeccion(Coleccion *Eliminando)
{
	while (Eliminando->Tamano() != 0)
	{
		delete Eliminando->Eliminar(0);
	}
}

void EliminarTodo()
{
	EliminarColeccion(Juanchis);
	EliminarColeccion(Tortugas);
    EliminarColeccion(Carteles);
    EliminarColeccion(Explosiones);
    EliminarColeccion(ColecMonedas);
	EliminarColeccion(BolasFuego);
	EliminarColeccion(Rebotadores);
	EliminarColeccion(BolasPico);
	EliminarColeccion(PuentesCaen);
	EliminarColeccion(VerdesLocos);
	EliminarColeccion(ColecRobots);
	EliminarColeccion(Rayos);
	EliminarColeccion(Plataformas);
	EliminarColeccion(PinchesPalo);
	EliminarColeccion(Triturines);
	EliminarColeccion(Caecuchos);
	EliminarColeccion(Teleporters);
	EliminarColeccion(Puertas);
	EliminarColeccion(Sauriolos);
}

void DescargarColecciones()
{
	DescargarColeccion(Juanchis);
	DescargarColeccion(Tortugas);
	DescargarColeccion(Carteles);
	DescargarColeccion(Explosiones);
	DescargarColeccion(ColecMonedas);
	DescargarColeccion(BolasFuego);
	DescargarColeccion(Rebotadores);
	DescargarColeccion(BolasPico);
	DescargarColeccion(PuentesCaen);
	DescargarColeccion(VerdesLocos);
	DescargarColeccion(ColecRobots);
	DescargarColeccion(Rayos);
	DescargarColeccion(Plataformas);
	DescargarColeccion(PinchesPalo);
	DescargarColeccion(Triturines);
	DescargarColeccion(Caecuchos);
	DescargarColeccion(Teleporters);
	DescargarColeccion(Puertas);
	DescargarColeccion(Sauriolos);
}

void ClearAllKeys()
{
	int Aux;
	for (Aux = 0;Aux < 256; Aux++)
		Teclado[Aux] = false;
}

void LimpiarPantalla()
{

#define VelocidadLimpiado 0.3

	double LineaPar;
	int    kk;
	int	   LLP;
	RECT AuxR;

	//AuxR.left   = AuxR.top = 0;
	//AuxR.right  = 800;
	//AuxR.bottom = 600;
	//lpDDSSec->BltFast(0,0,lpDDSPri,&AuxR, DDBLTFAST_WAIT);
	

	double Aumento;

	LastFrameTime = SDL::getTicks();

	for (LLP = 0, LineaPar = 0;LineaPar < 600;LineaPar += Aumento)
	{
		Delta = TiempoDesde(LastFrameTime);
		LastFrameTime = SDL::getTicks();
		if (Delta > MaxDelta)
			Delta = MaxDelta;
		else if (Delta == 0)
			Delta = 1;
		Aumento = Delta * VelocidadLimpiado;
		if (LineaPar + Aumento > 600)
			Aumento = 600 - LineaPar;
		for (kk = 0;kk < (int)LineaPar - LLP;kk++)
		{
			NegroIntercalado(SDL::screen(),800,LLP + kk,0);
			NegroIntercalado(SDL::screen(),800,599 - LLP - kk,1);
		}
		//lpDDSPri->Flip(NULL,DDFLIP_WAIT);
        SDL::flip();
		for (kk = 0;kk < (int)LineaPar - LLP;kk++)
		{
			NegroIntercalado(SDL::screen(),800,LLP + kk,0);
			NegroIntercalado(SDL::screen(),800,599 - LLP - kk,1);
		}
		LLP = (int)LineaPar;
	}

	AuxR.left   = 0;
	AuxR.top	= 0;
	AuxR.right  = 800;
	AuxR.bottom = 600;

	DrawRectangle(SDL::screen(),&AuxR,i2col(0));
	//lpDDSPri->Flip(NULL,DDFLIP_WAIT);
    SDL::flip();

	AuxR.left   = 0;
	AuxR.top	= 0;
	AuxR.right  = 800;
	AuxR.bottom = 600;

	DrawRectangle(SDL::screen(),&AuxR,i2col(0));
	//lpDDSPri->Flip(NULL,DDFLIP_WAIT);
    SDL::flip();
	ClearAllKeys();
}

void MenuBlt()
{
	RECT AuxR;
	// Dibujamos el fondo del menu.

	AuxR.left   = 0;
	AuxR.top	= 0;
	AuxR.right  = 800;
	AuxR.bottom = 600;
	BltFast(SDL::screen(),0,0,lpDDSFondo,&AuxR);

	// Manejamos delta...

	Delta = TiempoDesde(LastFrameTime);
	LastFrameTime = SDL::getTicks();
	if (Delta > MaxDelta)
		Delta = MaxDelta;
	else if (Delta == 0)
		Delta = 1;

	// Corremos el valor de color.
	static double ColorVal = 0, Incre = 1;
	ColorVal += Incre * Delta;
	if (ColorVal >= 255)
	{
		ColorVal = 255;
		Incre    = -VelocidadColorFX;
	}
	if (ColorVal <= 0)
	{
		ColorVal = 0;
		Incre    = VelocidadColorFX;
	}

	// Y las opciones.
	int Aux;
	for (Aux = 0;Aux < MenuActual.NumeroOpciones;Aux++)
		if (Aux == Seleccion)
			EscribirEnSuperficie(SDL::screen(),130,180 + 70 * Aux,*(MenuActual.NombreOpciones + Aux),RGB(ColorVal,0,0),hOptionFont,RGB(255 - ColorVal,255 - ColorVal,255 - ColorVal));
		else
			EscribirEnSuperficie(SDL::screen(),130,180 + 70 * Aux,*(MenuActual.NombreOpciones + Aux),RGB(55,55,255),hOptionFont);

	//lpDDSPri->Flip(NULL,DDFLIP_WAIT);
    SDL::flip();
}

bool AnyKey()
{
	int Aux;
	for (Aux = 0;Aux < 256; Aux++)
		if (Teclado[Aux])
			return true;
	return false;
}

void FelicitarBlt()
{
	RECT AuxR;
	AuxR.left   = 0;
	AuxR.right  = 800;
	AuxR.top	= 0;
	AuxR.bottom = 600;
	DrawRectangle(SDL::screen(),&AuxR,RGB(50,50,215));

	EscribirEnSuperficie(SDL::screen(),50,50,"¡Felicitaciones!",RGB(240,25,25),hTitleFont,RGB(0,70,0));

	EscribirEnSuperficie(SDL::screen(),100,175,"¡¡¡¡ Has completado NichoBros !!!!",RGB(255,255,255),hFuente);
	EscribirEnSuperficie(SDL::screen(),100,200,"Ahora salí a laburar, o a estudiar, o andá a dormir...",RGB(255,255,255),hFuente);
	EscribirEnSuperficie(SDL::screen(),100,225,"Pero dejá de jugar y hacé algo productivo.",RGB(255,255,255),hFuente);
	EscribirEnSuperficie(SDL::screen(),100,250,"¡Saludos y mucha suerte!",RGB(255,255,255),hFuente);

	EscribirEnSuperficie(SDL::screen(),150,450,"<Pulsa cualquier tecla para continuar>",RGB(10,255,10),hFuente);

	//lpDDSPri->Flip(NULL,DDFLIP_WAIT);
    SDL::flip();
}

void procesarEvento (const SDL_Event &ev)
{
    if (ev.type == SDL_KEYDOWN)
    {
        Teclado[ev.key.keysym.sym] = true; 
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
    }
    else if (ev.type == SDL_KEYUP)
        Teclado[ev.key.keysym.sym] = false; 
    else if (ev.type == SDL_QUIT)
    {
        g_active = false;
        bEjecutandose = false;
    }
} 


void Felicitar()
{
	SDL_Event msg;
	ClearAllKeys();
	PlayWav(sndVersionFull);
	//Bucle principal
	bEjecutandose = true;
    while (bEjecutandose)
	{
		while (SDL::pollEvent(msg))
	    {
		    procesarEvento(msg);
		}
	    if (g_active == true)
		{
			bEjecutandose = !AnyKey();
				
			FelicitarBlt();
		}
	}

	LimpiarPantalla();
}

bool MainMenu0(eBoton BotonPresionado) // Jugar
{
	if (BotonPresionado == botIntro)
	{
		PlayWav(sndBoton);
		MenuActual.NombreOpciones = NOJugarMenu;
		MenuActual.NumeroOpciones = 3;
		MenuActual.Funciones      = TFJugarMenu;
	}
	return true;
}

bool MainMenu1(eBoton BotonPresionado) // Opciones
{
	if (BotonPresionado == botIntro)
	{
		PlayWav(sndBoton);
		MenuActual.NombreOpciones = NOOpcionMenu;
		MenuActual.NumeroOpciones = 6;
		MenuActual.Funciones      = TFOpcionMenu;
	}
	return true;
}

bool MainMenu2(eBoton BotonPresionado) // Creditos
{
	if (BotonPresionado == botIntro)
	{
		PlayWav(sndBoton);
		MostrarCreditos = true;
		bEjecutandose = false;
		return true;
	}
	else
		return true;
}

bool MainMenu3(eBoton BotonPresionado) // Ayuda
{
	if (BotonPresionado == botIntro)
	{
		PlayWav(sndBoton);
		MostrarAyuda    = true;
		bEjecutandose   = false;
		return true;
	}
	else
		return true;
}

bool Cargar();

bool MainMenu4(eBoton BotonPresionado) // Acerca de...
{
	if (BotonPresionado == botIntro)
	{
		PlayWav(sndBoton);
		if (Cargar())
		{
			bEjecutandose   = false;
			Cargado = true;
		}
		return true;
	}
	else
		return true;
}

bool MainMenu5(eBoton BotonPresionado) // Salir
{
	if (BotonPresionado == botIntro)
	{
		PlayWav(sndBoton);
		bEjecutandose = false;
		return false;
	}
	else
		return false;
}

bool OpcionMenu0(eBoton BotonPresionado) // Vidas
{
	char AuxCad[MaxLongOpcion] = "Vidas ";
	char AuxNum[2];
	if (BotonPresionado == botDerecha   && VidasIniciales < MaxVidas)
	{
		PlayWav(sndToc);
		VidasIniciales++;
        Vidas = VidasIniciales;
	}
	if (BotonPresionado == botIzquierda && VidasIniciales > MinVidas)
	{
		PlayWav(sndToc);
		VidasIniciales--;
        Vidas = VidasIniciales;
	}
	sprintf(AuxNum,"%d",VidasIniciales);
	strcat(AuxCad,AuxNum);
	strcpy(MenuActual.NombreOpciones[0],AuxCad);
	return true;
}

bool OpcionMenu1(eBoton BotonPresionado) // Dificultad
{
	if (BotonPresionado == botDerecha   && Dificultad != inCyberAdicto)
	{
		PlayWav(sndToc);
		Dificultad = (eExtraInfo)((int)Dificultad + 1);
	}
	if (BotonPresionado == botIzquierda && Dificultad != inQuesoRemachado)
	{
		PlayWav(sndToc);
		Dificultad = (eExtraInfo)((int)Dificultad - 1);
	}
    char AuxCad[MaxLongOpcion] = "Dificultad ";
	strcat(AuxCad,NomDif[Dificultad]);
	strcpy(MenuActual.NombreOpciones[1],AuxCad);
	return true;
}

bool OpcionMenu2(eBoton BotonPresionado) // Volver
{
	if (BotonPresionado == botIntro)
	{
		PlayWav(sndBoton);
		MenuActual.NombreOpciones = NOMainMenu;
		MenuActual.NumeroOpciones = 6;
		MenuActual.Funciones      = TFMainMenu;
	}
	return true;
}

bool OpcionSonido(eBoton BotonPresionado)
{
    if (BotonPresionado == botIntro)
	{
		PlayWav(sndBoton);
		MenuActual.NombreOpciones = NOOpcionSonidoMenu;
		MenuActual.NumeroOpciones = 5;
		MenuActual.Funciones      = TFOpcionSonidoMenu;
	}
	return true;
}

bool OpcionSonidoSnd(eBoton BotonPresionado)
{
    if (BotonPresionado == botIntro) haySonido = !haySonido;
    if (haySonido)
        strcpy(NOOpcionSonidoMenu[0],"Sonido On");
    else
        strcpy(NOOpcionSonidoMenu[0],"Sonido Off");
    return true;
}

bool OpcionSonidoSndVol(eBoton BotonPresionado)
{
    if (BotonPresionado == botIzquierda)
    {
        volumenSonido -= 5;
        if (volumenSonido < 0) volumenSonido = 0;
    }
    else if (BotonPresionado == botDerecha)
    {
        volumenSonido += 5;
        if (volumenSonido > 100) volumenSonido = 100;
    }
    Mix_Volume(-1, (volumenSonido * MIX_MAX_VOLUME) / 100);
    sprintf(NOOpcionSonidoMenu[1],"Volumen Sonido %d%%",volumenSonido);
    return true;
}

bool OpcionSonidoMus(eBoton BotonPresionado)
{
    if (BotonPresionado == botIntro) hayMusica = !hayMusica;
    if (hayMusica)
        strcpy(NOOpcionSonidoMenu[2],"Música On");
    else
        strcpy(NOOpcionSonidoMenu[2],"Música Off");
    if (!hayMusica && SDL::playingMusic()) SDL::haltMusic();
    if (hayMusica && !SDL::playingMusic()) SDL::playMusic(musicaMenu,-1);
    return true;
}

bool OpcionSonidoMusVol(eBoton BotonPresionado)
{
    if (BotonPresionado == botIzquierda)
    {
        volumenMusica -= 5;
        if (volumenMusica < 0) volumenMusica = 0;
    }
    else if (BotonPresionado == botDerecha)
    {
        volumenMusica += 5;
        if (volumenMusica > 100) volumenMusica = 100;
    }
    Mix_VolumeMusic((volumenMusica * MIX_MAX_VOLUME) / 100);
    sprintf(NOOpcionSonidoMenu[3],"Volumen Música %d%%",volumenMusica);
    return true;
}

bool OpcionSonidoVolver(eBoton BotonPresionado)
{
    return MainMenu1(BotonPresionado); // Opciones
}

bool MostrarGaleria = false;

bool OpcionArte(eBoton BotonPresionado)
{
    if (BotonPresionado == botIntro)
	{
		PlayWav(sndBoton);
		MostrarGaleria    = true;
		bEjecutandose   = false;
		return true;
	}
	else
		return true;
}

bool CargarNicholas();

bool OpcionSkin(eBoton BotonPresionado)
{
    if (BotonPresionado == botIzquierda && skin > 0)
    {
        PlayWav(sndToc);
        skin = (eSkin)(skin-1);
    }
    else if (BotonPresionado == botDerecha && skin < TOTAL_SKINS - 1)
    {
        PlayWav(sndToc);
        skin = (eSkin)(skin+1);
    }
    sprintf(NOOpcionMenu[4],"Skin: %s",nombreSkin[skin]);
    CargarNicholas();
    return true;
}

bool jugarNivelDeUsuario = false;
char nombreNivelDeUsuario[256];


bool JugarMenu0(eBoton BotonPresionado) // Modo historia
{
    if (BotonPresionado == botIntro)
	{
		PlayWav(sndBoton);
        jugarNivelDeUsuario = false;
		bEjecutandose = false;
		return true;
	}
	else
		return true;
}

bool elegirNivelDeUsuario()
{
    DIR *d;
    dirent *dir;
    d = opendir("Niveles/Usuario");
    if (d)
    {
        std::vector<std::string> v;
        while ((dir = readdir(d)))
        {
            char Aux[256] = "Niveles/Usuario/";
            strcat(Aux,dir->d_name);
            struct stat b;
            if (stat(Aux,&b) == 0)
            {
                if (S_ISREG(b.st_mode) && strlen(dir->d_name) >= 4)
                {
                    std::string s(dir->d_name);
                    if (s.substr(s.size()-4) == ".NNB")
                    {
                        s.erase(s.size()-4);
                        v.push_back(s);
                    }
                }
            }
            else
                std::cerr << "Fallo stat(" << dir->d_name << ")" << std::endl;
        }
        
        closedir(d);
        
        std::sort(v.begin(),v.end());
        
        v.push_back("< CANCELAR >");
        
        memset(Teclado,0,sizeof(Teclado));
        bool Salgamos = false;
        int indice=0, base = 0;
        while (!Salgamos)
        {
            SDL_Event msg;
            if (SDL::pollEvent(msg))
            {
                procesarEvento(msg);
            }
            else
            {
                if (Teclado[SDLK_UP])
                {
                    Teclado[SDLK_UP] = false;
                    if (indice > 0) indice--;
                    PlayWav(sndToc);
                }
                if (Teclado[SDLK_DOWN])
                {
                    Teclado[SDLK_DOWN] = false;
                    if (indice < (int)(v.size()-1)) indice++;
                    PlayWav(sndToc);
                }
                
                while (indice >= base + 10) base++;
                while (indice < base) base--;
                
                SDL_Rect rect;
                rect.x = 30;
                rect.y = 30;
                rect.w = 750;
                rect.h = 500;
                
                SDL::fillRect(SDL::screen(),RGB(0,0,0),&rect);
                
                int y = 40;
                for(int k=0;k<10 && base+k < (int)(v.size());k++)
                {
                    EscribirEnSuperficie(SDL::screen(),60,y,v[base+k].c_str(),RGB(255,255,255),hFuente,RGB(255,0,0));
                    y += 50;
                }
                
                BufferPaint(35,45 + 50 * (indice-base), lpDDSFlecha,15,15);
                
                SDL::flip();
                Salgamos = Teclado[SDLK_RETURN];
            }
        }
        memset(Teclado,0,sizeof(Teclado));
        if (indice < (int)(v.size()-1))
        {
            strcpy(nombreNivelDeUsuario,v[indice].c_str());
            return true;
        }
        else
            return false;
    }
    else
    {
        std::cerr << "No se pudo abrir el directorio Niveles/Usuario" << std::endl;
        return false;
    }
}

bool JugarMenu1(eBoton BotonPresionado) // Jugar nivel de usuario
{
    if (BotonPresionado == botIntro)
	{
		PlayWav(sndBoton);
        if (elegirNivelDeUsuario())
        {
            jugarNivelDeUsuario = true;
            bEjecutandose = false;
        }
	}
	return true;
}

bool JugarMenu2(eBoton BotonPresionado)
{
    if (BotonPresionado == botIntro)
	{
		PlayWav(sndBoton);
		MenuActual.NombreOpciones = NOMainMenu;
		MenuActual.NumeroOpciones = 6;
		MenuActual.Funciones      = TFMainMenu;
	}
	return true;
}

void InicializarTablasFunciones()
{
	TFMainMenu         = new FuncionDeMenu[6];
	TFOpcionMenu       = new FuncionDeMenu[6];
    TFOpcionSonidoMenu = new FuncionDeMenu[5];
    TFJugarMenu        = new FuncionDeMenu[3];
    
    TFJugarMenu[0] = JugarMenu0;
    TFJugarMenu[1] = JugarMenu1;
    TFJugarMenu[2] = JugarMenu2;
    
	TFMainMenu[0]   = MainMenu0;
	TFMainMenu[1]   = MainMenu1;
	TFMainMenu[2]   = MainMenu2;
	TFMainMenu[3]   = MainMenu3;
	TFMainMenu[4]   = MainMenu4;
	TFMainMenu[5]   = MainMenu5;
    
	TFOpcionMenu[0] = OpcionMenu0;
	TFOpcionMenu[1] = OpcionMenu1;
    TFOpcionMenu[2] = OpcionSonido;
    TFOpcionMenu[3] = OpcionArte;
    TFOpcionMenu[4] = OpcionSkin;
	TFOpcionMenu[5] = OpcionMenu2;
    
    TFOpcionSonidoMenu[0] = OpcionSonidoSnd;
    TFOpcionSonidoMenu[1] = OpcionSonidoSndVol;
    TFOpcionSonidoMenu[2] = OpcionSonidoMus;
    TFOpcionSonidoMenu[3] = OpcionSonidoMusVol;
    TFOpcionSonidoMenu[4] = OpcionSonidoVolver;
}

void LiberarTablasFunciones()
{
	delete [] TFMainMenu;
	delete [] TFOpcionMenu;
    delete [] TFOpcionSonidoMenu;
    delete [] TFJugarMenu;
}

bool Menu()
{
    SDL::playMusic(musicaMenu, -1);
    
	SDL_Event    msg;
	bool   retval = true;
	eBoton AuxBoton;

	// Cargamos la imagen del fondo de pantalla.
	DescargarSuperficie(lpDDSFondo);
	lpDDSFondo = CrearSuperficieDesdeBitmap("Graficos/Fondos/MENU.GNB",false);

	// Inicializamos dos o tres boludeces

	Seleccion = 0;

	InicializarTablasFunciones();

	MenuActual.NumeroOpciones = 6;
	MenuActual.NombreOpciones = NOMainMenu;
	MenuActual.Funciones      = TFMainMenu;

	MostrarCreditos = false;
	MostrarAyuda    = false;
	MostrarAcercaDe = false;
    MostrarGaleria  = false;

	LastFrameTime = SDL::getTicks();

	// Esto antes se hacía al final, pero ahora hay que permitir cargar...
	NumNivel   = 1;
	Vidas      = VidasIniciales;
	Puntaje    = 0;
	Monedas    = 0;
	CantidadBF = 0;
	CantidadLaser = 0;

	//Bucle principal
	bEjecutandose = true;
    while (bEjecutandose)
	{
		while (SDL::pollEvent(msg))
	    {
		    procesarEvento(msg);
		}
	    if (g_active == true)
		{
			if (Teclado[SDLK_DOWN] && Seleccion < MenuActual.NumeroOpciones - 1)
			{
				PlayWav(sndToc);
				Teclado[SDLK_DOWN] = false;
				Seleccion++;
			}
			if (Teclado[SDLK_UP] && Seleccion > 0)
			{
				PlayWav(sndToc);
				Teclado[SDLK_UP] = false;
				Seleccion--;
			}

			if (Teclado[SDLK_RETURN])
			{
				Teclado[SDLK_RETURN] = false;
				AuxBoton = botIntro;
			}
			else if (Teclado[SDLK_LEFT])
			{
				Teclado[SDLK_LEFT] = false;
				AuxBoton = botIzquierda;
			}
			else if (Teclado[SDLK_RIGHT])
			{
				Teclado[SDLK_RIGHT] = false;
				AuxBoton = botDerecha;
			}
			else
				AuxBoton = botNulo;

			if (AuxBoton != botNulo)
				retval = MenuActual.Funciones[Seleccion](AuxBoton);
				
            if (Seleccion < 0) Seleccion = 0;
            if (Seleccion >= MenuActual.NumeroOpciones) Seleccion = MenuActual.NumeroOpciones - 1;
                
			MenuBlt();
		}

	}

	LiberarTablasFunciones();

	DescargarSuperficie(lpDDSFondo);

	LimpiarPantalla();
	
    SDL::haltMusic();
    
	return retval;
}

bool Jugar()
{
	SDL_Event msg;
	GameOver = false;
	//Bucle principal
	bEjecutandose = true;
	if (!Cargado)
	{
		CandadoX = -1;
		CandadoY = -1;
	}
	HayCandados = true;
	Cargado = false;
    while (bEjecutandose)
	{

		if (!InicializarJuego())
            return false;

		FinNivel = false;

        SDL::playMusic(musicaJuego,-1);

		while (!FinNivel)
		{
			while (SDL::pollEvent(msg))
	        {
			    procesarEvento(msg);
			}
	        if (g_active == true)
		    {
			
				ActualizarJuego();
				if (!FinNivel)
					Blt();
			}
		}

        SDL::haltMusic();

		if (!GameOver)
			LimpiarPantalla();

		delete Nicho;
		Nicho = NULL;
		EliminarTodo();

	}

	return NivelTerminado;

}

#define VelocidadSangre 0.25

void BloodScreen()
{
	long int k[800];
	double j;
	long   i, LastJ;
	for (i = 0; i < 800; i++)
		k[i] = -rand() % 50 - 5;

	//RECT AuxR;

	Delta = 1;
	LastFrameTime = SDL::getTicks();

	// Copiamos al backbuffer el primary buffer
	//AuxR.left   = AuxR.top = 0;
	//AuxR.right  = 800;
	//AuxR.bottom = 600;
	//BltFast(SDL::screen(),0,0,lpDDSPri,&AuxR);
    
	// A manchar!
	for (LastJ = 0,j = 0; j < 660; j += Delta * VelocidadSangre)
	{
		Delta = TiempoDesde(LastFrameTime);
		LastFrameTime = SDL::getTicks();
		if (Delta > MaxDelta)
			Delta = MaxDelta;
		else if (Delta == 0)
			Delta = 1;
		RojoAPedido(SDL::screen(), k, (long)j - LastJ,false);
		//lpDDSPri->Flip(NULL, DDFLIP_WAIT);
        SDL::flip();
		RojoAPedido(SDL::screen(), k, (long)j - LastJ,true);
		LastJ = (long)j;
	}
}

#define LineasCreditos 16
#define EspacioLineas 40
#define VelocidadLineas 0.05

typedef char ListaCreditos[LineasCreditos][80];

void CreditosBlt(char *ListaLineas, int BaseY)
{
	int  i;
	char *Cadena;
	SIZE Tamano;
	Delta = TiempoDesde(LastFrameTime);
	LastFrameTime = SDL::getTicks();
	if (Delta > MaxDelta)
		Delta = MaxDelta;
	else if (Delta == 0)
		Delta = 1;
        
    SDL::fillSurface(SDL::screen(),RGB(0,0,0));
        
	for (i = 0; i < LineasCreditos; i++)
	{
		Cadena = ListaLineas + i * 80;

		// Averiguamos el tamaño de nuestro texto.
        SDL_Surface *auxSurf = SDL::renderTextSolid(hCreditsFont,Cadena,RGB(0,0,0));
        if (auxSurf)
        {
            Tamano.cx = auxSurf->w + 5; // Dejamos un espacio
            Tamano.cy = auxSurf->h + 5; // extra para la sombra.
            SDL::freeSurface(auxSurf);
            
            // Ya podemos centrarlo respecto a la Y!
            // Lo escribimos con sombra blanca, y el color que salga...
            if (*Cadena == '=')
                EscribirEnSuperficie(SDL::screen(),400 - Tamano.cx / 2,BaseY + i * EspacioLineas,Cadena + 1,RGB(25,100,255),hCreditsFont,RGB(100,255,68));
            else
                EscribirEnSuperficie(SDL::screen(),400 - Tamano.cx / 2,BaseY + i * EspacioLineas,Cadena,RGB(255,0,0),hCreditsFont,RGB(255,255,255));
        }
	}
	//lpDDSPri->Flip(NULL, DDFLIP_WAIT);
    SDL::flip();
}

void VaciarLista(char *ListaLineas)
{
	int  i;
	char *Cadena;
	for (i = 0; i < LineasCreditos; i++)
	{
		Cadena = ListaLineas + i * 80;
		strcpy(Cadena, "");
	}
}

bool SiguienteLinea(char *ListaLineas, std::fstream &FileToUse)
{
	static int CallsAfterEnd = 0;
	int  i;
	for (i = 0; i < LineasCreditos - 1; i++)
		strcpy(ListaLineas + i * 80, ListaLineas + (i + 1) * 80);
	FileToUse.getline(ListaLineas + (LineasCreditos - 1) * 80,80);
	if (FileToUse.eof())
		if (CallsAfterEnd > 0)
		{
			CallsAfterEnd--;
			return CallsAfterEnd > 0;
		}
		else
		{
			CallsAfterEnd = LineasCreditos + 1;
			return true;
		}
	else
		return true;
}

void DoCreditos()
{
	SDL_Event msg;
	ListaCreditos LaLista;
	std::fstream archivo;
	archivo.open("Creditos.cnb",std::ios::in);
    if (archivo.fail())
    {
        std::cerr << "Error al abrir el archivo " << "Creditos.cnb" << std::endl;
        return;
    }
	VaciarLista((char*)LaLista);
	LastFrameTime = SDL::getTicks();
	Delta		  = 1;
    PlayWav(sndDiscursoCreditos);
	//Bucle principal
	bEjecutandose = true;
    while (bEjecutandose)
	{

		while (SDL::pollEvent(msg))
        {
		    procesarEvento(msg);
		}
        if (g_active == true)             
	    {
			for (double Base = 0; Base > -EspacioLineas; Base -= Delta * VelocidadLineas)
			{
                while (SDL::pollEvent(msg))
                    procesarEvento(msg);
				CreditosBlt((char*)LaLista,(int)Base);
				if (AnyKey())
					bEjecutandose = false;
			}
			if (bEjecutandose)
				bEjecutandose = SiguienteLinea((char*)LaLista,archivo) && !AnyKey();
		}

	}

    SDL::stopSound();
    
	archivo.close();
	LimpiarPantalla();
}

#define VelocidadKapuColor 0.25

void AyudaBlt(int page)
{
	RECT AuxR;

	static double KapuColor = 0,Incre = VelocidadKapuColor;

	Delta = TiempoDesde(LastFrameTime);
	LastFrameTime = SDL::getTicks();
	if (Delta > MaxDelta)
		Delta = MaxDelta;
	else if (Delta == 0)
		Delta = 1;
	
	KapuColor += Incre * Delta;
	if (KapuColor > 255)
	{
		KapuColor = 255;
		Incre	  = -VelocidadKapuColor;
	}
	if (KapuColor < 0)
	{
		KapuColor = 0;
		Incre	  = VelocidadKapuColor;
	}
	
	// Fondo

	AuxR.left   = 0;
	AuxR.top    = 0;
	AuxR.right  = 800;
	AuxR.bottom = 600;

	DrawRectangle(SDL::screen(),&AuxR,i2col(0x2F13));

	char LINE[1000];

	std::ifstream Entrada("Ayuda.anb");

	int pp = -1;
	do
	{
		Entrada.getline(LINE,1000);
		if (*LINE == '-')
			pp++;
	} while (pp != page);

	// Titulo

	EscribirEnSuperficie(SDL::screen(),3,0,LINE + 1,RGB((int)KapuColor,70,(int)KapuColor),hTitleFont,RGB(50,50,255 - (int)KapuColor));

	// Ayuditas.

	Entrada.getline(LINE,1000);
	
	SIZE ss;
	int PuntoY = 130;

	while (*LINE != '-' && !Entrada.eof())
	{

        SDL_Surface *auxSurf = SDL::renderTextSolid(hAyudaFont,LINE,RGB(0,0,0));
        if (auxSurf)
        {
            ss.cx = auxSurf->w;
            ss.cy = auxSurf->h;
            SDL::freeSurface(auxSurf);

            EscribirEnSuperficie(SDL::screen(),400 - ss.cx / 2,PuntoY,LINE,RGB(155,(int)KapuColor / 2,(int)KapuColor),hAyudaFont,RGB(255,255,255));
        }
		PuntoY += 24;
		Entrada.getline(LINE,1000);
	}

	// Mostramos todo!

	//lpDDSPri->Flip(NULL, DDFLIP_WAIT);
    SDL::flip();
	Entrada.close();
}

void DoAyuda()
{
	#define PAGES 5
	int page;
	SDL_Event  msg;
	LastFrameTime = SDL::getTicks();
	Delta		  = 1;
	//Bucle principal
	bEjecutandose  = true;
	page = 0;
    while (bEjecutandose)
	{

		while (SDL::pollEvent(msg))
        {
		    procesarEvento(msg);
		}
        if (g_active == true)
	    {
			AyudaBlt(page);
			if (Teclado[SDLK_RIGHT])
			{
				Teclado[SDLK_RIGHT] = false;
				page++;
			}
			if (page == PAGES)
				page--;
			if (Teclado[SDLK_LEFT])
			{
				Teclado[SDLK_LEFT] = false;
				page--;
			}
			if (page < 0)
				page++;
			bEjecutandose = !Teclado[SDLK_ESCAPE];
		}

	}

	LimpiarPantalla();
}

void GaleriaBlt(int page)
{
	SDL::fillSurface(SDL::screen(),i2col(0));

    SDL::blit(187,0,lpConceptArt[page],SDL::screen());

    SDL::flip();
}

void DoGaleria()
{
	int page;
	SDL_Event  msg;
	LastFrameTime = SDL::getTicks();
	Delta		  = 1;
	//Bucle principal
	bEjecutandose  = true;
	page = 0;
    while (bEjecutandose)
	{

		while (SDL::pollEvent(msg))
        {
		    procesarEvento(msg);
		}
        if (g_active == true)
	    {
			GaleriaBlt(page);
			if (Teclado[SDLK_RIGHT])
			{
				Teclado[SDLK_RIGHT] = false;
				page++;
			}
			if (page == CONCEPT_ART_SIZE)
				page--;
			if (Teclado[SDLK_LEFT])
			{
				Teclado[SDLK_LEFT] = false;
				page--;
			}
			if (page < 0)
				page++;
			bEjecutandose = !Teclado[SDLK_ESCAPE];
		}

	}

	LimpiarPantalla();
}

void waitTillEnter()
{
    bEjecutandose  = true;
    while (bEjecutandose)
	{
        SDL_Event msg;
		while (SDL::pollEvent(msg))
        {
		    procesarEvento(msg);
		}
        if (g_active == true)
	    {
			if (Teclado[SDLK_RETURN])
			{
				Teclado[SDLK_RETURN] = false;
                bEjecutandose = false;
			}
		}

	}
}

void DoMachete()
{
    SDL::blit(0,0,parchment,SDL::screen());
    SDL::flip();
    
    waitTillEnter();
    
    SDL::fillSurface(SDL::screen(),RGB(30,30,30));
    SDL::blit(400 - 30,300 - 45,imglstNicholasAni[0],SDL::screen());
    SDL::blit(400 - 33,300 - 67,fuuu,SDL::screen());
    SDL::flip();
    
    waitTillEnter();
    
    SDL::fillSurface(SDL::screen(),RGB(0,0,155));
    EscribirEnSuperficie(SDL::screen(),20,100,"¡Felicitaciones, completaste el modo historia de NichoBros!",RGB(255,255,255),hFuente);
    EscribirEnSuperficie(SDL::screen(),20,130,"Ahora vienen los niveles 'flash'.",RGB(255,255,255),hFuente);
    EscribirEnSuperficie(SDL::screen(),20,160,"Consideralos un bonus de niveles extra, que no cumplen los requisitos",RGB(255,255,255),hFuente);
    EscribirEnSuperficie(SDL::screen(),20,190,"necesarios para estar en la línea de niveles principales.",RGB(255,255,255),hFuente);
    EscribirEnSuperficie(SDL::screen(),20,220,"A partir de ahora nada tiene sentido.",RGB(255,255,255),hFuente);
    SDL::flip();
    
    waitTillEnter();
    
	LimpiarPantalla();
}




long TiempoQueQueda;

void AcercaDeBlt()
{
	RECT AuxR;
	char Cadena[50];

	static double KapuColor = 0,Incre = VelocidadKapuColor;

	Delta = TiempoDesde(LastFrameTime);
	LastFrameTime = SDL::getTicks();
	if (Delta > MaxDelta)
		Delta = MaxDelta;
	else if (Delta == 0)
		Delta = 1;
	
	TiempoQueQueda -= Delta;
	if (TiempoQueQueda < 0)
		TiempoQueQueda = 0;

	KapuColor += Incre * Delta;
	if (KapuColor > 255)
	{
		KapuColor = 255;
		Incre	  = -VelocidadKapuColor;
	}
	if (KapuColor < 0)
	{
		KapuColor = 0;
		Incre	  = VelocidadKapuColor;
	}
	
	// Fondo

	AuxR.left   = 0;
	AuxR.top    = 0;
	AuxR.right  = 800;
	AuxR.bottom = 600;

	DrawRectangle(SDL::screen(),&AuxR,i2col(0xCA33));

	// Titulo

	EscribirEnSuperficie(SDL::screen(),50,2,"Acerca de Nichobros DEMO",RGB(200,(int)KapuColor,70),hTitleFont,RGB(50,50,200));

	// CopyRight

	EscribirEnSuperficie(SDL::screen(),40,80,"Nichobros DEMO CopyRight 2004-2005 TeclaSoft",RGB((int)KapuColor,(int)KapuColor,255),hFuente);

	// Consigue full!

	EscribirEnSuperficie(SDL::screen(),5,105,"¿Que con qué argumentos te convencemos de conseguir la versión completa?",RGB(155,0,(int)KapuColor),hFuente);
	EscribirEnSuperficie(SDL::screen(),5,125,"Estas son solo algunas de las increíbles carácterísticas de Nicho-Bros:",RGB(0,(int)KapuColor,0),hFuente);

	EscribirEnSuperficie(SDL::screen(),15,165,"Muchos más niveles, con más secretos, y más diversión",RGB((int)KapuColor,255,255),hFuente);
	EscribirEnSuperficie(SDL::screen(),15,185,"Muchos más enemigos, más gráficos y sonidos, asegurando una gran experiencia",RGB(255,(int)KapuColor,255),hFuente);
	EscribirEnSuperficie(SDL::screen(),15,205,"¡La aparición estelar del santo! (Encontrarlo no será fácil...)",RGB(255,255,(int)KapuColor),hFuente);
	EscribirEnSuperficie(SDL::screen(),15,225,"¡La aparición estelar de Menem! (Cortesía del cabeza de melón, El 'Dani Galeano')",RGB((int)KapuColor,(int)KapuColor,255),hFuente);
	EscribirEnSuperficie(SDL::screen(),15,245,"Nuevas abilidades de nicholas y más objetos interactivos (¡Palancas y botoncitos!)",RGB((int)KapuColor,255,(int)KapuColor),hFuente);
	EscribirEnSuperficie(SDL::screen(),15,265,"Un instalador en castellano.",RGB(255,(int)KapuColor,(int)KapuColor),hFuente);
	EscribirEnSuperficie(SDL::screen(),15,285,"No tener que soportar estas pantallas típicas de las demos FREEWARE",RGB((int)KapuColor,(int)KapuColor,(int)KapuColor),hFuente);
	EscribirEnSuperficie(SDL::screen(),15,305,"Un juego con problemitas técnicos resueltos (ver créditos)",RGB((int)KapuColor,255,255),hFuente);
	EscribirEnSuperficie(SDL::screen(),15,325,"Una pantalla con los más altos puntajes (salón de la gloria).",RGB(255,(int)KapuColor,255),hFuente);
	EscribirEnSuperficie(SDL::screen(),15,345,"Un editor de niveles completo (¡Para crear tus propios niveles!)",RGB(255,255,(int)KapuColor),hFuente);
	EscribirEnSuperficie(SDL::screen(),15,365,"Más sonidos de calidad, y también ¡Música digital!",RGB((int)KapuColor,(int)KapuColor,255),hFuente);
	EscribirEnSuperficie(SDL::screen(),15,385,"Y muchas, pero muchas sorpresas más...",RGB((int)KapuColor,255,(int)KapuColor),hFuente);

	EscribirEnSuperficie(SDL::screen(),5,425,"Nicho-bros, versión completa. Disponible próximamente, otoño 2006.",RGB(0,(int)KapuColor,0),hFuente);
	EscribirEnSuperficie(SDL::screen(),5,445,"El final nunca estuvo tan cerca... ¡¿Hasta ahora?!",RGB(155,0,(int)KapuColor),hFuente);

	// TiempoQueQueda

	if (TiempoQueQueda > 0)
	{
		//ltoa(TiempoQueQueda / 1000,Cadena,10);
        sprintf(Cadena,"%ld",TiempoQueQueda / 1000);
		EscribirEnSuperficie(SDL::screen(),100,550,"Tiempo restante en seg. (Paciencia...):",RGB(155,155,(int)KapuColor),hFuente);
		EscribirEnSuperficie(SDL::screen(),500,550,Cadena,RGB(155,155,(int)KapuColor),hFuente);
	}
	else
		EscribirEnSuperficie(SDL::screen(),100,550,"¡Listo! Ya podés salir de esta pantalla presionando cualquier tecla.",RGB(155,155,(int)KapuColor),hFuente);
	// Mostramos todo!

	//lpDDSPri->Flip(NULL, DDFLIP_WAIT);
    SDL::flip();
}

void DoAcercaDe()
{
	SDL_Event  msg;
	LastFrameTime = SDL::getTicks();
	Delta		  = 1;
	//Bucle principal
	bEjecutandose  = true;
	TiempoQueQueda = 15000;
    while (bEjecutandose)
	{

		if (SDL::pollEvent(msg))
        {
		    procesarEvento(msg);
		}
        else if (g_active == true)
	    {
			AcercaDeBlt();
			bEjecutandose = !AnyKey() || TiempoQueQueda > 0;
			if (Teclado[SDLK_p] && Teclado[SDLK_z])
				bEjecutandose = false;
		}

	}

	LimpiarPantalla();
}

void GameOverBlt()
{
#define VelMovTex 0.13
	static double XP = -500,Incre = VelMovTex;
	static double ColorCambia = 0, IncreCol = 1;
	RECT AuxR;
	AuxR.left   = 0;
	AuxR.right  = 800;
	AuxR.top	= 0;
	AuxR.bottom = 600;
	DrawRectangle(SDL::screen(),&AuxR,i2col(0xF800));
	EscribirEnSuperficie(SDL::screen(),(long)XP,250,"FIN DEL JUEGO",RGB(ColorCambia,ColorCambia,0),hTitleFont,RGB(255 - ColorCambia,255 - ColorCambia,255));
	Delta = TiempoDesde(LastFrameTime);
	LastFrameTime = SDL::getTicks();
	if (Delta > MaxDelta)
		Delta = MaxDelta;
	else if (Delta == 0)
		Delta = 1;
	ColorCambia += IncreCol * Delta;
	if (ColorCambia > 255)
	{
		ColorCambia = 255;
		IncreCol	= -1;
	}
	if (ColorCambia < 0)
	{
		ColorCambia = 0;
		IncreCol	= 1;
	}
	XP += Incre * Delta;
	if (XP > 805)
		Incre = -VelMovTex;
	if (XP < -500)
		Incre = VelMovTex;
	//lpDDSPri->Flip(NULL,DDFLIP_WAIT);
    SDL::flip();
}

void DoGameOver()
{
	SDL_Event msg;

	BloodScreen();

	LastFrameTime = SDL::getTicks();

	//Bucle principal
	bEjecutandose = true;
    while (bEjecutandose)
	{

		if (SDL::pollEvent(msg))
        {
		    procesarEvento(msg);
		}
        else if (g_active == true)
	    {
			bEjecutandose = !AnyKey();
			GameOverBlt();
		}

	}

	LimpiarPantalla();
}

bool InicializarSDL()
{ 
    if (!SDL::init(800, 600, 16,true, "Nichobros"))
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
    hOptionFont = NULL;
    hAyudaFont = NULL;
    hCreditsFont = NULL;
    hTitleFont = NULL;
    
    //Open the fonts
	if ((hFuente = SDL::openFont( "Fuentes/ARIAL.TTF", 24 )) == NULL ||
        (hOptionFont = SDL::openFont( "Fuentes/IMPACT.TTF", 55 )) == NULL ||
        (hTitleFont = SDL::openFont( "Fuentes/VINERITC.TTF", 96 )) == NULL ||
        (hAyudaFont = SDL::openFont( "Fuentes/ARIAL.TTF", 21 )) == NULL ||
        (hCreditsFont = SDL::openFont( "Fuentes/STYLU.TTF", 30 )) == NULL)
    {
        std::cerr << "SDL::openFont() trouble." << std::endl;
        return false;
    }
    
    if (!SDL::openAudio(44100, MIX_DEFAULT_FORMAT, true, 1024))
    {
        std::cerr << "SDL::openAudio() trouble." << std::endl;
        return false;
    }

    SDL_ShowCursor(SDL_DISABLE);
    
    return true;
} 

void DescargarSuperficie(LPDIRECTDRAWSURFACE &Surf)
{
	if (Surf != NULL)
	{
        SDL::freeSurface(Surf);
		Surf = NULL;
	}
}

void DescargarColeccion(Coleccion *MiColec)
{
	if (MiColec != NULL)
	{
		delete MiColec;
		MiColec = NULL;
	}
}

void FinalizarSDL()
{
    
	// Liberamos nuestras colecciones.
	if (Nicho != NULL)
	{
		delete Nicho;
		Nicho = NULL;
	}

	if (Nivel != NULL)
	{
		delete Nivel;
		Nivel = NULL;
	}

	DescargarColecciones();

    // Liberamos imágenes utilizadas por nuestro juego...
    DescargarSuperficie(lpDDSFondo);
    DescargarSuperficie(lpDDSBarraCansancio);
    DescargarSuperficie(lpDDSBarraVerde);
    DescargarSuperficie(lpDDSFlecha);
    DescargarSuperficie(fuuu);
    DescargarSuperficie(parchment);
    // Asignamos a NULL aquellos punteros que en realidad
    // apuntan a otro fijo, es decir, son variables y no
    // sería correcto descargar la superficie a la que
    // apuntan, ya que esta se descargaría dos veces, pues
    // existe un puntero fijo apuntando siempre a esta
    // superficie.
    imglstTerreno   [caBloqueInterr - 1] = NULL;
    imglstTerreno   [caFuegoLava    - 1] = NULL;
    imglstTerreno   [caAgua         - 1] = NULL;
    imglstTerreno   [caCalentador   - 1] = NULL;
    imglstParticulas[partVerde		   ] = NULL;
    imglstFuego		[fuegArribaPico    ] = NULL;
    imglstFuego		[fuegAbajoPico     ] = NULL;
    imglstJefes     [RobotsNormal	   ] = NULL;
    imglstPremios   [prCandado		   ] = NULL;
    imglstPremios   [prLaser           ] = NULL;
    imglstPremios   [prBolaFuego       ] = NULL;
    imglstCosasMalas[cmTriturin		   ] = NULL;
    int k;
    // imglstNicholas
    for (k = 0; k < TAMANO_NICHOLAS; k++)
        imglstNicholas[k] = NULL;

    // Empezamos a descargar las verdaderas imagenes

    // imglstTerreno
    for (k = 0; k < TAMANO_TERRENO; k++)
        DescargarSuperficie(imglstTerreno[k]);
    // imglstPremios
    for (k = 0; k < TAMANO_PREMIOS; k++)
        DescargarSuperficie(imglstPremios[k]);
    // imglstEnemigos
    for (k = 0; k < TAMANO_ENEMIGOS; k++)
        DescargarSuperficie(imglstEnemigos[k]);
    // imglstExplosiones
    for (k = 0; k < TAMANO_EXPLOSIONES; k++)
        DescargarSuperficie(imglstExplosiones[k]);
    // imglstAnimaciones
    for (k = 0; k < TAMANO_ANIMACIONES; k++)
        DescargarSuperficie(imglstAnimaciones[k]);
    // imglstFuego
    for (k = 0; k < TAMANO_FUEGO; k++)
        DescargarSuperficie(imglstFuego[k]);
    // imglstRebotador
    for (k = 0; k < TAMANO_REBOTADOR; k++)
        DescargarSuperficie(imglstRebotador[k]);
    // imglstRenders
    for (k = 0; k < TAMANO_RENDERS; k++)
        DescargarSuperficie(imglstRenders[k]);
    // imglstNicholasAni
    for (k = 0; k < TAMANO_NICHOLASANI; k++)
        DescargarSuperficie(imglstNicholasAni[k]);
    // imglstGreenRare
    for (k = 0; k < TAMANO_GREENRARE; k++)
        DescargarSuperficie(imglstGreenRare[k]);
    // imglstRobots
    for (k = 0; k < TAMANO_ROBOTS; k++)
        DescargarSuperficie(imglstRobots[k]);
    // imglstJefes
    for (k = 0; k < TAMANO_JEFES; k++)
        DescargarSuperficie(imglstJefes[k]);
    // imglstEnergia
    for (k = 0; k < TAMANO_ENERGIA; k++)
        DescargarSuperficie(imglstEnergia[k]);
    // imglstPlataformas
    for (k = 0; k < TAMANO_PLATAFORMAS; k++)
        DescargarSuperficie(imglstPlataformas[k]);
    // imglstCosasMalas
    for (k = 0; k < TAMANO_COSASMALAS; k++)
        DescargarSuperficie(imglstCosasMalas[k]);
    // imglstAparejos
    for (k = 0; k < TAMANO_APAREJOS; k++)
        DescargarSuperficie(imglstAparejos[k]);

    //  conceptArt
    for(int i=0;i<CONCEPT_ART_SIZE;i++)
        DescargarSuperficie(lpConceptArt[i]);

    // Limpieza mixer
    
   
    SDL::stopAllChannels();

    for (k = 0; k < TAMANO_SONIDO; k++)
        if (lpDSBuffers[k] != NULL)
        {
            SDL::freeChunk(lpDSBuffers[k]);
            lpDSBuffers[k] = NULL;
        }

    if (musicaJuego != NULL) SDL::freeMusic(musicaJuego);
    if (musicaJuego != NULL) SDL::freeMusic(musicaMenu);
 
    SDL::closeAudio();
    
    // Limpieza TTF
    
    SDL::closeFont(hFuente);
    SDL::closeFont(hAyudaFont);
    SDL::closeFont(hTitleFont);
    SDL::closeFont(hCreditsFont);
    SDL::closeFont(hOptionFont);
    
    SDL::quitTTF();
    
    // Nos vamos final!
    
    SDL::quit();
}


LPDIRECTDRAWSURFACE CrearBarraCansancio()
{
	LPDIRECTDRAWSURFACE lpDDSTemp;
    {
        SDL_Surface *rval;
        rval = SDL_CreateRGBSurface(SDL_SWSURFACE,100,20,16,0,0,0,0);
        if (!rval) return NULL;
        lpDDSTemp = SDL_DisplayFormat(rval);
        SDL::freeSurface(rval);
    }
    if (lpDDSTemp == NULL) return NULL;
	// Dibujamos la info en nuestra superficie.
	SDL_LockSurface(lpDDSTemp);
	int AuxX;
    Uint16 *pixels = (Uint16 *)lpDDSTemp->pixels;
    Uint16 ColorValue;
	for (AuxX = 0; AuxX < 100; AuxX++)
	{
		//MoveToEx(TempDC,AuxX,0, NULL);
		//DeleteObject(SelectObject(TempDC,CreatePen(PS_SOLID,0,RGB(ColorValue,ColorValue / 4,0))));
		//LineTo(TempDC,AuxX,20);
        ColorValue = (((25 + AuxX) / 4) << 11) +
                     (((25 + AuxX) / 16) << 5);
        for(int i=0;i<20;i++)
            pixels[i*(lpDDSTemp->pitch/2) + AuxX] = ColorValue;
	}
	SDL_UnlockSurface(lpDDSTemp);
	return lpDDSTemp;
}

LPDIRECTDRAWSURFACE CrearBarraVerde()
{
	LPDIRECTDRAWSURFACE lpDDSTemp;
    {
        SDL_Surface *rval;
        rval = SDL_CreateRGBSurface(SDL_SWSURFACE,100,20,16,0,0,0,0);
        if (!rval) return NULL;
        lpDDSTemp = SDL_DisplayFormat(rval);
        SDL::freeSurface(rval);
    }
    if (lpDDSTemp == NULL) return NULL;
	// Dibujamos la info en nuestra superficie.
	SDL_LockSurface(lpDDSTemp);
	int AuxX;
    Uint16 *pixels = (Uint16 *)lpDDSTemp->pixels;
    Uint16 ColorValue;
	for (AuxX = 0; AuxX < 100; AuxX++)
	{
		//MoveToEx(TempDC,AuxX,0, NULL);
		//DeleteObject(SelectObject(TempDC,CreatePen(PS_SOLID,0,RGB(ColorValue,ColorValue / 4,0))));
		//LineTo(TempDC,AuxX,20);
        ColorValue = (((25 + AuxX) / 4) << 5) +
                     (((25 + AuxX) / 16) << 11);
        for(int i=0;i<20;i++)
            pixels[i*(lpDDSTemp->pitch/2) + AuxX] = ColorValue;
	}
	SDL_UnlockSurface(lpDDSTemp);
	return lpDDSTemp;
}

/*
LPDIRECTDRAWSURFACE CrearBarraVerde()
{
	LPDIRECTDRAWSURFACE lpDDSTemp;
	DDSURFACEDESC ddsd;
	HRESULT rval;
	HDC TempDC;
	//Inicializo la superficie a devolver
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize		    = sizeof(ddsd);
    ddsd.dwFlags		= DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth		= 100;
	ddsd.dwHeight		= 20;
	rval = lpDD->CreateSurface(&ddsd, &lpDDSTemp, NULL);
    if (rval != DD_OK) return NULL;
	// Dibujamos la info en nuestra superficie.
	lpDDSTemp->GetDC(&TempDC);
	int AuxX, ColorValue = 50;
	for (AuxX = 0; AuxX < 100; AuxX++, ColorValue += 2)
	{
		MoveToEx(TempDC,AuxX,0, NULL);
		DeleteObject(SelectObject(TempDC,CreatePen(PS_SOLID,0,RGB(ColorValue / 4,ColorValue,0))));
		LineTo(TempDC,AuxX,20);
	}
	lpDDSTemp->ReleaseDC(TempDC);
	return lpDDSTemp;
}*/

bool CargarNicholas()
{
    for(int i=0;i<100;i++) DescargarSuperficie(imglstNicholasAni[i]);
    std::string path = (std::string("Graficos/Nicholas/s") + char('0'+skin+1)) + '/';
    
    #define check(x) if (!(x)) {std::cerr << "Error al cargar la imagen." << std::endl; std::cerr << #x << std::endl; return false;}
    
    check(imglstNicholasAni[0 ] = CrearSuperficieDesdeBitmap((path + "NicholasDe.gnb").c_str()));
	check(imglstNicholasAni[1 ] = CrearSuperficieDesdeBitmap((path + "NicholasIz.gnb").c_str()));
	check(imglstNicholasAni[2 ] = CrearSuperficieDesdeBitmap((path + "NicholasCaDe01.gnb").c_str()));
	check(imglstNicholasAni[3 ] = CrearSuperficieDesdeBitmap((path + "NicholasCaDe02.gnb").c_str()));
	check(imglstNicholasAni[4 ] = CrearSuperficieDesdeBitmap((path + "NicholasCaDe03.gnb").c_str()));
	check(imglstNicholasAni[5 ] = CrearSuperficieDesdeBitmap((path + "NicholasCaDe04.gnb").c_str()));
	check(imglstNicholasAni[6 ] = CrearSuperficieDesdeBitmap((path + "NicholasCaDe05.gnb").c_str()));
	check(imglstNicholasAni[7 ] = CrearSuperficieDesdeBitmap((path + "NicholasCaDe06.gnb").c_str()));
	check(imglstNicholasAni[8 ] = CrearSuperficieDesdeBitmap((path + "NicholasCaDe07.gnb").c_str()));
	check(imglstNicholasAni[9 ] = CrearSuperficieDesdeBitmap((path + "NicholasCaDe08.gnb").c_str()));
	check(imglstNicholasAni[10] = CrearSuperficieDesdeBitmap((path + "NicholasCaDe09.gnb").c_str()));
	check(imglstNicholasAni[11] = CrearSuperficieDesdeBitmap((path + "NicholasCaDe10.gnb").c_str()));
	check(imglstNicholasAni[12] = CrearSuperficieDesdeBitmap((path + "NicholasCaDe11.gnb").c_str()));
	check(imglstNicholasAni[13] = CrearSuperficieDesdeBitmap((path + "NicholasCaDe12.gnb").c_str()));
	check(imglstNicholasAni[14] = CrearSuperficieDesdeBitmap((path + "NicholasCaDe13.gnb").c_str()));
	check(imglstNicholasAni[15] = CrearSuperficieDesdeBitmap((path + "NicholasCaDe14.gnb").c_str()));
	check(imglstNicholasAni[16] = CrearSuperficieDesdeBitmap((path + "NicholasCaIz01.gnb").c_str()));
	check(imglstNicholasAni[17] = CrearSuperficieDesdeBitmap((path + "NicholasCaIz02.gnb").c_str()));
	check(imglstNicholasAni[18] = CrearSuperficieDesdeBitmap((path + "NicholasCaIz03.gnb").c_str()));
	check(imglstNicholasAni[19] = CrearSuperficieDesdeBitmap((path + "NicholasCaIz04.gnb").c_str()));
	check(imglstNicholasAni[20] = CrearSuperficieDesdeBitmap((path + "NicholasCaIz05.gnb").c_str()));
	check(imglstNicholasAni[21] = CrearSuperficieDesdeBitmap((path + "NicholasCaIz06.gnb").c_str()));
	check(imglstNicholasAni[22] = CrearSuperficieDesdeBitmap((path + "NicholasCaIz07.gnb").c_str()));
	check(imglstNicholasAni[23] = CrearSuperficieDesdeBitmap((path + "NicholasCaIz08.gnb").c_str()));
	check(imglstNicholasAni[24] = CrearSuperficieDesdeBitmap((path + "NicholasCaIz09.gnb").c_str()));
	check(imglstNicholasAni[25] = CrearSuperficieDesdeBitmap((path + "NicholasCaIz10.gnb").c_str()));
	check(imglstNicholasAni[26] = CrearSuperficieDesdeBitmap((path + "NicholasCaIz11.gnb").c_str()));
	check(imglstNicholasAni[27] = CrearSuperficieDesdeBitmap((path + "NicholasCaIz12.gnb").c_str()));
	check(imglstNicholasAni[28] = CrearSuperficieDesdeBitmap((path + "NicholasCaIz13.gnb").c_str()));
	check(imglstNicholasAni[29] = CrearSuperficieDesdeBitmap((path + "NicholasCaIz14.gnb").c_str()));
	check(imglstNicholasAni[30] = CrearSuperficieDesdeBitmap((path + "NicholasSalDe.gnb").c_str()));
	check(imglstNicholasAni[31] = CrearSuperficieDesdeBitmap((path + "NicholasSalIz.gnb").c_str()));
	check(imglstNicholasAni[32] = CrearSuperficieDesdeBitmap((path + "NicholasTreDe1.gnb").c_str()));
	check(imglstNicholasAni[33] = CrearSuperficieDesdeBitmap((path + "NicholasTreDe2.gnb").c_str()));
	check(imglstNicholasAni[34] = CrearSuperficieDesdeBitmap((path + "NicholasTreDe3.gnb").c_str()));
	check(imglstNicholasAni[35] = CrearSuperficieDesdeBitmap((path + "NicholasTreDe4.gnb").c_str()));
	check(imglstNicholasAni[36] = CrearSuperficieDesdeBitmap((path + "NicholasTreDe5.gnb").c_str()));
	check(imglstNicholasAni[37] = CrearSuperficieDesdeBitmap((path + "NicholasTreDe6.gnb").c_str()));
	check(imglstNicholasAni[38] = CrearSuperficieDesdeBitmap((path + "NicholasTreIz1.gnb").c_str()));
	check(imglstNicholasAni[39] = CrearSuperficieDesdeBitmap((path + "NicholasTreIz2.gnb").c_str()));
	check(imglstNicholasAni[40] = CrearSuperficieDesdeBitmap((path + "NicholasTreIz3.gnb").c_str()));
	check(imglstNicholasAni[41] = CrearSuperficieDesdeBitmap((path + "NicholasTreIz4.gnb").c_str()));
	check(imglstNicholasAni[42] = CrearSuperficieDesdeBitmap((path + "NicholasTreIz5.gnb").c_str()));
	check(imglstNicholasAni[43] = CrearSuperficieDesdeBitmap((path + "NicholasTreIz6.gnb").c_str()));
	check(imglstNicholasAni[44] = CrearSuperficieDesdeBitmap((path + "NicholasBotDe1.gnb").c_str()));
	check(imglstNicholasAni[45] = CrearSuperficieDesdeBitmap((path + "NicholasBotDe2.gnb").c_str()));
	check(imglstNicholasAni[46] = CrearSuperficieDesdeBitmap((path + "NicholasBotDe3.gnb").c_str()));
	check(imglstNicholasAni[47] = CrearSuperficieDesdeBitmap((path + "NicholasBotIz1.gnb").c_str()));
	check(imglstNicholasAni[48] = CrearSuperficieDesdeBitmap((path + "NicholasBotIz2.gnb").c_str()));
	check(imglstNicholasAni[49] = CrearSuperficieDesdeBitmap((path + "NicholasBotIz3.gnb").c_str()));

	check(imglstNicholasAni[50] = CrearSuperficieDesdeBitmap((path + "SNicholasDe.gnb").c_str()));
	check(imglstNicholasAni[51] = CrearSuperficieDesdeBitmap((path + "SNicholasIz.gnb").c_str()));
	check(imglstNicholasAni[52] = CrearSuperficieDesdeBitmap((path + "SNicholasCaDe01.gnb").c_str()));
	check(imglstNicholasAni[53] = CrearSuperficieDesdeBitmap((path + "SNicholasCaDe02.gnb").c_str()));
	check(imglstNicholasAni[54] = CrearSuperficieDesdeBitmap((path + "SNicholasCaDe03.gnb").c_str()));
	check(imglstNicholasAni[55] = CrearSuperficieDesdeBitmap((path + "SNicholasCaDe04.gnb").c_str()));
	check(imglstNicholasAni[56] = CrearSuperficieDesdeBitmap((path + "SNicholasCaDe05.gnb").c_str()));
	check(imglstNicholasAni[57] = CrearSuperficieDesdeBitmap((path + "SNicholasCaDe06.gnb").c_str()));
	check(imglstNicholasAni[58] = CrearSuperficieDesdeBitmap((path + "SNicholasCaDe07.gnb").c_str()));
	check(imglstNicholasAni[59] = CrearSuperficieDesdeBitmap((path + "SNicholasCaDe08.gnb").c_str()));
	check(imglstNicholasAni[60] = CrearSuperficieDesdeBitmap((path + "SNicholasCaDe09.gnb").c_str()));
	check(imglstNicholasAni[61] = CrearSuperficieDesdeBitmap((path + "SNicholasCaDe10.gnb").c_str()));
	check(imglstNicholasAni[62] = CrearSuperficieDesdeBitmap((path + "SNicholasCaDe11.gnb").c_str()));
	check(imglstNicholasAni[63] = CrearSuperficieDesdeBitmap((path + "SNicholasCaDe12.gnb").c_str()));
	check(imglstNicholasAni[64] = CrearSuperficieDesdeBitmap((path + "SNicholasCaDe13.gnb").c_str()));
	check(imglstNicholasAni[65] = CrearSuperficieDesdeBitmap((path + "SNicholasCaDe14.gnb").c_str()));
	check(imglstNicholasAni[66] = CrearSuperficieDesdeBitmap((path + "SNicholasCaIz01.gnb").c_str()));
	check(imglstNicholasAni[67] = CrearSuperficieDesdeBitmap((path + "SNicholasCaIz02.gnb").c_str()));
	check(imglstNicholasAni[68] = CrearSuperficieDesdeBitmap((path + "SNicholasCaIz03.gnb").c_str()));
	check(imglstNicholasAni[69] = CrearSuperficieDesdeBitmap((path + "SNicholasCaIz04.gnb").c_str()));
	check(imglstNicholasAni[70] = CrearSuperficieDesdeBitmap((path + "SNicholasCaIz05.gnb").c_str()));
	check(imglstNicholasAni[71] = CrearSuperficieDesdeBitmap((path + "SNicholasCaIz06.gnb").c_str()));
	check(imglstNicholasAni[72] = CrearSuperficieDesdeBitmap((path + "SNicholasCaIz07.gnb").c_str()));
	check(imglstNicholasAni[73] = CrearSuperficieDesdeBitmap((path + "SNicholasCaIz08.gnb").c_str()));
	check(imglstNicholasAni[74] = CrearSuperficieDesdeBitmap((path + "SNicholasCaIz09.gnb").c_str()));
	check(imglstNicholasAni[75] = CrearSuperficieDesdeBitmap((path + "SNicholasCaIz10.gnb").c_str()));
	check(imglstNicholasAni[76] = CrearSuperficieDesdeBitmap((path + "SNicholasCaIz11.gnb").c_str()));
	check(imglstNicholasAni[77] = CrearSuperficieDesdeBitmap((path + "SNicholasCaIz12.gnb").c_str()));
	check(imglstNicholasAni[78] = CrearSuperficieDesdeBitmap((path + "SNicholasCaIz13.gnb").c_str()));
	check(imglstNicholasAni[79] = CrearSuperficieDesdeBitmap((path + "SNicholasCaIz14.gnb").c_str()));
	check(imglstNicholasAni[80] = CrearSuperficieDesdeBitmap((path + "SNicholasSalDe.gnb").c_str()));
	check(imglstNicholasAni[81] = CrearSuperficieDesdeBitmap((path + "SNicholasSalIz.gnb").c_str()));
	check(imglstNicholasAni[82] = CrearSuperficieDesdeBitmap((path + "SNicholasTreDe1.gnb").c_str()));
	check(imglstNicholasAni[83] = CrearSuperficieDesdeBitmap((path + "SNicholasTreDe2.gnb").c_str()));
	check(imglstNicholasAni[84] = CrearSuperficieDesdeBitmap((path + "SNicholasTreDe3.gnb").c_str()));
	check(imglstNicholasAni[85] = CrearSuperficieDesdeBitmap((path + "SNicholasTreDe4.gnb").c_str()));
	check(imglstNicholasAni[86] = CrearSuperficieDesdeBitmap((path + "SNicholasTreDe5.gnb").c_str()));
	check(imglstNicholasAni[87] = CrearSuperficieDesdeBitmap((path + "SNicholasTreDe6.gnb").c_str()));
	check(imglstNicholasAni[88] = CrearSuperficieDesdeBitmap((path + "SNicholasTreIz1.gnb").c_str()));
	check(imglstNicholasAni[89] = CrearSuperficieDesdeBitmap((path + "SNicholasTreIz2.gnb").c_str()));
	check(imglstNicholasAni[90] = CrearSuperficieDesdeBitmap((path + "SNicholasTreIz3.gnb").c_str()));
	check(imglstNicholasAni[91] = CrearSuperficieDesdeBitmap((path + "SNicholasTreIz4.gnb").c_str()));
	check(imglstNicholasAni[92] = CrearSuperficieDesdeBitmap((path + "SNicholasTreIz5.gnb").c_str()));
	check(imglstNicholasAni[93] = CrearSuperficieDesdeBitmap((path + "SNicholasTreIz6.gnb").c_str()));
	check(imglstNicholasAni[94] = CrearSuperficieDesdeBitmap((path + "SNicholasBotDe1.gnb").c_str()));
	check(imglstNicholasAni[95] = CrearSuperficieDesdeBitmap((path + "SNicholasBotDe2.gnb").c_str()));
	check(imglstNicholasAni[96] = CrearSuperficieDesdeBitmap((path + "SNicholasBotDe3.gnb").c_str()));
	check(imglstNicholasAni[97] = CrearSuperficieDesdeBitmap((path + "SNicholasBotIz1.gnb").c_str()));
	check(imglstNicholasAni[98] = CrearSuperficieDesdeBitmap((path + "SNicholasBotIz2.gnb").c_str()));
	check(imglstNicholasAni[99] = CrearSuperficieDesdeBitmap((path + "SNicholasBotIz3.gnb").c_str()));
    
    return true;
}

bool InicializarSuperficies()
{ 
    // Creacion de superficies del juego.
    
	// Barras con el cansancio.

	check(lpDDSBarraCansancio = CrearBarraCansancio());
	check(lpDDSBarraVerde     = CrearBarraVerde());
	check(lpDDSFlecha         = CrearSuperficieDesdeBitmap("Graficos/Cursores/Flecha.gnb"));
    check(parchment           = CrearSuperficieDesdeBitmap("Graficos/Fondos/PARCHMENT.GNB",false));
    
    // imglstTerreno
    check(imglstTerreno[0]  = CrearSuperficieDesdeBitmap("Graficos/Pisos/Comun.gnb"));
    check(imglstTerreno[1]  = CrearSuperficieDesdeBitmap("Graficos/Pisos/Metal.gnb"));
    check(imglstTerreno[2]  = CrearSuperficieDesdeBitmap("Graficos/Pisos/Piedra.gnb"));
    check(imglstTerreno[3]  = CrearSuperficieDesdeBitmap("Graficos/Carteles/Salida.gnb"));
  //imglstTerreno[4]  = CrearSuperficieDesdeBitmap("Graficos/Bloques/Interrogacion.gnb");
    check(imglstTerreno[5]  = CrearSuperficieDesdeBitmap("Graficos/Bloques/Simple.gnb"));
	check(imglstTerreno[6]  = CrearSuperficieDesdeBitmap("Graficos/Bloques/Anulado.gnb"));
  //imglstTerreno[7]  = PUNTERO!
	check(imglstTerreno[8]  = CrearSuperficieDesdeBitmap("Graficos/Pasto/PastoIzquierda.gnb"));
	check(imglstTerreno[9]  = CrearSuperficieDesdeBitmap("Graficos/Pasto/PastoMedio.gnb"));
	check(imglstTerreno[10] = CrearSuperficieDesdeBitmap("Graficos/Pasto/PastoDerecha.gnb"));
  //imglstTerreno[11] = PUNTERO!
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
    //PUNTERO! check(imglstTerreno[37] = CrearSuperficieDesdeBitmap("Graficos/Playa/AGUA1.gnb"));
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
    
    // imglstNicholas

    //imglstNicholas[N]  = Son todos punteros!

    // imglstPremios
    check(imglstPremios[0]   = CrearSuperficieDesdeBitmap("Graficos/Premios/Speed.gnb"));
	check(imglstPremios[1]   = CrearSuperficieDesdeBitmap("Graficos/Premios/Moneda.gnb"));
	//imglstPremios[2] = imglstFuego[0];
	check(imglstPremios[3]   = CrearSuperficieDesdeBitmap("Graficos/Premios/Escudo.gnb"));
	check(imglstPremios[4]   = CrearSuperficieDesdeBitmap("Graficos/Premios/Aureola.gnb"));
	//imglstPremios[5] = imglstAnimaciones[6]
	//imglstPremios[6] = imglstAnimaciones[19]
	check(imglstPremios[7]   = CrearSuperficieDesdeBitmap("Graficos/Premios/Cereza.gnb"));
	check(imglstPremios[8]   = CrearSuperficieDesdeBitmap("Graficos/Premios/Frutilla.gnb"));
	check(imglstPremios[9]   = CrearSuperficieDesdeBitmap("Graficos/Premios/Juguito.gnb"));
	check(imglstPremios[10]  = CrearSuperficieDesdeBitmap("Graficos/Premios/Banana.gnb"));
	check(imglstPremios[11]  = CrearSuperficieDesdeBitmap("Graficos/Premios/PizzaVeggy.gnb"));
	check(imglstPremios[12]  = CrearSuperficieDesdeBitmap("Graficos/Premios/Licuado.gnb"));
	check(imglstPremios[13]  = CrearSuperficieDesdeBitmap("Graficos/Premios/Pizza.gnb"));
	check(imglstPremios[14]  = CrearSuperficieDesdeBitmap("Graficos/Premios/PizzaCompleta.gnb"));
	check(imglstPremios[15]  = CrearSuperficieDesdeBitmap("Graficos/Premios/Radio.gnb"));
	check(imglstPremios[16]  = CrearSuperficieDesdeBitmap("Graficos/Premios/LiquidoVerde.gnb"));
    // imglstEnemigos
    check(imglstEnemigos[0]  = CrearSuperficieDesdeBitmap("Graficos/Enemigos/Juanchi1.gnb"));
    check(imglstEnemigos[1]  = CrearSuperficieDesdeBitmap("Graficos/Enemigos/Juanchi2.gnb"));
    check(imglstEnemigos[2]  = CrearSuperficieDesdeBitmap("Graficos/Enemigos/JuanchiAp.gnb"));
    check(imglstEnemigos[3]  = CrearSuperficieDesdeBitmap("Graficos/Enemigos/JuanchiRev.gnb"));
    check(imglstEnemigos[4]  = CrearSuperficieDesdeBitmap("Graficos/Enemigos/TortugaDe.gnb"));
    check(imglstEnemigos[5]  = CrearSuperficieDesdeBitmap("Graficos/Enemigos/TortugaCaDe1.gnb"));
    check(imglstEnemigos[6]  = CrearSuperficieDesdeBitmap("Graficos/Enemigos/TortugaCaDe2.gnb"));
    check(imglstEnemigos[7]  = CrearSuperficieDesdeBitmap("Graficos/Enemigos/TortugaIz.gnb"));
    check(imglstEnemigos[8]  = CrearSuperficieDesdeBitmap("Graficos/Enemigos/TortugaCaIz1.gnb"));
    check(imglstEnemigos[9]  = CrearSuperficieDesdeBitmap("Graficos/Enemigos/TortugaCaIz2.gnb"));
    check(imglstEnemigos[10] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/TortugaRev.gnb"));
    check(imglstEnemigos[11] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/GreenRareRev.gnb"));
	check(imglstEnemigos[12] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/Caecucho.gnb"));
	check(imglstEnemigos[13] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/Caecucho2.gnb"));
	check(imglstEnemigos[14] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/Caecucho3.gnb"));
	check(imglstEnemigos[15] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/Caecucho4.gnb"));
	check(imglstEnemigos[16] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/Caecucho5.gnb"));
	check(imglstEnemigos[17] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/SaurioloDe1.gnb"));
	check(imglstEnemigos[18] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/SaurioloDe2.gnb"));
	check(imglstEnemigos[19] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/SaurioloDe3.gnb"));
	check(imglstEnemigos[20] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/SaurioloIz1.gnb"));
	check(imglstEnemigos[21] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/SaurioloIz2.gnb"));
	check(imglstEnemigos[22] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/SaurioloIz3.gnb"));
	check(imglstEnemigos[23] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/SaurioloRevDe.gnb"));
	check(imglstEnemigos[24] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/SaurioloRevIz.gnb"));
	check(imglstEnemigos[25] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/CaparatuzoDe.gnb"));
	check(imglstEnemigos[26] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/CaparatuzoDe2.gnb"));
	check(imglstEnemigos[27] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/CaparatuzoIz.gnb"));
	check(imglstEnemigos[28] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/CaparatuzoIz2.gnb"));
	check(imglstEnemigos[29] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/CaparatuzoRev.gnb"));
	check(imglstEnemigos[30] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/BichotoDe.gnb"));
	check(imglstEnemigos[31] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/BichotoDe2.gnb"));
	check(imglstEnemigos[32] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/BichotoIz.gnb"));
	check(imglstEnemigos[33] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/BichotoIz2.gnb"));
	check(imglstEnemigos[34] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/BichotoRev.gnb"));
    check(imglstEnemigos[35] = CrearSuperficieDesdeBitmap("Graficos/Playa/PEZABAJO.gnb"));
    check(imglstEnemigos[36] = CrearSuperficieDesdeBitmap("Graficos/Playa/PEZARRIBA.gnb"));
    // imglstExplosiones
    check(imglstExplosiones[0] = CrearSuperficieDesdeBitmap("Graficos/Explosiones/Piedra1.gnb"));
    check(imglstExplosiones[1] = CrearSuperficieDesdeBitmap("Graficos/Explosiones/Piedra2.gnb"));
    check(imglstExplosiones[2] = CrearSuperficieDesdeBitmap("Graficos/Explosiones/Piedra3.gnb"));
    check(imglstExplosiones[3] = CrearSuperficieDesdeBitmap("Graficos/Explosiones/Piedra4.gnb"));
	check(imglstExplosiones[4] = CrearSuperficieDesdeBitmap("Graficos/Explosiones/explosion1.gnb"));
    // imglstAnimaciones
    check(imglstAnimaciones[0]  = CrearSuperficieDesdeBitmap("Graficos/Bloques/Interrogacion.gnb"));
    check(imglstAnimaciones[1]  = CrearSuperficieDesdeBitmap("Graficos/Bloques/Interrogacion2.gnb"));
    check(imglstAnimaciones[2]  = CrearSuperficieDesdeBitmap("Graficos/Bloques/Interrogacion3.gnb"));
	check(imglstAnimaciones[3]  = CrearSuperficieDesdeBitmap("Graficos/CosasMalas/Calentador.gnb"));
	check(imglstAnimaciones[4]  = CrearSuperficieDesdeBitmap("Graficos/CosasMalas/Calentador2.gnb"));
	check(imglstAnimaciones[5]  = CrearSuperficieDesdeBitmap("Graficos/CosasMalas/Calentador3.gnb"));
	check(imglstAnimaciones[6]  = CrearSuperficieDesdeBitmap("Graficos/CosasMalas/Calentador4.gnb"));
	check(imglstAnimaciones[7]  = CrearSuperficieDesdeBitmap("Graficos/Premios/Candado1.gnb"));
	check(imglstAnimaciones[8]  = CrearSuperficieDesdeBitmap("Graficos/Premios/Candado2.gnb"));
	check(imglstAnimaciones[9]  = CrearSuperficieDesdeBitmap("Graficos/Premios/Candado3.gnb"));
	check(imglstAnimaciones[10] = CrearSuperficieDesdeBitmap("Graficos/CosasMalas/Triturin.gnb"));
	check(imglstAnimaciones[11] = CrearSuperficieDesdeBitmap("Graficos/CosasMalas/Triturin2.gnb"));
	check(imglstAnimaciones[12] = CrearSuperficieDesdeBitmap("Graficos/CosasMalas/Triturin3.gnb"));
	check(imglstAnimaciones[13] = CrearSuperficieDesdeBitmap("Graficos/CosasMalas/Triturin4.gnb"));
	check(imglstAnimaciones[14] = CrearSuperficieDesdeBitmap("Graficos/CosasMalas/Triturin5.gnb"));
	check(imglstAnimaciones[15] = CrearSuperficieDesdeBitmap("Graficos/CosasMalas/Triturin6.gnb"));
	check(imglstAnimaciones[16] = CrearSuperficieDesdeBitmap("Graficos/CosasMalas/Triturin7.gnb"));
	check(imglstAnimaciones[17] = CrearSuperficieDesdeBitmap("Graficos/CosasMalas/Triturin8.gnb"));
	check(imglstAnimaciones[18] = CrearSuperficieDesdeBitmap("Graficos/CosasMalas/Triturin9.gnb"));
	check(imglstAnimaciones[19] = CrearSuperficieDesdeBitmap("Graficos/Premios/Laser1.gnb"));
	check(imglstAnimaciones[20] = CrearSuperficieDesdeBitmap("Graficos/Premios/Laser2.gnb"));
	check(imglstAnimaciones[21] = CrearSuperficieDesdeBitmap("Graficos/Premios/Laser3.gnb"));
	check(imglstAnimaciones[22] = CrearSuperficieDesdeBitmap("Graficos/Premios/Laser4.gnb"));
	check(imglstAnimaciones[23] = CrearSuperficieDesdeBitmap("Graficos/Premios/Laser5.gnb"));
	check(imglstAnimaciones[24] = CrearSuperficieDesdeBitmap("Graficos/Premios/Laser6.gnb"));
	check(imglstAnimaciones[25] = CrearSuperficieDesdeBitmap("Graficos/Premios/Laser7.gnb"));
	check(imglstAnimaciones[26] = CrearSuperficieDesdeBitmap("Graficos/Premios/Laser8.gnb"));
    check(imglstAnimaciones[27] = CrearSuperficieDesdeBitmap("Graficos/Playa/AGUA1.gnb"));
    check(imglstAnimaciones[28] = CrearSuperficieDesdeBitmap("Graficos/Playa/AGUA2.gnb"));
    check(imglstAnimaciones[29] = CrearSuperficieDesdeBitmap("Graficos/Playa/AGUA3.gnb"));
	// imglstFuego
	check(imglstFuego[0]  = CrearSuperficieDesdeBitmap("Graficos/Fuego/BolaPeq.gnb"));
	check(imglstFuego[1]  = CrearSuperficieDesdeBitmap("Graficos/Fuego/BolaPeq2.gnb"));
	check(imglstFuego[2]  = CrearSuperficieDesdeBitmap("Graficos/Fuego/BolaPeq3.gnb"));
	check(imglstFuego[3]  = CrearSuperficieDesdeBitmap("Graficos/Fuego/BolaPeq4.gnb"));
	check(imglstFuego[4]  = CrearSuperficieDesdeBitmap("Graficos/Fuego/BolaPeq5.gnb"));
	check(imglstFuego[5]  = CrearSuperficieDesdeBitmap("Graficos/Fuego/BolaPico.gnb"));
	check(imglstFuego[6]  = CrearSuperficieDesdeBitmap("Graficos/Fuego/BolaPico2.gnb"));
	check(imglstFuego[7]  = CrearSuperficieDesdeBitmap("Graficos/Fuego/BolaPico3.gnb"));
	check(imglstFuego[8]  = CrearSuperficieDesdeBitmap("Graficos/Fuego/BolaPicoR.gnb"));
	check(imglstFuego[9]  = CrearSuperficieDesdeBitmap("Graficos/Fuego/BolaPicoR2.gnb"));
	check(imglstFuego[10] = CrearSuperficieDesdeBitmap("Graficos/Fuego/BolaPicoR3.gnb"));
	//imglstFuego[11] = PUNTERO!;
	//imglstFuego[12] = PUNTERO!;
	check(imglstFuego[13] = CrearSuperficieDesdeBitmap("Graficos/Fuego/Lava.gnb"));
	check(imglstFuego[14] = CrearSuperficieDesdeBitmap("Graficos/Fuego/Lava2.gnb"));
	check(imglstFuego[15] = CrearSuperficieDesdeBitmap("Graficos/Fuego/Lava3.gnb"));
	// imglstRebotador
	check(imglstRebotador[0] = CrearSuperficieDesdeBitmap("Graficos/Rebotadores/Rebotador.gnb"));
	check(imglstRebotador[1] = CrearSuperficieDesdeBitmap("Graficos/Rebotadores/Rebotador2.gnb"));
	check(imglstRebotador[2] = CrearSuperficieDesdeBitmap("Graficos/Rebotadores/Rebotador3.gnb"));
	check(imglstRebotador[3] = CrearSuperficieDesdeBitmap("Graficos/Rebotadores/Rebotador4.gnb"));
	check(imglstRebotador[4] = CrearSuperficieDesdeBitmap("Graficos/Rebotadores/Rebotador5.gnb"));
	check(imglstRebotador[5] = CrearSuperficieDesdeBitmap("Graficos/Rebotadores/Rebotador6.gnb"));
	// imglstRenders
	check(imglstRenders[0] = CrearSuperficieDesdeBitmap("Graficos/Renders/PartVerde.gnb"));
	check(imglstRenders[1] = CrearSuperficieDesdeBitmap("Graficos/Renders/PartVerde2.gnb"));
	check(imglstRenders[2] = CrearSuperficieDesdeBitmap("Graficos/Renders/PartVerde3.gnb"));
	check(imglstRenders[3] = CrearSuperficieDesdeBitmap("Graficos/Renders/PartVerde4.gnb"));
	// imglstGreenRare
	check(imglstGreenRare[0] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/GreenRare.gnb"));
	check(imglstGreenRare[1] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/GreenRare2.gnb"));
	check(imglstGreenRare[2] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/GreenRare3.gnb"));
	check(imglstGreenRare[3] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/GreenRare4.gnb"));
	check(imglstGreenRare[4] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/GreenRare5.gnb"));
	check(imglstGreenRare[5] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/GreenRare6.gnb"));
	check(imglstGreenRare[6] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/GreenRare7.gnb"));
	check(imglstGreenRare[7] = CrearSuperficieDesdeBitmap("Graficos/Enemigos/GreenRare8.gnb"));
	// imglstRobots
	check(imglstRobots[0] = CrearSuperficieDesdeBitmap("Graficos/Jefes/RobotsIz2.gnb"));
	check(imglstRobots[1] = CrearSuperficieDesdeBitmap("Graficos/Jefes/RobotsIz1.gnb"));
	check(imglstRobots[2] = CrearSuperficieDesdeBitmap("Graficos/Jefes/RobotsMed.gnb"));
	check(imglstRobots[3] = CrearSuperficieDesdeBitmap("Graficos/Jefes/RobotsDe1.gnb"));
	check(imglstRobots[4] = CrearSuperficieDesdeBitmap("Graficos/Jefes/RobotsDe2.gnb"));
	// imglstJefes
	check(imglstJefes[0]  = imglstRobots[2]); // PUNTERO!
	check(imglstJefes[1]  = CrearSuperficieDesdeBitmap("Graficos/Jefes/RobotsRev.gnb"));
	// imglstEnergia
	check(imglstEnergia[0] = CrearSuperficieDesdeBitmap("Graficos/Energia/Rayo.gnb"));
	// imglstAparejos
	check(imglstAparejos[0]  = CrearSuperficieDesdeBitmap("Graficos/Aparejos/Telebase.gnb"));
	check(imglstAparejos[1]  = CrearSuperficieDesdeBitmap("Graficos/Aparejos/BotonRojo.gnb"));
	check(imglstAparejos[2]  = CrearSuperficieDesdeBitmap("Graficos/Aparejos/BotonRojoIn.gnb"));
	check(imglstAparejos[3]  = CrearSuperficieDesdeBitmap("Graficos/Aparejos/BotonCyan.gnb"));
	check(imglstAparejos[4]  = CrearSuperficieDesdeBitmap("Graficos/Aparejos/BotonCyanIn.gnb"));
	check(imglstAparejos[5]  = CrearSuperficieDesdeBitmap("Graficos/Aparejos/PuertaRoja.gnb"));
	check(imglstAparejos[6]  = CrearSuperficieDesdeBitmap("Graficos/Aparejos/PuertaCyan.gnb"));
	check(imglstAparejos[7]  = CrearSuperficieDesdeBitmap("Graficos/Aparejos/BotonRojoRev.gnb"));
	check(imglstAparejos[8]  = CrearSuperficieDesdeBitmap("Graficos/Aparejos/BotonRojoInRev.gnb"));
	check(imglstAparejos[9]  = CrearSuperficieDesdeBitmap("Graficos/Aparejos/BotonCyanRev.gnb"));
	check(imglstAparejos[10] = CrearSuperficieDesdeBitmap("Graficos/Aparejos/BotonCyanInRev.gnb"));
	// imglstPlataformas
	check(imglstPlataformas[0] = CrearSuperficieDesdeBitmap("Graficos/Aparejos/Plataforma.gnb"));
	check(imglstPlataformas[1] = CrearSuperficieDesdeBitmap("Graficos/Aparejos/PlataformaCyan.gnb"));
	check(imglstPlataformas[2] = CrearSuperficieDesdeBitmap("Graficos/Aparejos/PlataformaVerde.gnb"));
	// imglstCosasMalas
	check(imglstCosasMalas[0] = CrearSuperficieDesdeBitmap("Graficos/CosasMalas/PinchesPalo.gnb"));
	//imglstCosasMalas[1] = PUNTERO!
	// imglstNicholasAni
    
    check(fuuu = CrearSuperficieDesdeBitmap("Graficos/Nicholas/FuuuNicho.gnb"));
    
    check(CargarNicholas());

    // Concept art
    
    char cad[32] = "Graficos/Art/Art0.gnb";
    
    for(int i=0;i<CONCEPT_ART_SIZE;i++)
    {
        cad[16] = '1'+i;
        check(lpConceptArt[i] = CrearSuperficieDesdeBitmap(cad));
    }

	// Nos vamos...
    return true;
    #undef check
}

bool InicializarSoundBuffers()
{
    #define check(x) if (!(x)) {std::cerr << "Error al cargar el sonido." << std::endl; std::cerr << #x << std::endl; return false;}
    
	check(CargarDirectSound(&lpDSBuffers[0 ],"Sonido/Aplastado.snb"));
	check(CargarDirectSound(&lpDSBuffers[1 ],"Sonido/Bebiendo.snb"));
	check(CargarDirectSound(&lpDSBuffers[2 ],"Sonido/Caida.snb"));
	check(CargarDirectSound(&lpDSBuffers[3 ],"Sonido/ChamelotCorto.snb"));
	check(CargarDirectSound(&lpDSBuffers[4 ],"Sonido/NivelCompleto.snb"));
	lpDSBuffers[5] = NULL;
	check(CargarDirectSound(&lpDSBuffers[6 ],"Sonido/OhOh.snb"));
	check(CargarDirectSound(&lpDSBuffers[7 ],"Sonido/PlocAy.snb"));
	check(CargarDirectSound(&lpDSBuffers[8 ],"Sonido/Plop.snb"));
	check(CargarDirectSound(&lpDSBuffers[9 ],"Sonido/Resorte.snb"));
	check(CargarDirectSound(&lpDSBuffers[10],"Sonido/RupturaBloque.snb"));
	check(CargarDirectSound(&lpDSBuffers[11],"Sonido/VueltaNormal.snb"));
	check(CargarDirectSound(&lpDSBuffers[12],"Sonido/Moneda.snb"));
	check(CargarDirectSound(&lpDSBuffers[13],"Sonido/BolaFuego.snb"));
	check(CargarDirectSound(&lpDSBuffers[14],"Sonido/AuraEscudo.snb"));
	check(CargarDirectSound(&lpDSBuffers[15],"Sonido/Aureola.snb"));
	check(CargarDirectSound(&lpDSBuffers[16],"Sonido/LanzoBolaFuego.snb"));
	check(CargarDirectSound(&lpDSBuffers[17],"Sonido/PuenteCae.snb"));
	check(CargarDirectSound(&lpDSBuffers[18],"Sonido/EsoMeDolio.snb"));
	check(CargarDirectSound(&lpDSBuffers[19],"Sonido/MarchaFunebre.snb"));
	check(CargarDirectSound(&lpDSBuffers[20],"Sonido/Toc.snb"));
	check(CargarDirectSound(&lpDSBuffers[21],"Sonido/Boton.snb"));
	check(CargarDirectSound(&lpDSBuffers[22],"Sonido/ConseguirFull.snb"));
	check(CargarDirectSound(&lpDSBuffers[23],"Sonido/Generador.snb"));
	check(CargarDirectSound(&lpDSBuffers[24],"Sonido/GolpeRobots.snb"));
	check(CargarDirectSound(&lpDSBuffers[25],"Sonido/MetalRebote.snb"));
	check(CargarDirectSound(&lpDSBuffers[26],"Sonido/FinJuego.snb"));
    check(CargarDirectSound(&lpDSBuffers[27],"Sonido/Sufuo.snb"));
    check(CargarDirectSound(&lpDSBuffers[28],"Sonido/PumPum.snb"));
    check(CargarDirectSound(&lpDSBuffers[29],"Sonido/MeAburro.snb"));
    check(CargarDirectSound(&lpDSBuffers[30],"Sonido/Laser.snb"));
    check(CargarDirectSound(&lpDSBuffers[31],"Sonido/GluGluGlu.snb"));
    check(CargarDirectSound(&lpDSBuffers[32],"Sonido/Niam.snb"));
    check(CargarDirectSound(&lpDSBuffers[33],"Sonido/DiscursoCreditos.snb"));
    check(CargarDirectSound(&lpDSBuffers[34],"Sonido/Tiempo.snb"));
    
    check(musicaMenu  = SDL::loadMusic("Sonido/Musica/Pulite.mnb"));
    check(musicaJuego = SDL::loadMusic("Sonido/Musica/Nichobros.mnb"));
    
	// Sufuo, pumpum y meaburro son sonidos en espera.
	return true;	
    #undef check
}

//******** para cargar nuestros archivos de audio ************************************
int CargarDirectSound(LPDIRECTSOUNDBUFFER* buffer, const char* filename)
{
    (*buffer) = SDL::loadChunk(filename);
    return (*buffer) != NULL;
    /*
	HMMIO wavefile; // abrimos nuestro archivo
	wavefile = mmioOpen(filename, 0, MMIO_READ|  MMIO_ALLOCBUF);
	if (wavefile == NULL) return(0);

	MMCKINFO parent; // buscamos los datos del wav
	memset(&parent, 0, sizeof(MMCKINFO));
	parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmioDescend(wavefile, &parent, 0, MMIO_FINDRIFF);

	MMCKINFO child; // buscamos el formato de los datos
	memset(&child, 0, sizeof(MMCKINFO));
	child.fccType = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(wavefile, &child, &parent,0);

	WAVEFORMATEX wavefmt;	// leemos el formato
	mmioRead(wavefile, (char*)&wavefmt, sizeof(wavefmt));
	if(wavefmt.wFormatTag != WAVE_FORMAT_PCM) return(0);

	mmioAscend(wavefile, &child, 0); // buscamos el chunk de datos
	child.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(wavefile, &child, &parent, MMIO_FINDCHUNK);

	DSBUFFERDESC bufdesc;	// creamos un DirectSoundBuffer para contener los datos
	memset(&bufdesc, 0, sizeof(DSBUFFERDESC));
	bufdesc.dwSize = sizeof(DSBUFFERDESC);
	bufdesc.dwFlags = DSBCAPS_GLOBALFOCUS;
	bufdesc.dwBufferBytes = child.cksize;
	bufdesc.lpwfxFormat = &wavefmt;
	if(DS_OK != (lpDS->CreateSoundBuffer(&bufdesc, &(*buffer), NULL))) return(0);

	void *write1 = 0, *write2 = 0; 	// escribimos los datos en el buffer que acabamos de crear
	unsigned long length1, length2;
	(*buffer)->Lock(0, child.cksize, &write1, &length1, &write2, &length2, 0);
	if(write1 > 0)	mmioRead(wavefile, (char*)write1, length1);
	if(write2 > 0) mmioRead(wavefile, (char*)write2, length2);
	(*buffer)->Unlock(write1, length1, write2, length2);

	mmioClose(wavefile, 0); // cerramos el archivo
    return(1);
    */
}

void EscribirEnSuperficie(LPDIRECTDRAWSURFACE lpSup, int x, int y, LPCSTR texto, COLORREF color, HFONT fuente,COLORREF colorsombra)
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

void PresentarNivelBlt()
{
	// Manejamos delta...

	Delta = TiempoDesde(LastFrameTime);
	LastFrameTime = SDL::getTicks();
	if (Delta > MaxDelta)
		Delta = MaxDelta;
	else if (Delta == 0)
		Delta = 1;

	// Corremos el valor de color.
	static double ColorVal = 0, Incre = 1;
	ColorVal += Incre * Delta;
	if (ColorVal >= 255)
	{
		ColorVal = 255;
		Incre    = -VelocidadColorFX;
	}
	if (ColorVal <= 30)
	{
		ColorVal = 30;
		Incre    = VelocidadColorFX;
	}

    // Fondo negro...
	RECT   AuxR;
	AuxR.left   = 0;
	AuxR.top    = 0;
	AuxR.right  = 800;
	AuxR.bottom = 600;
	DrawRectangle(SDL::screen(),&AuxR,RGB(0,0,0));

    char auxNombre[256];

    strcpy(auxNombre,Nombre);

    const char *palabra = strtok(auxNombre," ");

    int printY = 85;
    
    while (palabra)
    {
    
        // Obtenemos el tamaño del texto...

        SIZE AuxSize;

        SDL_Surface *auxSurf = SDL::renderTextSolid(hTitleFont,palabra,RGB(0,0,0));
        AuxSize.cx = auxSurf->w;
        AuxSize.cy = auxSurf->h;
        SDL::freeSurface(auxSurf);

        // Ahora, con amarillo cambiante y sombra roja cambiante, el nombre del nivel.
        EscribirEnSuperficie(SDL::screen(),
                            400 - AuxSize.cx / 2,
                            printY - AuxSize.cy / 2,
                            palabra,RGB(255 - ColorVal,255 - ColorVal,0),hTitleFont,
                            RGB(255,ColorVal,ColorVal));
     
        palabra = strtok(NULL," ");
        printY += AuxSize.cy;
    }
    
	// Por ultimo, el mensaje de pulse tecla en verde, con un fondo gris suave.
	EscribirEnSuperficie(SDL::screen(),150,550,"<Pulsa cualquier tecla para comenzar el nivel>",RGB(10,255,10),hFuente,RGB(100,100,100));
	// Dibujamos!
	//lpDDSPri->Flip(NULL, DDFLIP_WAIT);
    SDL::flip();
}

void PresentarNivel()
{
	SDL_Event msg;
	
	// Esperamos tranquilos la pulsacion de una tecla por parte del usuario.

	LastFrameTime = SDL::getTicks();
	ClearAllKeys();
	bEjecutandose = true;
	while (bEjecutandose)
	{
		while (SDL::pollEvent(msg))
	    {
		    procesarEvento(msg);
		}
	    if (g_active == true)
		{
			bEjecutandose = !AnyKey();
			PresentarNivelBlt();
		}

	}
	// En este caso, es fundamental volver a colocar bEjecutandose en true.
	// Si asi no lo hicieramos, se saldria del juego incorrectamente.
	bEjecutandose = true;
}

bool InicializarJuego()
{
    Pos		= 0;
    Ciclo	= 0;
	InterrN	= 0;
	NivelTerminado = false;
    
    char cad[1000] = "Niveles/";
    
    if (jugarNivelDeUsuario)
    {
        strcat(cad,"Usuario/");
        strcat(cad,nombreNivelDeUsuario);
    }
    else
    {
        strcat(cad,"Nivel");
        char nn[256];
        sprintf(nn,"%d",NumNivel);
        strcat(cad,nn);
    }
    strcat(cad,".NNB");
    std::fstream archivo;
    archivo.open(cad,std::ios::in | std::ios::binary);
    
    if (archivo.fail())
    {
        std::cerr << "Error al abrir el archivo de nivel: " <<  cad << std::endl;
        return false;
    }
    Ancho = 0;
    archivo.read((char *)&Ancho,2);
	strcpy(Nombre,"");
	archivo.get(Nombre,MAX_LONG_NNIVEL,'\0');
	archivo.ignore();
    strcpy(FondoNivel,"Graficos/Fondos/");
	archivo.get(cad,200,'\0');
    archivo.ignore();
	strcat(FondoNivel,cad);
	
    if (lpDDSFondo != NULL)
	{
		// Si existe un fondo, lo matamos.
		SDL::freeSurface(lpDDSFondo);
		lpDDSFondo = NULL;
	}
	// Cargamos en memoria la superficie del fondo...
	if ((lpDDSFondo = CrearSuperficieDesdeBitmap(FondoNivel,false)) == NULL)
    {
        std::cerr << "Error al abrir el archivo de fondo: " <<  FondoNivel << std::endl;
        archivo.close();
        return false;
    }
    
    
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
            archivo.get(Valor);
			if (Valor > 26)
				Valor--;
            Nivel->SetTerreno(X, Y,(eCasillas)Valor);

            archivo.get(Valor);
			if (Valor > 26)
				Valor--;
            Nivel->SetEventos(X, Y,(eEventos)Valor);

			archivo.get(Valor);
			if (Valor > 26)
				Valor--;
			Nivel->SetExtraInfo(X,Y,(eExtraInfo)Valor);
			if (!(((Valor & inDifMas) != 0)?(Dificultad >= Valor % 4) : (Dificultad <= Valor % 4)))
            {
                if (Nivel->Eventos(X, Y) == evPinchePalo ||
                    Nivel->Eventos(X, Y) == evTriturin)
                    Nivel->SetEventos(X,Y,evPisoRigido);
                else
                    Nivel->SetEventos(X,Y,evNulo);
            }
		}
    archivo.close();
	// Inicializamos el array de teclado.
    int Kulo;
    for (Kulo = 0; Kulo <= 255; Kulo++)
        Teclado[Kulo] = false; // De entrada, suponemos.

	// Puntajes.
	PuntajePremio[0] = 50;
	PuntajePremio[1] = 100;
	PuntajePremio[2] = 250;
	PuntajePremio[3] = 500;
	PuntajePremio[4] = 1000;
	PuntajePremio[5] = 2500;
	PuntajePremio[6] = 5000;
	PuntajePremio[7] = 10000;
	PuntajePremio[8] = 50000;

	// Generamos las listas. Si ya existían, primero las eliminamos

    Teleporter::casillasConTeleporters.clear();

	DescargarColecciones();

	Juanchis     = new Coleccion;
	Tortugas	 = new Coleccion;
	Carteles	 = new Coleccion;
	Explosiones  = new Coleccion;
	ColecMonedas = new Coleccion;
	BolasFuego	 = new Coleccion;
	Rebotadores  = new Coleccion;
	BolasPico    = new Coleccion;
	PuentesCaen  = new Coleccion;
	VerdesLocos  = new Coleccion;
	ColecRobots  = new Coleccion;
	Rayos		 = new Coleccion;
	Plataformas  = new Coleccion;
	PinchesPalo  = new Coleccion;
	Triturines   = new Coleccion;
	Caecuchos    = new Coleccion;
	Teleporters  = new Coleccion;
	Puertas      = new Coleccion;
	Sauriolos    = new Coleccion;

	// ¡A crear objetos y a meterlos en las listas!

    ObjetoBase     *ObjetoAuxiliar;

	Juanchi		   *JuanAuxiliar;
	Tortuga		   *TortugaAuxiliar;
	Rebotador	   *RebotadorAuxiliar;
	BolaFuegoSalta *SaltarinaAuxiliar;
	VerdeLoco	   *VerdeLocoAuxiliar;
	Robots		   *RobotsAuxiliar;
	PlatCae		   *PlatCaeAuxiliar;
	PinchePalo	   *PinPaloAuxiliar;
	Triturin	   *TriturinAuxiliar;
	PlatLat        *PlatLatAuxiliar;
	CaeCucho       *CaeCuchoAuxiliar;
	Teleporter     *TeleporterAuxiliar;
	Puerta         *PuertaAuxiliar;
	Sauriolo       *SaurioloAuxiliar;
	Caparatuzo     *CaparatuzoAuxiliar;
	Bichoto        *BichotoAuxiliar;

	int LTPX = 0,LTPY = 0;
	Teleporter *UltimoTele = NULL;

    for (X = 0; X < Ancho; X++)
        for (Y = 0; Y < 20; Y++)
            switch (Nivel->Eventos(X, Y))
			{
                case evInicioNicho:
					if (CandadoX < 0 || CandadoY < 0)
					{
						Nicho = new Nicholas;
	                    Nicho->Inicializar(X, Y);
					}
					break;
                case evEneJuan:
                    ObjetoAuxiliar = new Juanchi();
					JuanAuxiliar = (Juanchi *)ObjetoAuxiliar;
                    JuanAuxiliar->Inicializar(X, Y);
					Juanchis->Agregar(ObjetoAuxiliar);
					break;
                case evEneTortuga:
                    ObjetoAuxiliar  = new Tortuga();
					TortugaAuxiliar = (Tortuga *)ObjetoAuxiliar;
                    TortugaAuxiliar->Inicializar(X, Y);
					Tortugas->Agregar(ObjetoAuxiliar);
					break;
				case evRebotadorSimple:
					ObjetoAuxiliar    = new Rebotador();
					RebotadorAuxiliar = (Rebotador *)ObjetoAuxiliar;
                    RebotadorAuxiliar->Inicializar(X, Y);
					Rebotadores->Agregar(ObjetoAuxiliar);
					break;
				case evBolaSaltarina:
					ObjetoAuxiliar    = new BolaFuegoSalta();
					SaltarinaAuxiliar = (BolaFuegoSalta *)ObjetoAuxiliar;
                    SaltarinaAuxiliar->Inicializar(X, Y,false);
					BolasPico->Agregar(ObjetoAuxiliar);
					break;
				case evEneVerdeLoco:
					ObjetoAuxiliar    = new VerdeLoco();
					VerdeLocoAuxiliar = (VerdeLoco *)ObjetoAuxiliar;
					VerdeLocoAuxiliar->Inicializar(X, Y);
					VerdesLocos->Agregar(ObjetoAuxiliar);
					break;
				case evJefeRobots:
					ObjetoAuxiliar    = new Robots();
					RobotsAuxiliar    = (Robots *)ObjetoAuxiliar;
					RobotsAuxiliar->Inicializar(X, Y);
					ColecRobots->Agregar(ObjetoAuxiliar);
					break;
				case evPlatCae:
					if (X > 0 && Nivel->Eventos(X - 1,Y) != evPlatCae)
					{
						ObjetoAuxiliar  = new PlatCae();
						PlatCaeAuxiliar = (PlatCae *)ObjetoAuxiliar;
						PlatCaeAuxiliar->Inicializar(X,Y);
						Plataformas->Agregar(ObjetoAuxiliar);
					}
					break;
				case evPinchePalo:
					ObjetoAuxiliar  = new PinchePalo();
					PinPaloAuxiliar = (PinchePalo *)ObjetoAuxiliar;
					PinPaloAuxiliar->Inicializar(X,Y);
					PinchesPalo->Agregar(ObjetoAuxiliar);
					break;
				case evTriturin:
					ObjetoAuxiliar  = new Triturin();
					TriturinAuxiliar = (Triturin *)ObjetoAuxiliar;
					TriturinAuxiliar->Inicializar(X,Y);
					Triturines->Agregar(ObjetoAuxiliar);
					break;
				case evPlatLat:
					ObjetoAuxiliar  = new PlatLat();
					PlatLatAuxiliar = (PlatLat *)ObjetoAuxiliar;
					PlatLatAuxiliar->Inicializar(X,Y,true);
					Plataformas->Agregar(ObjetoAuxiliar);
					break;
				case evPlatVert:
					ObjetoAuxiliar  = new PlatLat();
					PlatLatAuxiliar = (PlatLat *)ObjetoAuxiliar;
					PlatLatAuxiliar->Inicializar(X,Y,false);
					Plataformas->Agregar(ObjetoAuxiliar);
					break;
				case evCaeCuchoIz:
					ObjetoAuxiliar  = new CaeCucho();
					CaeCuchoAuxiliar = (CaeCucho *)ObjetoAuxiliar;
					CaeCuchoAuxiliar->Inicializar(X,Y,caecuchoIzquierda);
					Caecuchos->Agregar(ObjetoAuxiliar);
					break;
				case evCaeCuchoDe:
					ObjetoAuxiliar  = new CaeCucho();
					CaeCuchoAuxiliar = (CaeCucho *)ObjetoAuxiliar;
					CaeCuchoAuxiliar->Inicializar(X,Y,caecuchoDerecha);
					Caecuchos->Agregar(ObjetoAuxiliar);
					break;
				case evTeleporter:
					if (X > 0)
					{
						ObjetoAuxiliar  = new Teleporter();
						TeleporterAuxiliar = (Teleporter *)ObjetoAuxiliar;
						TeleporterAuxiliar->Inicializar(X,Y);
						Teleporters->Agregar(ObjetoAuxiliar);
						if (LTPX != 0 && LTPY != 0)
						{
							TeleporterAuxiliar->SetCompa(UltimoTele); // Marcamos los
							UltimoTele->SetCompa(TeleporterAuxiliar); // compañeritos!

							LTPX = 0;
							LTPY = 0;
						}
						else
						{
							LTPX	   = X;
							LTPY	   = Y;
							UltimoTele = TeleporterAuxiliar;
						}
					}
                    else
                        Nivel->SetEventos(X,Y,evPisoRigido);
					break;
				case evPuertaRoja:
					ObjetoAuxiliar = new Puerta();
					PuertaAuxiliar = (Puerta *)ObjetoAuxiliar;
					PuertaAuxiliar->Inicializar(X,Y,true);
					Puertas->Agregar(ObjetoAuxiliar);
					break;
				case evPuertaCyan:
					ObjetoAuxiliar = new Puerta();
					PuertaAuxiliar = (Puerta *)ObjetoAuxiliar;
					PuertaAuxiliar->Inicializar(X,Y,false);
					Puertas->Agregar(ObjetoAuxiliar);
					break;
				case evEneSauriolo:
					ObjetoAuxiliar = new Sauriolo();
					SaurioloAuxiliar = (Sauriolo *)ObjetoAuxiliar;
					SaurioloAuxiliar->Inicializar(X,Y);
					Sauriolos->Agregar(ObjetoAuxiliar);
					break;
				case evEneCaparatuzo:
					ObjetoAuxiliar = new Caparatuzo();
					CaparatuzoAuxiliar = (Caparatuzo *)ObjetoAuxiliar;
                    CaparatuzoAuxiliar->Inicializar(X, Y);
					Juanchis->Agregar(ObjetoAuxiliar);
					// Si: los caparatuzos los mezclamos con los juanchis: muy distintos no son,
					// salvo por los pinches...
					break;
				case evEneBichoto:
					ObjetoAuxiliar = new Bichoto();
					BichotoAuxiliar = (Bichoto *)ObjetoAuxiliar;
                    BichotoAuxiliar->Inicializar(X, Y);
					Juanchis->Agregar(ObjetoAuxiliar);
					// Si: los bichotos los mezclamos con los juanchis: muy distintos no son,
					// salvo por que no se quieren morir...
					break;
                case evPezSaltarin:
					ObjetoAuxiliar    = new BolaFuegoSalta();
					SaltarinaAuxiliar = (BolaFuegoSalta *)ObjetoAuxiliar;
                    SaltarinaAuxiliar->Inicializar(X, Y,true);
					BolasPico->Agregar(ObjetoAuxiliar);
					break;
                default:
                    break;
			}
	if (CandadoX >= 0 && CandadoY >= 0)
	{
		Pos = (CandadoX - 2) * 30;
		Nicho = new Nicholas;
	    Nicho->Inicializar(CandadoX, CandadoY - 1);
	}
	MaxPosicion     = 30 * Ancho - 800;
    Multiplicador   = 1;
	TiempoLV        = 0;
	ReintentarNivel = false;
    EstoyVivo		= true;
    Rebotar			= false;
	APC             = false;
	ATP             = false;
	EBF				= false;
	ERE				= false;
	ColRebotador	= false;
	CerrarCandado   = false;
	ApretadoBotR    = false;
	ApretadoBotC    = false;
	switch (Dificultad)
	{
		case inQuesoRemachado:
			ValorBolas = 1000;
			break;
		case inNewby:
			ValorBolas = 70;
			break;
		case inJuegueroDecente:
			ValorBolas = 50;
			break;
		case inCyberAdicto:
			ValorBolas = 5;
			break;
		default:
			ValorBolas = 50;
			break;
	}

	VidasG		   = Vidas;
	PuntajeG	   = Puntaje;
	MonedasG	   = Monedas;
	CantidadBFG	   = CantidadBF;
	CantidadLaserG = CantidadLaser;

	TiempoJuego     = TiempoDisponible;
	Delta			= 0;
	LastFrameTime	= SDL::getTicks();

    // Mostramos una presentacion de nivel, con otras informaciones
	PresentarNivel();
    
    return true;
}

void AvanzarImagenRobots()
{
	static eDireccion SentidoImagen = Derecha;
	static int NumeroIm = 2;
	if (SentidoImagen == Derecha)
		if (NumeroIm < 4)
			NumeroIm++;
		else
			SentidoImagen = Izquierda;
	else
		if (NumeroIm > 0)
			NumeroIm--;
		else
			SentidoImagen = Derecha;
	imglstJefes[RobotsNormal] = imglstRobots[NumeroIm];
}

bool ConfirmarSalida()
{
	SDL_Event msg;
	bool Salgamos = false;
    
	//RECT AuxR;
	//AuxR.left   = 0;
	//AuxR.top    = 0;
	//AuxR.right  = 800;
	//AuxR.bottom = 600;
	//BltFast(SDL::screen(),0,0,lpDDSPri,&AuxR);
    
	EscribirEnSuperficie(SDL::screen(),100,270,"¿Seguro? S/N",RGB(255,255,0),hTitleFont);
	//lpDDSPri->Flip(NULL,DDFLIP_WAIT);
    SDL::flip();
	while (!Salgamos)
	{
		while (SDL::pollEvent(msg))
	    {
		    procesarEvento(msg);
		}
	    if (g_active == true)
		{
			Salgamos = Teclado[SDLK_s] || Teclado[SDLK_n];
		}
	}
	LastFrameTime = SDL::getTicks();
	return Teclado[SDLK_s];
}

void GetSlots(char *L[])
{
	std::ifstream F;
	char Buffer[100];
	char Cad[100] = "Saves/game0.jnb";
	int i;
	for (i = 0;i < 10;i++)
	{
		Cad[10] = '0' + i;
		F.open(Cad,std::ios::binary);
		F.getline(Buffer,100,0);
		if (strlen(Buffer) == 0)
			strcpy(Buffer,"<VACIO>");
		L[i] = new char[strlen(Buffer) + 1];
		strcpy(L[i],Buffer);
		F.close();
	}
    L[10] = new char[64];
    strcpy(L[10],"Cancelar");
}

void FreeSlots(char *L[])
{
	int i;
	for (i = 0;i < 11;i++)
		delete [] L[i];
}

void PrintSlots(char *L[])
{
	RECT R;
	int i;
	SetRect(&R,200,20,750,420);
	DrawRectangle(SDL::screen(),&R,RGB(0,0,0));
	for (i = 0;i < 10;i++)
		EscribirEnSuperficie(SDL::screen(),250,70 + 30 * i,L[i],RGB(255,50,50),hCreditsFont,RGB(100,80,40));
	EscribirEnSuperficie(SDL::screen(),250,70 + 30 * 10,"Cancelar",RGB(255,50,50),hCreditsFont,RGB(100,80,40));
	EscribirEnSuperficie(SDL::screen(),250,25,"Menu de grabacion",RGB(225,221,100),hCreditsFont,RGB(15,40,250));
}

void Focus(int n)
{
	RECT R;
	SetRect(&R,200,50,250,420);
	DrawRectangle(SDL::screen(),&R,RGB(0,0,0));
	BufferPaint(225,78 + 30 * n, lpDDSFlecha,15,15);
}

#define Meter(Variable) F.write((char *)&Variable,sizeof(Variable))

void Save2File(int n, char *S)
{
	std::ofstream F;
	char Cadena[100] = "Saves/game0";
	Cadena[strlen(Cadena)-1] = '0' + n;
	strcat(Cadena,".jnb");
	F.open(Cadena,std::ios::out | std::ios::binary);
    if (F.fail())
    {
        std::cerr << "Error al grabar en el archivo: " << Cadena << std::endl;
        return;
    }
	F.write(S,strlen(S) + 1);

	Meter(NumNivel);

	Meter(Dificultad);

	Meter(VidasG);

	Meter(PuntajeG);

	Meter(MonedasG);

	Meter(CantidadBFG);
	Meter(CantidadLaserG);

	Meter(CandadoX);
	Meter(CandadoY);

	F.close();
}

#define Sacar(Variable) F.read((char *)&Variable, sizeof(Variable))

void LoadFromFile(int n)
{
	std::ifstream F;
	char Cadena[100] = "Saves/game0";
	Cadena[strlen(Cadena)-1] = '0' + n;
	strcat(Cadena,".jnb");
	F.open(Cadena,std::ios::binary);
    if (F.fail())
    {
        std::cerr << "Error al abrir para cargar el archivo: " << Cadena << std::endl;
        return;
    }
	F.getline(Cadena,100,0);

	Sacar(NumNivel);

	Sacar(Dificultad);

    char AuxCad[MaxLongOpcion] = "Dificultad ";
	strcat(AuxCad,NomDif[Dificultad]);
	strcpy(NOOpcionMenu[1],AuxCad);

	Sacar(Vidas);

	Sacar(Puntaje);

	Sacar(Monedas);

	Sacar(CantidadBF);
	Sacar(CantidadLaser);

	Sacar(CandadoX);
	Sacar(CandadoY);

	F.close();
}

bool Cargar()
{
	int OpcionActiva = 0;
	char* L[11];
	SDL_Event msg;
	bool Salgamos;
    
	//RECT AuxR;
	//AuxR.left   = 0;
	//AuxR.top    = 0;
	//AuxR.right  = 800;
	//AuxR.bottom = 600;
	//lpDDSSec->BltFast(0,0,lpDDSPri,&AuxR,DDBLTFAST_WAIT);
    
	GetSlots(L);
	PrintSlots(L);
	Focus(OpcionActiva);
	//lpDDSPri->Flip(NULL,DDFLIP_WAIT);
    SDL::flip();
	//lpDDSSec->BltFast(0,0,lpDDSPri,&AuxR,DDBLTFAST_WAIT);
    Salgamos = false;
	while (!Salgamos)
	{
		if (SDL::pollEvent(msg))
	    {
		    procesarEvento(msg);
		}
	    else if (g_active == true)
		{
			if (Teclado[SDLK_UP])
			{
				Teclado[SDLK_UP] = false;
				OpcionActiva--;
				PlayWav(sndToc);
			}
			if (Teclado[SDLK_DOWN])
			{
				Teclado[SDLK_DOWN] = false;
				OpcionActiva++;
				PlayWav(sndToc);
			}
			if (OpcionActiva > 10)
				OpcionActiva = 0;
			if (OpcionActiva < 0)
				OpcionActiva = 10;
			Focus(OpcionActiva);
			//lpDDSPri->Flip(NULL,DDFLIP_WAIT);
            SDL::flip();
			Salgamos = Teclado[SDLK_RETURN] && strcmp(L[OpcionActiva],"<VACIO>") != 0;
		}
	}
	if (OpcionActiva != 10)
		LoadFromFile(OpcionActiva);
	FreeSlots(L);
	LastFrameTime = SDL::getTicks();
    
    Teclado[SDLK_RETURN] = false;
    
	return OpcionActiva != 10;
}

void Grabar()
{
	int OpcionActiva = 0;
	char* L[11];
	SDL_Event msg;
	bool Salgamos;
    
	//RECT AuxR;
	//AuxR.left   = 0;
	//AuxR.top    = 0;
	//AuxR.right  = 800;
	//AuxR.bottom = 600;
	do
	{
        Salgamos = false;
        //lpDDSSec->BltFast(0,0,lpDDSPri,&AuxR,DDBLTFAST_WAIT);
        GetSlots(L);
        PrintSlots(L);
        Focus(OpcionActiva);
        //lpDDSPri->Flip(NULL,DDFLIP_WAIT);
        SDL::flip();
        //lpDDSSec->BltFast(0,0,lpDDSPri,&AuxR,DDBLTFAST_WAIT);
        while (!Salgamos)
        {
            if (SDL::pollEvent(msg))
            {
                procesarEvento(msg);
            }
            else if (g_active == true)
            {
                if (Teclado[SDLK_UP])
                {
                    Teclado[SDLK_UP] = false;
                    OpcionActiva--;
                    PlayWav(sndToc);
                }
                if (Teclado[SDLK_DOWN])
                {
                    Teclado[SDLK_DOWN] = false;
                    OpcionActiva++;
                    PlayWav(sndToc);
                }
                if (OpcionActiva > 10)
                    OpcionActiva = 0;
                if (OpcionActiva < 0)
                    OpcionActiva = 10;
                Focus(OpcionActiva);
                //lpDDSPri->Flip(NULL,DDFLIP_WAIT);
                SDL::flip();
                Salgamos = Teclado[SDLK_RETURN] || Teclado[SDLK_ESCAPE];
            }
        }
        if (Teclado[SDLK_ESCAPE]) break;
        if (OpcionActiva != 10)
        {
            bool SePuede = false;
            Salgamos = false;
            char Lectura[21] = "";
            while (!Salgamos)
            {
                if (SDL::pollEvent(msg))
                {
                    procesarEvento(msg);
                }
                else if (g_active == true)
                {
                    if (!Teclado[SDLK_RETURN])
                        SePuede = true;
                    Focus(OpcionActiva);

                    RECT R;
                    SetRect(&R,250,75 + 30 * OpcionActiva,750,105 + 30 * OpcionActiva);
                    DrawRectangle(SDL::screen(),&R,RGB(0,0,0));
                    int i;
                    if (strlen(Lectura) < 20)
                    {
                        for (i = 'a';i <= 'z';i++)
                            if (Teclado[i])
                            {
                                Lectura[strlen(Lectura) + 1] = 0;
                                Lectura[strlen(Lectura)] = toupper(i);
                                Teclado[i] = false;
                            }
                        for (i = '0';i <= '9';i++)
                            if (Teclado[i])
                            {
                                Lectura[strlen(Lectura) + 1] = 0;
                                Lectura[strlen(Lectura)] = i;
                                Teclado[i] = false;
                            }
                        if (Teclado[SDLK_SPACE])
                        {
                            Lectura[strlen(Lectura) + 1] = 0;
                            Lectura[strlen(Lectura)] = ' ';
                            Teclado[SDLK_SPACE] = false;
                        }
                    }
                    if (Teclado[SDLK_BACKSPACE] && strlen(Lectura) > 0)
                    {
                        Lectura[strlen(Lectura) - 1] = 0;
                        Teclado[SDLK_BACKSPACE] = false;
                    }
                    strcat(Lectura,"*");
                    EscribirEnSuperficie(SDL::screen(),250,70 + 30 * OpcionActiva,Lectura,RGB(255,50,50),hCreditsFont,RGB(100,80,40));
                    Lectura[strlen(Lectura) - 1] = 0;
                    
                    //lpDDSPri->Flip(NULL,DDFLIP_WAIT);
                    SDL::flip();
                    Salgamos = (Teclado[SDLK_RETURN] || Teclado[SDLK_ESCAPE]) && SePuede;
                }
            }
            if (!Teclado[SDLK_ESCAPE])
                Save2File(OpcionActiva,Lectura);
        }
	} while (Teclado[SDLK_ESCAPE]);
    Teclado[SDLK_ESCAPE] = false;
	FreeSlots(L);
	LastFrameTime = SDL::getTicks();
}

void ActualizarJuego()
{
	// Seteamos delta como el tiempo transcurrido desde el último
	// frame. Esta técnica permite independizar la velocidad del
	// juego de la pc en la cual se ejecuta.
	// Si delta sobrepasa el máximo establecido en MaxDelta, 
	// entonces consideramos que es mejor ralentizar el juego
	// que generar un salto tan brusco, generando una excepcion a
	// la idea anterior.
	Delta = (long)(TiempoDesde(LastFrameTime) * VelocidadJuego);
	if (Delta > MaxDelta)
		Delta = MaxDelta;
	else if (Delta == 0)
		Delta = 1;
	LastFrameTime = SDL::getTicks();
	long AuxDelta = Delta;
  while (AuxDelta > 0)
  {
	if (AuxDelta > MaxDeltaProc)
	{
		AuxDelta -= MaxDeltaProc;
		Delta    = MaxDeltaProc;
	}
	else
	{
		Delta = AuxDelta;
		AuxDelta = -1;
	}
  	// Manejamos los Interr, para lograr bloques animados.
	static int CambiandoInterr = Ciclo;
	if (Ciclo != CambiandoInterr)
	{
			CambiandoInterr = Ciclo;
			InterrN++;
	}
    if (InterrN >= 20160) // 20160 = 64(2->6) * 9(3->2) * 5 * 7
       InterrN = 0;
	imglstTerreno   [caBloqueInterr - 1] = imglstAnimaciones[     (InterrN / 8) % 3 ];
	imglstTerreno   [caFuegoLava    - 1] = imglstFuego      [13 + (InterrN / 8) % 3 ];
    imglstTerreno   [caAgua         - 1] = imglstAnimaciones[27 + (InterrN / 8) % 3 ];
	imglstTerreno   [caCalentador   - 1] = imglstAnimaciones[3  + (InterrN / 8) % 4 ];
	imglstPremios   [prBolaFuego       ] = imglstFuego		[     (InterrN / 8) % 5 ];
	imglstPremios   [prLaser	       ] = imglstAnimaciones[19 + (InterrN / 4) % 8 ];
	imglstParticulas[partVerde		   ] = imglstRenders    [     (InterrN / 8) % 4 ];
	imglstFuego		[fuegArribaPico    ] = imglstFuego		[5  + (InterrN / 8) % 3 ];
	imglstFuego		[fuegAbajoPico     ] = imglstFuego		[8  + (InterrN / 8) % 3 ];
	imglstCosasMalas[cmTriturin		   ] = imglstAnimaciones[10 + (InterrN / 4) % 9 ];
	static long TTT = 0;
	TTT += Delta;
	if (!CerrarCandado)
	{
		imglstPremios[prCandado] = imglstAnimaciones[(ImagenCandado = 7)];
		TTT = 0;
	}
	else if (ImagenCandado == 9)
	{
		if (CandadoX < Pos / 30)
			CerrarCandado = false;
	}
	else if (TTT > 350)
	{
		ImagenCandado++;
		imglstPremios[prCandado] = imglstAnimaciones[ImagenCandado];
		TTT = 0;
	}

	if (!Nicho->Teleportando)
	{
		int AuxSuperAumento, ExpInterr;
		AuxSuperAumento = (Nicho->Super())?(50):(0);
		ExpInterr		= (Teclado[SDLK_w])?(InterrN):(InterrN / 2);
		imglstNicholas  [NicholasCaDe] = imglstNicholasAni[2  + ExpInterr % 14 + AuxSuperAumento];
		imglstNicholas  [NicholasCaIz] = imglstNicholasAni[16 + ExpInterr % 14 + AuxSuperAumento];

		if (!CambiaTrepa)
			ExpInterr = 0;
		imglstNicholas  [NicholasTreDe] = imglstNicholasAni[32 + ExpInterr % 6  + AuxSuperAumento];
		imglstNicholas  [NicholasTreIz] = imglstNicholasAni[38 + ExpInterr % 6  + AuxSuperAumento];
	}

	static bool Ahora = true;
	if (Ciclo % 2 == 0)
	{
		if (Ahora)
		{
			AvanzarImagenRobots();
			Ahora = false;
		}
	}
	else
		Ahora = true;
    // Movemos la variable ciclo, usada en las animaciones.
	static int ReguladorDeCiclo = 0;
	Ciclo = (ReguladorDeCiclo / DuracionCiclo) % 4;
	ReguladorDeCiclo += Delta;
	// Acortamos el tiempo.
	TiempoJuego -= Delta;
	if (TiempoJuego < 0)
		TiempoJuego = 0;
	// Verificamos el tiempo de liquido verde.
	TiempoLV -= Delta;
	if (TiempoLV < 0)
		TiempoLV = 0;
	// Verificamos salida por escape.
    if (Teclado[SDLK_ESCAPE])
	{
		PlayWav(sndBoton);
		if (ConfirmarSalida())
		{
			StopAllWavs();
			EliminarTodo();
			FinNivel      = true;
			bEjecutandose = false;
			return;
		}
	}
	if (Teclado[SDLK_g] && !jugarNivelDeUsuario)
	{
		PlayWav(sndBoton);
		Grabar();
	}

    

	// Mandamos a calcular a todos los objetos.
	
	ChequearTodo();

	// Si juntamos suficientes monedas...¡Ganamos una vida!

	while (Monedas >= MonedasParaVida)
	{
		Monedas -= MonedasParaVida;
		Vidas++;
	}
	// Oh! Los cálculos aportados por nicholas informan que hemos
	// muerto! Por suerte tenemos más vidas... ¿ O no? Vamos a
	// manejar esto.

	if (ReintentarNivel || TiempoJuego <= 0)
	{
		StopAllWavs();
        if (TiempoJuego <= 0)
            PlayWav(sndTiempo);
		FinNivel = true;
        if (Vidas > 0)
            Vidas--;
        else
		{
			GameOver	  = true;
            bEjecutandose = false;
			PlayWav(sndFinJuego);
		}
        return;
    }

	// Wow! Lluegamos al final del nivel.

    if (NivelTerminado)
	{
		StopAllWavs();
		PlayWav(sndNivelCompleto);
		long iTime;
		iTime = SDL::getTicks();
		while (TiempoDesde(iTime) < 4250);
		FinNivel = true;
		// Ahora salimos, pero tendría que seguir en el next level
        bEjecutandose = false;
        return;
	}
  }
}

void Blt()
{ 
	// Avanzamos la cuenta de cuadros por segundo
#ifdef DEPURANDO
	if (TiempoDesde(FPS_tUltimo) >= 1000)
	{ 
		FPS_Actual = FPS_Suma;
		FPS_Suma = 0;
		FPS_tUltimo = SDL::getTicks();
	} 
	FPS_Suma++;
#endif // DEPURANDO

	// Dibujamos fondo y tiles.
	Redibujar();
	
	// Dibujamos a Nicholas.
	Nicho->Mostrar();
	
	// Dibujamos los objetos en lista!
	MostrarTodo();
	
	// Dibujamos tiles que tapan a los demás objetos.
	RedibujarTT();

	// Escribimos información del juego.
	EscribirDatos();

#ifdef DEPURANDO
	//Escribo Cuadros por segundo

    char Mensaje[50], FPSchar[25];
	ltoa(FPS_Actual,FPSchar);
	strcpy(Mensaje, "Cuadros por Segundo = ");
    strcat(Mensaje, FPSchar);
	EscribirEnSuperficie(SDL::screen(), 0, 0, LPCSTR(Mensaje), RGB(250,200,100), hFuente);

#endif // DEPURANDO

	//Intercambio las superficies Primaria y Secundaria
    //lpDDSPri->Flip(NULL, DDFLIP_WAIT);
    SDL::flip();
}

/*
bool CrearVentana(HINSTANCE hInstancia)
{
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hInstance = hInstancia;
    wc.lpfnWndProc = wndProc;
    wc.lpszClassName = g_appName;
    wc.cbSize = sizeof(wc);
    wc.style = CS_VREDRAW | CS_HREDRAW;
    if (RegisterClassEx(&wc) == 0) return false;

    g_hwnd = CreateWindowEx(WS_EX_TOPMOST, g_appName, g_appName, WS_POPUP | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT , CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstancia, 0);
    if (g_hwnd == NULL) return false;
    SetFocus(g_hwnd);
    ShowWindow(g_hwnd, SW_SHOWNORMAL);
    UpdateWindow(g_hwnd);
	
	ShowCursor(false);

    return true;
}
*/

int main(int argc, char *args[])
{
    {
        std::ifstream inpuVolSonido("volSonido");
        if (!inpuVolSonido.fail())
        {
            inpuVolSonido >> haySonido;
            inpuVolSonido >> hayMusica;
            inpuVolSonido >> volumenSonido;
            inpuVolSonido >> volumenMusica;
            inpuVolSonido.close();
            sprintf(NOOpcionSonidoMenu[1],"Volumen Sonido %d%%",volumenSonido);
            sprintf(NOOpcionSonidoMenu[3],"Volumen Música %d%%",volumenMusica);
            if (haySonido)
                sprintf(NOOpcionSonidoMenu[0],"Sonido On");
            else
                sprintf(NOOpcionSonidoMenu[0],"Sonido Off");
            if (hayMusica)
                sprintf(NOOpcionSonidoMenu[2],"Musica On");
            else
                sprintf(NOOpcionSonidoMenu[2],"Musica Off");
        }
    }
    
    g_active = true;
    
    #define zMem(c) memset((c),0,sizeof(c))
    zMem(Teclado);
    zMem(imglstTerreno);
    zMem(imglstNicholas);
    zMem(imglstPremios);
    zMem(imglstEnemigos);
    zMem(imglstExplosiones);
    zMem(imglstAnimaciones);
    zMem(imglstFuego);
    zMem(imglstRebotador);
    zMem(imglstRenders);
    zMem(imglstParticulas);
    zMem(imglstNicholasAni);
    zMem(imglstGreenRare);
    zMem(imglstRobots);
    zMem(imglstJefes);
    zMem(imglstEnergia);
    zMem(imglstPlataformas);
    zMem(imglstCosasMalas);
    zMem(imglstAparejos);
    zMem(lpConceptArt);

    int NumeroNiveles = 41;

    const int NIVEL_MACHETE = 28;

    //Inicio SDL
    if(!InicializarSDL())
    {
        std::cerr << "Fallo en la inicializacion de la SDL." << std::endl;
        FinalizarSDL();
        return -1;
    }

    Mix_Volume(-1, (volumenSonido * MIX_MAX_VOLUME) / 100);
    Mix_VolumeMusic((volumenMusica * MIX_MAX_VOLUME) / 100);


    // Inicio Superficies
    if(!InicializarSuperficies())
    {
        std::cerr << "Fallo en la inicializacion de las superficies." << std::endl;
        FinalizarSDL();
        return -1;
    }

	// Inicio Sound Buffers.
	if(!InicializarSoundBuffers())
	{
        std::cerr << "Fallo al inicializar los buffers de sonido." << std::endl;
		FinalizarSDL();
		return -1;
	}

	//DoAcercaDe();

    while (Menu()) // Cuando menu sea false, salimos del juego.
        if (MostrarCreditos)
            DoCreditos();
        else if (MostrarAcercaDe)
            DoAcercaDe();
        else if (MostrarAyuda)
            DoAyuda();
        else if (MostrarGaleria)
            DoGaleria();
        else if (jugarNivelDeUsuario)
        {
            Jugar();
            if (GameOver)
                DoGameOver();
        }
        else // La posta! Modo historia!
        {
            while (NumNivel <= NumeroNiveles && Jugar())
            {
                if (NumNivel == NIVEL_MACHETE)
                    DoMachete();
                NumNivel++;
                if (NumNivel > NumeroNiveles)
                {
                    Felicitar();
                    DoCreditos();
                }
            }
            if (GameOver)
                DoGameOver();
        }

	//DoAcercaDe();

    FinalizarSDL();
    
    {
        std::ofstream oupuVolSonido("volSonido");
        oupuVolSonido << haySonido << std::endl;
        oupuVolSonido << hayMusica << std::endl;
        oupuVolSonido << volumenSonido << std::endl;
        oupuVolSonido << volumenMusica << std::endl;
        oupuVolSonido.close();
    }
    return 0;
    
}
