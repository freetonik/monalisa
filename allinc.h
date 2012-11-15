#ifndef ALLINC
#define ALLINC

#include <stdio.h>
#include <time.h>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <fstream>
#include <stdint.h>

#include <SDL/SDL.h>

#define POP_SIZE 2
#define GENERATIONS 250

#define IMG_WIDTH 640
#define IMG_HEIGHT 480
#define MUTATE_POINTS 3000
#define KNOWN 0.05


struct Individual{
	Uint32 chromosome[IMG_WIDTH][IMG_HEIGHT];
	Uint32 fitness;
	bool eqpoints[IMG_WIDTH][IMG_HEIGHT];

};

struct Population{
	Individual pop[POP_SIZE];
};

Uint32 COLOR_WHITE = 4294967295;
Individual ideal;	// the ideal to compare with
static struct Individual population[POP_SIZE]; //population

SDL_Surface *screen = NULL;	// our screen; always remains the same


//declarations

Uint32 get_pixel32(SDL_Surface *surface, int x, int y);
void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel);
void displayIndividual(SDL_Surface *surface, Individual indi);
Uint32 fitness(Individual indi);
Individual getAlpha();
void sortPopulation();
void mutate(Uint32 chromosome[][IMG_HEIGHT], int indifitness, bool eqpoints[][IMG_HEIGHT]);
void printPopulation();
void OddHorizontalCrossover(Uint32 chromosome1[][IMG_HEIGHT], Uint32 chromosome2[][IMG_HEIGHT]);
void OddVerticalCrossover(Uint32 chromosome1[][IMG_HEIGHT], Uint32 chromosome2[][IMG_HEIGHT]);
void CutPointCrossover(Uint32 chromosome1[][IMG_HEIGHT], Uint32 chromosome2[][IMG_HEIGHT], int cut_x, int cut_y);
void evolve();
void fillAroundEqPoints(Uint32 chromosome[][IMG_HEIGHT], bool eqpoints[][IMG_HEIGHT]);



#endif 
