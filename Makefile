# cs335 duckhunt
# to compile your project, type make and press enter

CFLAGS = -I ./include
LIB = ./lib/fmod/libfmodex64.so ./libggfonts.so
LFLAGS = $(LIB) -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: duckhunt

duckhunt: duckhunt.cpp jasonT.cpp gPeregrina.cpp miguelT.cpp cuongL.cpp adebidolaJ.cpp
	g++ $(CFLAGS) duckhunt.cpp header.h jasonT.cpp gPeregrina.cpp miguelT.cpp cuongL.cpp adebidolaJ.cpp ppm.cpp fmod.c -Wall -Wextra $(LFLAGS) -o duckhunt

clean:
	rm -f duckhunt
	rm -f *.o

