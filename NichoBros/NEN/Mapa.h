// Este archivo define la clase mapa, estructura de datos utilizada
// para guardar la descripción del mapa o nivel.

// Evitamos doble inclusión...

#ifndef INC_MAPA

#define INC_MAPA

// Incluimos las librerías necesarias...

#include <stack>
#include "InfoObj.h"

// Definimos la clase mapa.

class Mapa
{
public:
	// Constructor:
	Mapa(int Ancho);
	// Destructor
	~Mapa();
	// Obtención de información.
	eCasillas  Terreno  (int X, int Y) const {return terreno  [X][Y];}
	eEventos   Eventos  (int X, int Y) const {return eventos  [X][Y];}
	eExtraInfo ExtraInfo(int X, int Y) const {return extrainfo[X][Y];}
	// Asignación de información.
	void SetTerreno  (int X, int Y,eCasillas  nv)
    {
        if (terreno[X][Y] != nv)
        {
            safe(X,Y);
            terreno  [X][Y] = nv;
        }
    }
	void SetEventos  (int X, int Y,eEventos   nv)
    {
        if (eventos[X][Y] != nv)
        {
            safe(X,Y);
            eventos  [X][Y] = nv;
        }
    }
	void SetExtraInfo(int X, int Y,eExtraInfo nv) 
    {
        if (extrainfo[X][Y] != nv)
        {
            safe(X,Y);
            extrainfo[X][Y] = nv;
        }
    }
    
    void SetEventosAndExtraInfo(int X,int Y, eEventos nEv, eExtraInfo nExIn)
    {
        if (!(eventos[X][Y] == nEv && extrainfo[X][Y] == nExIn))
        {
            safe(X,Y);
            eventos[X][Y] = nEv;
            extrainfo[X][Y] = nExIn;
        }
    }
    
    void storyClear();
    void Undo();
    void Redo();
    
private:

    struct Cambio
    {
        int x,y;
        eCasillas cas;
        eEventos ev;
        eExtraInfo extra;
    };
    
    void safe(int X,int Y);
    void keep(int X,int Y,std::stack<Cambio> &s);
    void perform(const Cambio &c);
    
	eCasillas  **terreno;   // Son
	eEventos   **eventos;   // arrays de 2
	eExtraInfo **extrainfo; // dimensiones!
	int ancho;
    std::stack<Cambio> vBack;
    std::stack<Cambio> vFor;
};

#endif // INC_MAPA
