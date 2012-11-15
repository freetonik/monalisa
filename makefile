CC=g++
CFLAGS=-g

mainmake: mainf.cc allinc.h
	g++ $(CFLAGS) mainf.cc -lSDL -o monalisa

clean:
	rm -f main a.out
