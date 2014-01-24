// Declaramos unas cuantas de direct draw!

// Evitamos doble inclusión.

#ifndef INC_DDINC

#define INC_DDINC

typedef SDL_Surface* LPDIRECTDRAWSURFACE;
typedef TTF_Font * HFONT;
typedef Mix_Chunk * LPDIRECTSOUNDBUFFER;
typedef int HRESULT;
typedef const char * LPCSTR;
typedef SDL_Color COLORREF;

struct RECT
{
    int top, left, right, bottom;
};

inline
void SetRect(RECT *r,int left,int top,int right,int bottom)
{
    r->left = left;
    r->right = right;
    r->top = top;
    r->bottom = bottom;
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

struct SIZE {int cx,cy;};

// Incluimos librerías necesarias.

#include "InfoObj.h"

// Variables globales de directdraw.
extern bool bEjecutandose;
extern HFONT hFuente;
extern HFONT hTitleFont;

// Variables globales de directsound
// Buffers con todos los sonidos...
extern LPDIRECTSOUNDBUFFER lpDSBuffers[TAMANO_SONIDO];

//Prototipos de funciones relacionadas con DirectDraw
bool             InicializarSuperficies();
void				EscribirEnSuperficie(LPDIRECTDRAWSURFACE,int,int,LPCSTR,COLORREF,HFONT,COLORREF = (COLORREF){10,10,10});
void				ActualizarJuego();
bool				InicializarJuego();
void                Blt();

// Funciones de DS.
bool InicializarSoundBuffers();
int     CargarDirectSound(LPDIRECTSOUNDBUFFER* buffer, const char* filename);

inline
void	StopAllWavs()
{
	//int Aux;
	//for (Aux = 0; Aux < TAMANO_SONIDO; Aux++)
	//	lpDSBuffers[Aux]->Stop();
    SDL::stopAllChannels();
}

inline
COLORREF i2col(int x)
{
    COLORREF res;
    res.b = x & 0x01F;
    res.g = (x & 0X07E0) >> 5;
    res.r = (x & 0XF800) >> 11;
    return res;
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

extern bool haySonido;

inline 
void    PlayWav(eSonido wavNum)
{
	//DWORD k;
	//lpDSBuffers[wavNum]->GetStatus(&k);
	//if (!(k & DSBSTATUS_PLAYING))
	//{
	//	lpDSBuffers[wavNum]->SetCurrentPosition(0);
	//	lpDSBuffers[wavNum]->Play(0,0,0);
	//}
    if (haySonido) SDL::playChunk(lpDSBuffers[wavNum],0);
}


inline
void BltFast(SDL_Surface *destination, int x,int y, SDL_Surface * source, RECT *recti)
{
    SDL_Rect rec = r2sdl(*recti);
    SDL::blit(x,y,source,destination,&rec);
}

inline
bool IsPlaying(Mix_Chunk *sound)
{
    for(int i=0;i<MIX_CHANNELS;i++)
    if (Mix_Playing(i) && sound == Mix_GetChunk(i))
        return true;
    return false;
}

inline
void SoundStop(Mix_Chunk *sound)
{
    for(int i=0;i<MIX_CHANNELS;i++)
    if (Mix_Playing(i) && sound == Mix_GetChunk(i))
        Mix_HaltChannel(i);
}

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

// Un par de funciones auxiliares del programa.

void DescargarSuperficie(LPDIRECTDRAWSURFACE &S);
void QuemarLinea  (LPDIRECTDRAWSURFACE Surf,int Ancho,int N);
void WhiteLine    (LPDIRECTDRAWSURFACE S1,LPDIRECTDRAWSURFACE S2,int Ancho,int N);
void WhiteExtremes(LPDIRECTDRAWSURFACE S1,LPDIRECTDRAWSURFACE S2,int Ancho,int N);
void WhitePaint(long X , long Y , LPDIRECTDRAWSURFACE Surf,int Ancho, int Alto);
eTiposCol ColisionaNicholas(RECT &OR, LPDIRECTDRAWSURFACE lpSprite,int surStartH = 0);
bool      ColisionGeneral  (RECT &R1,RECT &R2,LPDIRECTDRAWSURFACE S1,LPDIRECTDRAWSURFACE S2,int start1=0, int start2=0);
bool	  ColNicholasCasilla(int X, int Y);
void BufferPaint(long X , long Y , LPDIRECTDRAWSURFACE Surf,int Ancho, int Alto,int X0 = 0,int Y0 = 0);

#endif // INC_DDINC
