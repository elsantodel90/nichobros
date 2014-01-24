// Este archivo define la clase mapa, estructura de datos utilizada
// para guardar la descripci�n del mapa o nivel.

// Evitamos doble inclusi�n...

#ifndef INC_MAPA

#define INC_MAPA

// Incluimos las librer�as necesarias...

#include "InfoObj.h"

// Definimos la clase mapa.

class Mapa
{
public:
	// Constructor:
	Mapa(int Ancho);
	// Destructor
	~Mapa();
	// Obtenci�n de informaci�n.
	eCasillas  Terreno  (int X, int Y)
    {
        if (X < 0 || Y < 0 || X >= ancho || Y >= 20) return caVacia;
        return terreno  [X][Y];
    }
	eEventos   Eventos  (int X, int Y)
    {
        if (X < 0 || Y < 0 || X >= ancho || Y >= 20)
        {
            if (X == -1 || X == ancho || Y == -100)
                return evPisoRigido;
            else
                return evNulo;
        }
        return eventos  [X][Y];
    }
	eExtraInfo ExtraInfo(int X, int Y)
    {
        if (X < 0 || Y < 0 || X >= ancho || Y >= 20) return inDifMas;
        return extrainfo[X][Y];
    }
	// Asignaci�n de informaci�n.
	void SetTerreno  (int X, int Y,eCasillas  nv) {terreno  [X][Y] = nv;}
	void SetEventos  (int X, int Y,eEventos   nv) {eventos  [X][Y] = nv;}
	void SetExtraInfo(int X, int Y,eExtraInfo nv) {extrainfo[X][Y] = nv;}
private:
	eCasillas  **terreno;   // Son
	eEventos   **eventos;   // arrays de 2
	eExtraInfo **extrainfo; // dimensiones!
	int ancho;
};

#endif // INC_MAPA
