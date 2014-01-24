#ifndef SDL_CLASS_H
#define SDL_CLASS_H

// Clase para juntar toda la mierda SDL related que sea reutilizable.

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

// Tipos de la SDL:

// SDL_Surface	-> texturas, imagenes 2D
// SDL_Event	-> descripcion de un evento (clic, movimiento del mouse, teclado, etc)
// TTF_Font		-> fuente de texto true type
// SDL_Rect		-> rectangulo. Sus miembros son x,y,w,h
// SDL_Color	-> color de la SDL. Sus miembros son r,g,b.

namespace SDL
{
	// ----------------  SDL Grafica  -----------------------
	
	// Retorna true si todo salio bien.
	bool init(int width, int height, int depth,bool fullScreen, const char *caption);
	int screenWidth();
	int screenHeight();
	int screenDepth();
	const char * windowCaption();
	SDL_Surface * screen();
	// Si se especifica color key, se le da el color RGB del color a usar como transparente.
	SDL_Surface *loadImage(const char *filename, bool colorKey = false, int red = 0,int green = 0, int blue = 0);
	void fillRect(SDL_Surface *destino, const SDL_Color color, SDL_Rect *rect);
	void fillSurface(SDL_Surface *destino, const SDL_Color color);
	// blitea el rectangulo srcRect de source a destination, lo pone en (x,y) en destination.
	// Un retangulo NULL (default) blitea todo source.
	void blit( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect * srcRect = NULL);
	// Muestra el proximo frame.
	void flip();
	void freeSurface(SDL_Surface *);
	void quit();
	
	// ----------------  Eventos, input-output  -----------------------
	
	// Devuelve true si se extrajo un evento de la cola.
	bool pollEvent(SDL_Event &);
	// Obtiene los keystates. Luego de x = getKeyState(&n), x[K] evalua a true si se encuentra
	// presionada la tecla K.
	// Se pasa por referencia (puntero) un entero que recibe la cantidad de keys disponibles.
	// Si se pasa NULL (default) la funcion simplemente no aporta tal informacion.
	Uint8 *getKeyState(int * numKeys = NULL); 
	
	// ----------------  SDL Mixer  -----------------------

	// Devuelve true si todo salio bien.
	// Wrapper de la funcion correspondiente de la SDL : Mirar documentacion de esta con cuidado.
	// Uso tipico:
    // El 1024 ese es flexible:
    // Valores mas grandes generan mas lag
    // Valores mas chicos pueden generar buffer underruns en maquinas lentas (suena feo)
    // Lo ideal depende de la target machine.
	// openAudio( 22050, MIX_DEFAULT_FORMAT, true, 1024)
	// openAudio( 44100, MIX_DEFAULT_FORMAT, true, 1024)
    // Si falla con esos parametros, empieza a probar con menos channels hasta
    // que logra abrir, si no puede ni con un channel devuelve false.
	bool openAudio(int freq, Uint16 format, bool stereo, int chunksize);
    void closeAudio();
	
	// Cargan musica y sonido. Ver documentacion de la SDL para saber los formatos soportados.
	
	Mix_Music * loadMusic(const char *filename);
	Mix_Chunk * loadChunk(const char *filename);
	
	// Liberan musica y sonido.
	
	void freeMusic(Mix_Music *);
	void freeChunk(Mix_Chunk *);
	
	// Los siguientes reproducen, pausan, etc
	
	// Devuelve true si todo sale bien.
	// loops es la cantidad de veces que se repite.
	// 0 para que suene una sola vez, 1 para repetir 2 veces, etc.
	// -1 para repetir indefinidamente.
	bool playChunk(Mix_Chunk *chunk, int loops);

	bool playingMusic();
	bool pausedMusic();
	
	// Devuelve true si salio todo bien.
	bool playMusic(Mix_Music *music, int loops);	// loops es la cantidad de veces : 0 no reproduce, 1 reproduce una vez, etc.
													// -1 para infinitas veces.
	void resumeMusic();
	void pauseMusic(); // Note: Music can only be paused if it is actively playing.
	void haltMusic();
	
	void stopSound();
	
	// Combinacion de stopSound y haltMusic
	// Detiene la reproduccion en todos los canales (incluyendo la musica).
	// Liberar un sonido/musica que se estaba reproduciendo es MUY mala idea.
	// Por lo tanto es muy buena idea correr esto antes de liberar cosas.
	void stopAllChannels();
	
	// ----------------  SDL Timer  -----------------------
	
	int getTicks();
	
	// ----------------  True Type Fonts  -----------------------
	
	bool initTTF();
	TTF_Font *openFont(const char *fileName, int size);
	// Devuelve una superficie nueva creada por la funcion, que contiene el texto en cuestion.
	// La superficie en cuestion tiene fondo transparente (el color especificado es el color del texto).
	SDL_Surface *renderTextSolid(TTF_Font *font, const char *text, SDL_Color color);
	void closeFont(TTF_Font *);
	void quitTTF();
}
	
#endif // SDL_CLASS_H
