#Nichobros

Un sencillo juego de plataformas con mucho cariño, inspirado en tantos otros. Nichobros es parte del dominio público (public domain), lo
que significa que se puede utilizar su código fuente y ejecutables provistos, así como todo el material gráfico y de sonido, de manera 100% libre,
para lo que se le dé la gana.
Si piensa utilizarlo para cualquier cosa que no sea jugarlo, aprecio que se ponga en contacto y me lo haga saber.

#Contenidos

El juego incluye:

- Este README
- Ejecutable del juego y del creador de niveles (NEN)
- Código fuente del juego y del creador de niveles.
- Manual (Manual Nichobros.pdf)
- Gráficos y Fuentes necesarias para la visualizacion del juego.
- Niveles jugables
- Partituras con la música original del juego (En pdf y formato musescore)
- Midis con la música original del juego ([New!] Para deleite de los que tengan un sintetizador hardware o jueguen con midis)
- Sonido

#Para jugarlo:

Descomprimir y ejecutar de acuerdo a estas instrucciones:

##Windows:
El juego debería funcionar directamente al ejecutar el .exe:
Nichobros.exe para el juego, NichobrosEN.exe para el editor de niveles.

##Linux (Testeado en **ubuntu**):
###Instalación de la SDL
Para poderlo jugar en linux, instalar los paquetes de la SDL, SDL_image, SDL_mixer, SDL_ttf y SDL_sound.
(Para windows se supone que todo eso está incluído en las dll).

- Ejemplo en ubuntu 12.04, **32 bits** (debería funcionar casi igual en versiones más nuevas **DE 32 BITS**):

  sudo aptitude install libsdl1.2debian libsdl-image1.2 libsdl-mixer1.2 libsdl-ttf2.0-0 libsdl-sound1.2

- Ejemplo en ubuntu 14.04, **64 bits** (debería funcionar casi igual en versiones más nuevas **DE 64 BITS**):

  sudo dpkg --add-architecture i386
  
  sudo aptitude update
  
  sudo aptitude install libsdl1.2debian:i386 libsdl-image1.2:i386 libsdl-mixer1.2:i386 libsdl-ttf2.0-0:i386 libsdl-sound1.2:i386

El primer "sudo dpkg --add-architecture i386" y la distinción de paquetes ":i386" se debe a que el juego está compilado para 32 bits
(habría que hacer retoques menores al codigo para que se pueda compilar en 64 bits).

###Ejecución:
El ejecutable del juego es el archivo "Nichobros", que se ejecuta directamente una vez realizado el paso previo.
El ejecutable del editor de niveles es "NichobrosEN".

#Compilación:
Para los valientes que quieran compilar el código (por ejemplo, porque desean hacerle modificaciones! :) ), a continuación van las instrucciones de compilación.
Por favor contactarme para hacerme saber que piensan utilizar el código fuente o modificarlo en forma alguna.
##Información general de compilación:
- En Debug.h hay un define para compilar con informacion de depuración (muchos carteles con datos, fps, etc).
- Se compilan por un lado el nichobros, y por otro el editor de niveles, que está en la subcarpeta NEN. Cada uno tiene su makefile.
- TO-DO: Lograr compilacion nativa 64 bits, para tener un ejecutable nativo de 64 bits (y evitar tener que instalar las versiones de 32 bits de la SDL).

##Compilación en Ubuntu 12.04 **32bits**

Debería ser igual en versiones más nuevas, mientras sean de 32 bits.
El código debería poderse compilar en 64 bits con cambios menores, hay algunas cositas feas como casteos de punteros a enteros que se rompen
al compilarlo en 64 bits. Se puede compilar en una maquina virtual de 32 bits por ejemplo.

- Se instalan paquetes de SDL necesarios:

      sudo aptitude install libsdl1.2debian libsdl-image1.2 libsdl-mixer1.2 libsdl-ttf2.0-0 libsdl-sound1.2 libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev libsdl-sound1.2-dev
- Por supuesto, se instala g++ si no se tiene instalado, con sudo aptitude install g++
- Se corren los makefiles (ejecutar un simple comando "make", en el directorio con el código). That's it.

##Compilación en Windows (testeado sólo en 32 bits):
- Al compilar el editor de niveles, hay que modificar una parte en el Princip.cpp. Está comentado lo que hay que cambiar, que en windows no compila.
- Debería funcionar, eventualmente linkeando con los estaticos de las dll provistas de SDL (descargar de las paginas de SDL).
- Se proveen archivos Make.bat con comandos de compilación, análogos a los Makefile para linux.
