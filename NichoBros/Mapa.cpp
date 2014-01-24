// Implementamos la clase mapa, estructura de datos utilizada para
// guardar la descripci�n del mapa o nivel.

#include "Mapa.h"

Mapa::Mapa(int Ancho) : ancho(Ancho)
{
	// Generaramos la primera dimensi�n (X)
	terreno   = new eCasillas  *[ancho];
	eventos   = new eEventos   *[ancho];
	extrainfo = new eExtraInfo *[ancho];
	// Generamos la segunda dimensi�n (Y)
	int Aux, Aux2;
	for (Aux = 0; Aux < ancho; Aux++)
	{
		terreno  [Aux] = new eCasillas [20];
		eventos  [Aux] = new eEventos  [20];
		extrainfo[Aux] = new eExtraInfo[20];
		for (Aux2 = 0;Aux2 < 20; Aux2++)
		{
			terreno  [Aux][Aux2] = caVacia;
			eventos  [Aux][Aux2] = evNulo;
			extrainfo[Aux][Aux2] = eExtraInfo(inQuesoRemachado | inDifMas);
		}
	}
}

Mapa::~Mapa()
{
	// Liberamos la dimensi�n Y:
	int Aux;
	for (Aux = 0; Aux < ancho; Aux++)
	{
		delete [] terreno  [Aux];
		delete [] eventos  [Aux];
		delete [] extrainfo[Aux];
	}
	// Liberamos la dimensi�n X.
	delete [] terreno;
	delete [] eventos;
	delete [] extrainfo;
}