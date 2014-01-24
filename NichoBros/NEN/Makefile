../NichobrosEN : Mapa.o Princip.o RutinasSDL/rutinasSDL.o
		g++ -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer -o NichobrosEN Mapa.o Princip.o RutinasSDL/rutinasSDL.o
		mv NichobrosEN ..

Mapa.o : Mapa.cpp Mapa.h InfoObj.h
		g++ -c -Wall -o Mapa.o Mapa.cpp

Princip.o : Princip.cpp Mapa.h InfoObj.h RutinasSDL/rutinasSDL.h
		g++ -c -Wall -o Princip.o Princip.cpp

RutinasSDL/rutinasSDL.o : RutinasSDL/rutinasSDL.cpp
		g++ -c -Wall -o RutinasSDL/rutinasSDL.o RutinasSDL/rutinasSDL.cpp

.PHONY : clean

clean : 
		rm -f *.o
		rm -f RutinasSDL/rutinasSDL.o
