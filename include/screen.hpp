// in this file we will 
#include <SDL2/SDL.h>
#include <vector>
#include <deque>

void blockRender( SDL_Renderer* aRenderer, int aTopLeftX, int aTopLeftY, int aXWidth, int aYWidth );

void populateBlocks( SDL_Renderer* aRenderer, int aBlocksX, int aBlocksY );

void populateBlocksWithBoundaries( SDL_Renderer* aRenderer, int aBlocksX, int aBlocksY );


struct Maze
{

int mBlocksX;
int mBlocksY;

SDL_Renderer* mRenderer;

std::deque< std::pair< const int, const int>> mStack;
// std::deque< std::pair< int, int>> mStack;

std::vector<std::vector<bool>> mVisited;

void rendermaze();

//gets neighbour if one exists, or pops one off the stack
void getNeighbour(std::pair<int, int>& aCurrent);

bool validNeighbour(int aX, int aY);

//constructor
Maze(int aBlocksX, int aBlocksY, SDL_Renderer* aRenderer);


};