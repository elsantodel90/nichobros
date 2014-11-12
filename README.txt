Para jugar el juego, descomprimir y correr el ejecutable correspondiente (Nichobros o Nichobros.exe en windows)
Para poderlo jugar en linux, instalar los paquetes de la SDL, SDL_image, SDL_mixer y SDL_ttf (instalar todo
lo que diga "sdl" por las dudas no vendria mal ;) ). Para windows se supone que todo eso esta incluido a manopla en las dll.
En linux se podria pero es mejor pedir paquetitos :D (Si usaste dosbox y/o scummvm y/o juegos en linux, es posible que ya
esten todos o varios instalados pero mejor chequear).

Ejemplo en ubuntu 12.04:
sudo aptitude install libsdl1.2debian libsdl-image1.2 libsdl-mixer1.2 libsdl-ttf2.0-0 libsdl-sound1.2

El juego incluye:
    -> Este README
    -> Ejecutable del juego y del creador de niveles (NEN)
    -> Codigo fuente del juego y del creador de niveles.
    -> Manual (Manual Nichobros.pdf)
    -> Graficos y Fuentes necesarias para la visualizacion del juego.
    -> Niveles jugables
    -> Partituras con la musica original del juego (En pdf y formato musescore)
    -> Midis con la musica original del juego (New! Para deleite de los que tengan un sintetizador hardware o jueguen con midis)
    -> Sonido

Editor de niveles:
Se corre con el ejecutable NichobrosEN o NichobrosEN.exe en windows. Ver el manual.

Informacion general de compilacion:
-> En Debug.h hay un define para compilar con informacion de depuracion (muchos carteles con datos, fps, etc).
-> Se compilan por un lado el nichobros, y por otro el editor de niveles, que esta en la subcarpeta NEN. Cada uno tiene su makefile.
-> TODO: Lograr compilacion nativa 64 bits, para tener un ejecutable nativo de 64 bits (y evitar problemas de versiones de los .so).

Compilacion en Ubuntu 12.04 *32bits* 
(deberia ser igual en versiones mas nuevas, mientras sea de 32 bits. El codigo deberia poderse compilar en 64 bits con cambios menores, hay algunas cositas feas como casteos de punteros a enteros que se rompen al compilarlo en 64 bits. Se puede compilar en una maquina virtual de 32 bits por ejemplo):
-> Se instalan paquetes de SDL necesarios:
      sudo aptitude install libsdl1.2debian libsdl-image1.2 libsdl-mixer1.2 libsdl-ttf2.0-0 libsdl-sound1.2 libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev libsdl-sound1.2-dev
-> Por supuesto, se instala g++ si no se tiene instalado, con sudo aptitude install g++
-> Se corren los makefiles.

Compilacion en Windows (testeado solo en 32 bits):
-> Al compilar el editor de niveles, hay que modificar una parte en el Princip.cpp. Esta comentado lo que hay que cambiar, que en windows no compila.
-> Deberia funcionar, eventualmente linkeando con los estaticos de las dll provistas de SDL (descargar de las paginas de SDL).
-> Se proveen archivos Make.bat con comandos de compilacion, analogos a los Makefile para linux.
