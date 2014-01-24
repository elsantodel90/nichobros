// Implementamos las funciones m�s complejas de la clase coleccion.

// Incluimos librer�as necesarias...

#include "Colec.h"

// Colecciones
Coleccion *Juanchis		= NULL;
Coleccion *Tortugas		= NULL;
Coleccion *Carteles		= NULL;
Coleccion *Explosiones	= NULL;
Coleccion *ColecMonedas = NULL;
Coleccion *BolasFuego	= NULL;
Coleccion *Rebotadores  = NULL;
Coleccion *BolasPico    = NULL;
Coleccion *PuentesCaen  = NULL;
Coleccion *VerdesLocos  = NULL;
Coleccion *ColecRobots  = NULL;
Coleccion *Rayos		= NULL;
Coleccion *Plataformas	= NULL;
Coleccion *PinchesPalo  = NULL;
Coleccion *Triturines	= NULL;
Coleccion *Caecuchos    = NULL;
Coleccion *Teleporters  = NULL;
Coleccion *Puertas      = NULL;
Coleccion *Sauriolos	= NULL;

// Implementaci�n de las funciones.

Coleccion::~Coleccion()
{
	// Vaciamos la lista.
	for (;tamano > 0;Eliminar(0));
}

void Coleccion::Agregar(ObjetoBase *Elemento)
{
	Nodo *Nuevo;
	Nuevo = new Nodo();
	// Si el �ltimo elemento es NULL, la lista est� vac�a.
	if (!ultimo)
		primero = actual = Nuevo;
	else
		ultimo->Sig = Nuevo;
	ultimo = Nuevo;
	Nuevo->Sig = NULL;
	Nuevo->Dat = Elemento;
	tamano++;
}

ObjetoBase *Coleccion::Eliminar(long Indice)
{
	// Si el elemento est� por encima del total de elementos, no
	// lo eliminamos. Por el contrario, si est� por debajo, elimi-
	// namos el primero.
	if (tamano > 0 && Indice < tamano)
	{
		Nodo *Aux, *Ult;
		ObjetoBase *ValorRet;
		Ult = NULL;
		Aux = primero;
		while (Indice > 0)
		{
			Ult = Aux;
			Aux = Aux->Sig;
			Indice--;
		}
		if (Ult != NULL)
		{
			Ult->Sig = Aux->Sig;
			if (Ult->Sig == NULL)
				ultimo = Ult;
		}
		else
		{
			primero  = Aux->Sig;
			if (primero == NULL)
				ultimo = NULL;
		}
		ValorRet = Aux->Dat;
		delete Aux;
		if (actual == Aux)
			actual = primero;
		tamano--;
		return ValorRet;
	}
	return NULL;
}

ObjetoBase *Coleccion::Avanzar()
{
	ObjetoBase *Aux;
	Aux = actual->Dat;
	actual = actual->Sig; // Avanzamos!
	if (!actual) // �Llegamos al final?
		actual = primero; // Volvemos al principio!
	return Aux;
}