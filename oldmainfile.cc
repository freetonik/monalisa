#include "allinc.h"

using namespace std;

// get Uint32 variable (pixel) from surface at x, y
Uint32 get_pixel32(SDL_Surface *surface, int x, int y)
{
	Uint32 *pixels = (Uint32 *)surface->pixels;
	return pixels[(y*surface->w) + x];
}

// put pixel on surface at x,y
void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	Uint32 *pixels = (Uint32 *)surface->pixels;
	pixels[(y * surface->w) + x] = pixel;
}

//display an individual on the screen (surface must be current screen)
void displayIndividual(SDL_Surface *surface, Individual indi)
{
	if (SDL_MUSTLOCK(surface)) 
		if (SDL_LockSurface(surface) < 0)
			cerr << "Couldn't lock screen: " << SDL_GetError() << endl;

	for (int x=0; x<IMG_WIDTH; x++)
	{
		for (int y=0; y<IMG_HEIGHT; y++)
		{
			put_pixel32(surface, x, y, indi.chromosome[x][y]);
		}
	}	

	if (SDL_MUSTLOCK(surface))
		SDL_UnlockSurface(surface);

	SDL_Flip(surface);
}

Uint32 fitness(Individual indi)
{
	int fit=0;	//zero is perfect fitness
	Uint32 delta, c1,c2;
	for (int x=0; x<IMG_WIDTH; x++)
	{
		for (int y=0; y<IMG_HEIGHT; y++)
		{
			c1 = ideal.chromosome[x][y];
			c2 = indi.chromosome[x][y];
			if (c1==c2) indi.eqpoints[x][y]=true;
			else if (c2 < c1)
			{
				if (c2 < c1/2) 
				{
					if (c2 < c1/4) fit=fit+4; else fit=fit+3;
				}
				if (c2 > c1/2)
				{
					if (c2 < c1 * 3/4) fit=fit+2; else fit=fit+1;			
				}
			}
			else if (c2 > c1)
			{
				if (c2 < c1*2)
				{
					if (c2 < c1*1.5) fit=fit+1; else fit=fit+2;
				}			
				if (c2 > c1*2)
				{
					if (c2 < c1*4) fit = fit+3; else fit=fit+4;
				}
			}
		}	
	}	
	return fit;
}

// report the best individual
Individual getAlpha()
{
	int top=0;
	for (int i=1; i<POP_SIZE; i++)
		if (fitness(population[i]) < fitness(population[top])) top=i;
	return population[top];
}

// sort population in ascending order of fitness
void sortPopulation()
{
	int i, j;
	Individual key;
	for (j=1; j < POP_SIZE; j++)
	{
		key=population[j];
		for (i=j-1; (i>=0) && (population[i].fitness > key.fitness); i--)
		{
			population[i+1] = population[i];
		}
		population[i+1]=key;
	}
	return;
}

void fillAroundEqPoints(Uint32 chromosome[][IMG_HEIGHT], bool eqpoints[][IMG_HEIGHT])
{
	bool done;
	for (int x=1; x<IMG_WIDTH-1; x++)
		for (int y=1; y<IMG_HEIGHT-1; y++)
		{
			if (!eqpoints[x][y])
			done=false;
			{
				if (eqpoints[x-1][y-1] && !done) 
				{
					chromosome[x][y]=chromosome[x-1][y-1];
					done=true;
				}
				if (eqpoints[x][y-1] && !done) 
				{
					chromosome[x][y]=chromosome[x][y-1];
					done=true;
				}
				if (eqpoints[x+1][y-1] && !done) 
				{
					chromosome[x][y]=chromosome[x+1][y-1];
					done=true;
				}
				if (eqpoints[x-1][y] && !done)
				{
					chromosome[x][y]=chromosome[x-1][y];
					done=true;
				}
				if (eqpoints[x+1][y] && !true) 
				{
					chromosome[x][y]=chromosome[x+1][y];
					done=true;
				}
				if (eqpoints[x-1][y+1] && !true) 
				{
					chromosome[x][y]=chromosome[x-1][y+1];
					done=true;
				}
				if (eqpoints[x][y-1] && !done) 
				{
					chromosome[x][y]=chromosome[x][y-1];
					done=true;
				}
				if (eqpoints[x+1][y+1] && !done)
				{
					chromosome[x][y]=chromosome[x+1][y+1];
					done=true;
				}
			}
		}	

}

void mutate(Uint32 chromosome[][IMG_HEIGHT], int indifitness, bool eqpoints[][IMG_HEIGHT])
{

	Individual mutated;
	int x, y;
	Uint32 c1, c2;
	for (int x=0; x<IMG_WIDTH; x++)
		for (int y=0; y<IMG_HEIGHT; y++)
		{
			mutated.chromosome[x][y]=chromosome[x][y];	
			c1 = ideal.chromosome[x][y];
			c2 = mutated.chromosome[x][y];
			if (c1==c2) eqpoints[x][y]=true;
			else if (c2 < c1)
			{
				if (c2 < c1*0.5) 
				{
					if (c2 < c1*0.25) 
						if (c2 < c1*0.13) mutated.chromosome[x][y]+=rand()%(c1*87/100);
						else mutated.chromosome[x][y]+=rand()%(c1*75/100);
					if (c2 > c1*0.25)
						if (c2 < c1*0.38) mutated.chromosome[x][y]+=rand()%(c1*62/100);
						else mutated.chromosome[x][y]+=rand()%(c1*5/10);
				}
				if (c2 > c1/2)
				{
					if (c2 < c1*0.75)
						if (c2 < c1*0.63) mutated.chromosome[x][y]+=rand()%(c1*37/100);
						else mutated.chromosome[x][y]+=rand()%(c1*25/100);			
					if (c2 > c1*0.75)
						if (c2 < c1*0.88) mutated.chromosome[x][y]+=rand()%(c1*18/100);
						else mutated.chromosome[x][y]++;
				}
			}
			else if (c2 > c1)
			{
				if (c2 < c1*2)
				{
					if (c2 < c1*1.25)
						if (c2 < c1*1.12) mutated.chromosome[x][y]--;
						else mutated.chromosome[x][y]-=rand()%(c1*12/100);
					if (c2 > c1*1.25)
						if (c2 < c1*1.38) mutated.chromosome[x][y]-=rand()%(c1*25/100);
						else mutated.chromosome[x][y]-=rand()%(c1*38/100);
				}	
				if (c2 > c1*2)
				{
					if (c2 < c1*2.25)
						if (c2 < c1*2.13) mutated.chromosome[x][y]-=rand()%(c1*2);
						else mutated.chromosome[x][y]-=rand()%(c1*2 + c1*13/100);
					if (c2 > c1*2.25)
						if (c2 < c1*2.38) mutated.chromosome[x][y]-=rand()%(c1*2 + c1*25/100);
						else mutated.chromosome[x][y]-=rand()%(c1*2 + c1*38/100);
				}
			}
		}
		
	for (int i=0; i<MUTATE_POINTS; i++)
	{
		x = rand()%(IMG_WIDTH);
		y = rand()%(IMG_HEIGHT);
		
		if (!eqpoints[x][y]) 
			if (!eqpoints[x][y]) 
				if (mutated.chromosome[x][y]<COLOR_WHITE) mutated.chromosome[x][y]++;
				else mutated.chromosome[x][y]--;

	}

	for (int i=0; i<IMG_WIDTH * IMG_HEIGHT / 100 * KNOWN; i++)
	{
		x = rand()%(IMG_WIDTH);
		y = rand()%(IMG_HEIGHT);
		
		if (!eqpoints[x][y]) 
		{
			if (!eqpoints[x][y]) mutated.chromosome[x][y]=ideal.chromosome[x][y];
			eqpoints[x][y]=true;
		}
		
	
	}

	mutated.fitness = fitness(mutated);

	if (mutated.fitness < indifitness) 
	{
		cout << "$";
		for (int x=0; x<IMG_WIDTH; x++)
			for (int y=0; y<IMG_HEIGHT; y++)
				chromosome[x][y]=mutated.chromosome[x][y];	
	}


}


void printPopulation()
{
	cout << endl;
	for (int i=0; i<POP_SIZE; i++)
		cout << "Fitness of individual " << i << " " << population[i].fitness << endl;
	cout << endl;
}

void OddHorizontalCrossover(Uint32 chromosome1[][IMG_HEIGHT], Uint32 chromosome2[][IMG_HEIGHT])
{
	for (int x=0; x<IMG_WIDTH; x++)
		for (int y=0; y<IMG_HEIGHT; y+=2)
			chromosome1[x][y]=chromosome2[x][y];
}

void OddVerticalCrossover(Uint32 chromosome1[][IMG_HEIGHT], Uint32 chromosome2[][IMG_HEIGHT])
{
	for (int x=0; x<IMG_WIDTH; x+=2)
		for (int y=0; y<IMG_HEIGHT; y++)
			chromosome1[x][y]=chromosome2[x][y];
}


void CutPointCrossover(Uint32 chromosome1[][IMG_HEIGHT], Uint32 chromosome2[][IMG_HEIGHT], int cut_x, int cut_y)
{
	for (int x=cut_x; x<IMG_WIDTH; x++)
		for (int y=cut_y; y<IMG_HEIGHT; y++)
			chromosome1[x][y]=chromosome2[x][y];
}

// EVOLUTION
void evolve()
{
	int q=0;
	int x,y, chance, cut_x, cut_y, j;
	Individual child;
	cout << "... starting evolution..." << endl;
	while (population[0].fitness!=0)
	{
		cout << "G: " << q << endl;
		
		for (j=POP_SIZE-1; j>POP_SIZE/2; j--)
		{
			for (x=0; x<IMG_WIDTH; x++)
				for (y=0; y<IMG_HEIGHT; y++)
					child.chromosome[x][y]=population[j].chromosome[x][y];
		
			//XOVER
			chance = rand()%2;
			switch (chance)
			{
			case(0):
				OddHorizontalCrossover(child.chromosome, population[POP_SIZE-j].chromosome);
			case(1):
				OddVerticalCrossover(child.chromosome, population[POP_SIZE-j].chromosome);
			case(3):
				cut_x=rand()%IMG_WIDTH;
				cut_y=rand()%IMG_HEIGHT;
				CutPointCrossover(child.chromosome, population[POP_SIZE-j].chromosome, cut_x, cut_y);
			}
			child.fitness=fitness(child);
			if (child.fitness < population[j].fitness)
			{
				for (x=0; x<IMG_WIDTH; x++)
					for (y=0; y<IMG_HEIGHT; y++)
						population[j].chromosome[x][y]=child.chromosome[x][y];			
			}
		}
		
		//mutation
		for (int i=0; i<POP_SIZE; i++)
		{
			mutate(population[i].chromosome, population[i].fitness, population[i].eqpoints);
			fillAroundEqPoints(population[i].chromosome, population[i].eqpoints);
			population[i].fitness=fitness(population[i]);
		}

		
		sortPopulation();
		//cout << ", F=" << population[0].fitness << endl;
		printPopulation();
		displayIndividual(screen, population[0]);
		q++;
		
	}
}

int main(int argc, char *argv[])
{	
	srand(time(NULL)); // randomizer
	// opening file by parameter
	if ( argc != 2 ) 
    	  	 cout<<"usage: "<< argv[0] <<" <filename>\n";
 	else {
   	    ifstream the_file ( argv[1] );
   
   	    if ( !the_file.is_open() )
      	    	cout<<"Could not open file\n";
    	    else {
		
      		cout<<"File " << argv[1] << ": generating ideal...";	
   	    }

	SDL_Surface *image = NULL;	// loaded image
	SDL_Init(SDL_INIT_EVERYTHING);	// initialize SDL
	screen = SDL_SetVideoMode(IMG_WIDTH, IMG_HEIGHT, 32, SDL_SWSURFACE); // set screen 640x480
	SDL_WM_SetCaption("Monalisa","Monalisa");
	image = SDL_LoadBMP(argv[1]);	// open our file
	SDL_Event event;
	int done=0;			// if done==1 program terminates

	SDL_BlitSurface (image, NULL, screen, NULL);	// put image on the screen
	SDL_Flip(screen);	

	// creating ideal from given .BMP-file
	//ideal.fitness=0;
	for (int x=0; x<IMG_WIDTH; x++)
	{
		for (int y=0; y<IMG_HEIGHT; y++)
		{
			ideal.chromosome[x][y] = get_pixel32(screen, x, y);
		}
	}
	
	ideal.fitness=fitness(ideal);
	cout << "Ideal fitness=" << ideal.fitness << endl;
	//creating population
	for (int i=0; i<POP_SIZE; i++)
	{
		for (int x=0; x<IMG_WIDTH; x++)
		{
			for (int y=0; y<IMG_HEIGHT; y++)
			{
				population[i].chromosome[x][y] = (Uint32)rand()%4292967295;
				population[i].eqpoints[x][y]=false;
			}
		}
		population[i].fitness = fitness(population[i]);
	}
	
	sortPopulation();
	printPopulation();
	mutate(population[0].chromosome, population[0].fitness, population[0].eqpoints);
	population[0].fitness=fitness(population[0]);
	displayIndividual(screen, population[0]);
	sortPopulation();
	printPopulation();
	evolve();	
	
	
	cout << "Done. Close the window to quit" << endl;
	// at the end we can quit by pressing "X" button
	while(!done)
	{
		while ( SDL_PollEvent(&event) ) //Check for events
	 	{
	       		if ( event.type == SDL_QUIT ) //Check if the 'x' button has been pressed
		 	{ 
				done = 1; 
			}
	        }
	}

	return 0;
}}
