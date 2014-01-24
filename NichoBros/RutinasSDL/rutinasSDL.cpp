#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "rutinasSDL.h"

extern bool hayMusica;

namespace SDL
{
	int scrWidth;
	int scrHeight;
	int scrDepth; 
	const char *wndCaption;
	SDL_Surface *scr;

	bool init(int width, int height, int depth,bool fullScreen, const char *caption)
	{
		scrWidth   = width;
		scrHeight  = height;
		scrDepth   = depth;
		wndCaption = caption;
		//Initialize all SDL subsystems 
		if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) { return false; } 
		//Set up the screen
		const unsigned int FLAGS = SDL_SWSURFACE | (fullScreen?SDL_FULLSCREEN:0);
		scr = SDL_SetVideoMode( scrWidth, scrHeight, scrDepth, FLAGS ); 
		//If there was an error in setting up the screen
		if( scr == NULL ) { quit(); return false; } 
		//Set the window caption
		SDL_WM_SetCaption( wndCaption, NULL ); 
		//If everything initialized fine 
		return true; 
	} 

	int screenWidth() {return scrWidth; }
	int screenHeight() {return scrHeight;}
	int screenDepth() {return scrDepth;}
	
	const char * windowCaption() {return wndCaption;}
	
	SDL_Surface * screen() {return scr;}
	
	SDL_Surface *loadImage(const char *filename, bool colorKey, int red,int green, int blue)
	{
		//Temporary storage for the image that's loaded
		SDL_Surface* loadedImage = NULL;
		//The optimized image that will be used
		SDL_Surface* optimizedImage = NULL; 

		//Load the image using SDL_image
		loadedImage = IMG_Load(filename); 
		
		//If nothing went wrong in loading the image
		if( loadedImage != NULL ) 
		{
			//Create an optimized image 
			optimizedImage = SDL_DisplayFormat( loadedImage ); 
			//Free the old image
			SDL_FreeSurface( loadedImage ); 
			if (colorKey && optimizedImage != NULL)
			{
				//Map the color key
				Uint32 ck = SDL_MapRGB( optimizedImage->format, red, green, blue); 
				//Set all pixels of color ck to be transparent
				SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, ck );
			}
		}
		
		//Return the optimized image 
		return optimizedImage; 
	}

	//Fill the screen white 
	

	void fillRect(SDL_Surface *destino, const SDL_Color color, SDL_Rect *rect)
	{
		SDL_FillRect(destino, rect , SDL_MapRGB(destino->format, color.r, color.g, color.b)); 
	}
	
	void fillSurface(SDL_Surface *destino, const SDL_Color color)
	{
		fillRect(destino,color, &destino->clip_rect);
	}

	void blit( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect * srcRect)
	{ 
		//Make a temporary rectangle to hold the offsets 
		SDL_Rect offset; 
		//Give the offsets to the rectangle 
		offset.x = x; offset.y = y;
		//Blit the surface
		SDL_BlitSurface( source, srcRect, destination, &offset ); 
	} 

	void flip()
	{
		SDL_Flip(scr);
	}

	void freeSurface(SDL_Surface *surf)
	{
		SDL_FreeSurface(surf);
	}

	bool pollEvent(SDL_Event &ev)
	{
		return SDL_PollEvent(&ev);
	}

	Uint8 *getKeyState(int * numKeys)
	{
		return SDL_GetKeyState(numKeys);
	}

	bool openAudio(int freq, Uint16 format, bool stereo, int chunksize)
	{
        return Mix_OpenAudio(freq,format,(stereo?2:1),chunksize) == 0;
	}

	Mix_Music * loadMusic(const char *filename)
	{
		return Mix_LoadMUS(filename);
	}
	
	Mix_Chunk * loadChunk(const char *filename)
	{
		return Mix_LoadWAV(filename);
	}

	void freeMusic(Mix_Music *mus)
	{
		Mix_FreeMusic(mus);
	}
	
	void freeChunk(Mix_Chunk *chunk)
	{
		Mix_FreeChunk(chunk);
	}

	bool playChunk(Mix_Chunk *chunk, int loops)
	{
		return Mix_PlayChannel(-1,chunk,loops) != -1;
	}
	
	bool playingMusic()
	{
		return Mix_PlayingMusic();
	}

	bool pausedMusic()
	{
		return Mix_PausedMusic();
	}
	
	bool playMusic(Mix_Music *music, int loops)
	{
        if (hayMusica) return Mix_PlayMusic( music, loops ) != -1;
        else return true;
	}
	
	void resumeMusic()
	{
		Mix_ResumeMusic();
	}
	
	void pauseMusic()
	{
		Mix_PauseMusic();
	}
	
	void haltMusic()
	{
		Mix_HaltMusic();
	}
	
	void stopSound()
	{
		Mix_HaltChannel(-1);
	}
	
	void stopAllChannels()
	{
		stopSound();
		haltMusic();
	}
	
	void closeAudio()
	{
		Mix_CloseAudio();
	}

	int getTicks()
	{
		return SDL_GetTicks();
	}

	void quit() 
	{ 
		SDL_Quit();
	}

	bool initTTF()
	{
		//Initialize SDL_ttf 
		return TTF_Init() != -1;
	}

	TTF_Font *openFont(const char *fileName, int size)
	{
		return TTF_OpenFont(fileName,size);
	}
	
	SDL_Surface *renderTextSolid(TTF_Font *font, const char *text, SDL_Color color)
	{
		return TTF_RenderText_Solid(font, text, color ); 
	}
	void closeFont(TTF_Font *font)
	{
		TTF_CloseFont(font);
	}
	
	void quitTTF()
	{
		TTF_Quit();
	}
}
