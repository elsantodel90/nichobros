// Este archivo de cabecera lo utilizamos para definir la clase
// colecci�n, que no es m�s que una estructura de datos parecida
// a un array din�mico, pero de m�s flexibilidad.
// Tambien declaramos los punteros a objetos de la clase coleccion
// que ser�n listas de objetos del juego.

// Evitamos doble inclusi�n...

#ifndef INC_COLEC

#define INC_COLEC

#include <cstdlib>

// Definicion de la clase ObjetoBase. Esta clase est� vac�a: per-
// mite que objetos de diferentes clases sean utilizados indistin-
// tamente por nuestra super coleccion. :-)

class ObjetoBase
{
public:
	virtual void Check() = 0;
	virtual void Mostrar() = 0;
	// Clase practicamente vac�a!
	// Esta decision es mala: pero es consecuencia de trasladar
	// c�digo de visual basic a visual c++...
	// De hecho, antes era peor y la clase estaba vac�a.
};

// Definimos la clase nodo, que ser�a un elemento de la coleccion.

class Coleccion; // Declaramos anticipadamente para advertir al compilador.

class Nodo
{
friend class Coleccion;
public:
	Nodo() : Sig(NULL), Dat(NULL) {}
private:
	Nodo	   *Sig;
	ObjetoBase *Dat;
};

// Definimos la clase coleccion.

class Coleccion
{
friend class Tortuga;
public:
	// Constructor.
	Coleccion() : primero(NULL),
				   actual(NULL),
				   ultimo(NULL), tamano(0) {}
	// Destructor.
	~Coleccion();
	// Agrega un elemento al final de la lista.
	void Agregar(ObjetoBase *Elemento);
	// Elimina un elemento determinado (si existe)
	ObjetoBase *Eliminar(long Indice);
	// Selecciona como nodo actual al primero.
	void SelPrimero() {actual = primero;}
	// Devuelve el elemento actual y avanza al siguiente.
	ObjetoBase *Avanzar();
	// Devuelve el tama�o.
	const long Tamano() {return tamano;}
private:
	Nodo *primero;
	Nodo *actual;
	Nodo *ultimo;
	long tamano;
};

// Colecciones
extern Coleccion *Juanchis;
extern Coleccion *Tortugas;
extern Coleccion *Carteles;
extern Coleccion *Explosiones;
extern Coleccion *ColecMonedas;
extern Coleccion *BolasFuego;
extern Coleccion *Rebotadores;
extern Coleccion *BolasPico;
extern Coleccion *PuentesCaen;
extern Coleccion *VerdesLocos;
extern Coleccion *ColecRobots;
extern Coleccion *Rayos;
extern Coleccion *Plataformas;
extern Coleccion *PinchesPalo;
extern Coleccion *Triturines;
extern Coleccion *Caecuchos;
extern Coleccion *Teleporters;
extern Coleccion *Puertas;
extern Coleccion *Sauriolos;

#endif // INC_COLEC
