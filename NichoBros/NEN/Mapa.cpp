// Implementamos la clase mapa, estructura de datos utilizada para
// guardar la descripción del mapa o nivel.
#include <iostream>
#include "Mapa.h"

Mapa::Mapa(int Ancho) : ancho(Ancho)
{
	// Generaramos la primera dimensión (X)
	terreno   = new eCasillas  *[ancho];
	eventos   = new eEventos   *[ancho];
	extrainfo = new eExtraInfo *[ancho];
	// Generamos la segunda dimensión (Y)
	int Aux;
	for (Aux = 0; Aux < ancho; Aux++)
	{
		terreno  [Aux] = new eCasillas [20];
		eventos  [Aux] = new eEventos  [20];
		extrainfo[Aux] = new eExtraInfo[20];
        for (int Aux2 = 0;Aux2 < 20; Aux2++)
		{
			terreno  [Aux][Aux2] = caVacia;
			eventos  [Aux][Aux2] = evNulo;
			extrainfo[Aux][Aux2] = eExtraInfo(inQuesoRemachado | inDifMas);
		}
	}
}

Mapa::~Mapa()
{
	// Liberamos la dimensión Y:
	int Aux;
	for (Aux = 0; Aux < ancho; Aux++)
	{
		delete [] terreno  [Aux];
		delete [] eventos  [Aux];
		delete [] extrainfo[Aux];
	}
	// Liberamos la dimensión X.
	delete [] terreno;
	delete [] eventos;
	delete [] extrainfo;
}

void Mapa::keep(int X,int Y,std::stack<Mapa::Cambio> &s)
{
    Cambio c;
    c.x = X;
    c.y = Y;
    c.cas = Terreno(X,Y);
    c.ev = Eventos(X,Y);
    c.extra = ExtraInfo(X,Y);
    s.push(c);
}

void Mapa::safe(int X,int Y)
{
    keep(X,Y,vBack);
    vFor = std::stack<Cambio>();
}

void Mapa::perform(const Mapa::Cambio &c)
{
    terreno[c.x][c.y] = c.cas;
    eventos[c.x][c.y] = c.ev;
    extrainfo[c.x][c.y] = c.extra;
}

void Mapa::storyClear()
{
    vBack = vFor = std::stack<Cambio>();
}

void Mapa::Undo()
{
    if (!vBack.empty())
    {
        Cambio c = vBack.top(); vBack.pop();
        keep(c.x,c.y,vFor);
        perform(c);
    }
}

void Mapa::Redo()
{
    if (!vFor.empty())
    {
        Cambio c = vFor.top(); vFor.pop();
        keep(c.x,c.y,vBack);
        perform(c);
    }
}
