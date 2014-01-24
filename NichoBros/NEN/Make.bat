g++ -c -Wall -o Mapa.o Mapa.cpp
g++ -c -Wall -o Princip.o Princip.cpp
g++ -c -Wall -o RutinasSDL/rutinasSDL.o RutinasSDL/rutinasSDL.cpp
g++ -o NichobrosEN Mapa.o Princip.o RutinasSDL/rutinasSDL.o -lMinGW32 -lSDLmain -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer
move NichobrosEN.exe ..