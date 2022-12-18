#include "../include/screen.hpp"
#include "../include/utils.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>

void blockRender( SDL_Renderer* aRenderer, int aTopLeftX, int aTopLeftY, int aXWidth, int aYWidth )
{
  // StopWatch s = {"rendering"};
  // SDL_SetRenderDrawColor(aRenderer, std::rand() % 256, std::rand() % 256, std::rand() % 256, 0xFF);
  SDL_SetRenderDrawColor(aRenderer, 255, 255, 255, 0xFF);
  //these will be fixed to be the screen width and height
  int lW, lH;
  SDL_GetRendererOutputSize( aRenderer, &lW, &lH );
  
  for (int widthIter = aTopLeftX; widthIter < std::min(aTopLeftX + aXWidth, lW); widthIter++)
  {
    for (int heightIter = aTopLeftY; heightIter < std::min(aTopLeftY + aYWidth, lH); heightIter++) 
    {
      if (SDL_RenderDrawPoint(aRenderer, widthIter, heightIter) < 0) abort();
      // std::cout << widthIter << " " << heightIter << "\n";
    }    
  }
  SDL_SetRenderDrawColor(aRenderer, 86, 29, 94, 0xFF);
  // SDL_RenderPresent(aRenderer);
}

void populateBlocks( SDL_Renderer* aRenderer, int aBlocksX, int aBlocksY )
{
  StopWatch s = {"populating"};
  //get screen width and height
  int lW, lH;
  SDL_GetRendererOutputSize( aRenderer, &lW, &lH );

  //get block width
  int lBlockWidth = lW / aBlocksX;

  //get block height
  int lBlockHeight = lH / aBlocksY;

  for ( int widthIter = 0; widthIter < lW ; widthIter += lBlockWidth )
  {
    for ( int heightIter = 0; heightIter < lH; heightIter += lBlockHeight )
      blockRender( aRenderer, widthIter, heightIter, lBlockWidth, lBlockHeight );
  }
  SDL_RenderPresent(aRenderer);
}

void populateBlocksWithBoundaries( SDL_Renderer* aRenderer, int aBlocksX, int aBlocksY )
{

  StopWatch s = {"populating"};
  //get screen width and height
  int lW, lH;
  SDL_GetRendererOutputSize( aRenderer, &lW, &lH );

  // fix the width of the bars
  int lBarWidth = 1;
  
  //get block width
  int lBlockWidth = lW / aBlocksX - 2 * lBarWidth;

  //get block height
  int lBlockHeight = lH / aBlocksY - 2 * lBarWidth;

  //iterate, but offset by the width of the bars
  for ( int widthIter = lBarWidth; widthIter < lW ; widthIter += lBlockWidth + lBarWidth )
  {
    for ( int heightIter = 0; heightIter < lH; heightIter += lBlockHeight + lBarWidth  )
      blockRender( aRenderer, widthIter, heightIter, lBlockWidth, lBlockHeight );
  }
  SDL_RenderPresent(aRenderer);
}

Maze::Maze(int aBlocksX, int aBlocksY, SDL_Renderer* aRenderer) : 
  mBlocksX{aBlocksX}, mBlocksY{aBlocksY}, mRenderer{aRenderer}
{}

bool Maze::validNeighbour(int aX, int aY)
{
  if ( (aX < 0) or (aX >= mBlocksX) ) return false;
  if ( (aY < 0) or (aY >= mBlocksY) ) return false;
  return !mVisited[aX][aY]; //if it's been visited, return false
}


void Maze::getNeighbour( std::pair<int, int>& aCurrent )
{
  int lCurrentX = aCurrent.first;
  int lCurrentY = aCurrent.second;

  std::vector<std::pair<int, int >> lPossibleNextSteps;

  //check north
  if (validNeighbour(lCurrentX, lCurrentY - 1)) lPossibleNextSteps.push_back(std::make_pair(lCurrentX, lCurrentY - 1));

  //check east
  if (validNeighbour(lCurrentX + 1, lCurrentY )) lPossibleNextSteps.push_back(std::make_pair(lCurrentX + 1, lCurrentY ));
  
  //check south
  if (validNeighbour(lCurrentX, lCurrentY + 1)) lPossibleNextSteps.push_back(std::make_pair(lCurrentX, lCurrentY + 1 ));
  
  //check west
  if (validNeighbour(lCurrentX - 1, lCurrentY )) lPossibleNextSteps.push_back(std::make_pair(lCurrentX - 1, lCurrentY ));

  //if there's no valid neighbours, modify aCurrent to be the top element from the stack and return
  //TODO check if this isn't just going to set it to the same value as before
  if (!lPossibleNextSteps.size())
  {
    mStack.pop_back(); // before this line, aCurrent should match the top element on the stack
    aCurrent = mStack.back();
    return;
  }

  else //choose one of the elements from the possibilities
  {
    aCurrent = lPossibleNextSteps.at(std::rand() % lPossibleNextSteps.size());
    //mark this place as visited
    mVisited[aCurrent.first][aCurrent.second] = true;

    //we can render a connection between the new aCurrent, at the location at the top of the stack


    //push this new location to the stack
    mStack.push_back(aCurrent);
    return;
  }
}

void Maze::rendermaze()
{
  StopWatch s = {"rendermaze"};
  
  //set mVisited
  std::vector<std::vector<bool>> mVisited (
  mBlocksX, std::vector<bool>(mBlocksY) 
  );

  

}