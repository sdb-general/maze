#include "../include/screen.hpp"
#include "../include/utils.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>


// const int boundaryWidth = 1;

void blockRender( SDL_Renderer* aRenderer, int aTopLeftX, int aTopLeftY, int aXWidth, int aYWidth )
{
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
  // SDL_SetRenderDrawColor(aRenderer, 86, 29, 94, 0xFF);
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

void Maze::renderFull ( std::pair<int, int> aBlock )
{
  //render a block with boundaries
  int lXindex = aBlock.first;
  int lYindex = aBlock.second;

  int lTopLeftX = mBoundaryWidth + lXindex * ( mBlockWidth + mBoundaryWidth );
  int lTopLeftY = mBoundaryWidth + lYindex * ( mBlockHeight + mBoundaryWidth );

  blockRender ( mRenderer, lTopLeftX, lTopLeftY, mBlockWidth, mBlockHeight );
}

void Maze::renderFull ( std::pair<int, int> aBlock1, std::pair<int, int> aBlock2)
{
  // perform same function as above but colour in the boundary
  renderFull(aBlock1);//; renderFull(aBlock2);
  // std::this_thread::sleep_for(std::chrono::milliseconds(100));

  int upper;
  int lTopLeftX, lTopLeftY;

  //work out the top left corner of the boundary between them

  if (aBlock1.first == aBlock2.first) // they differ in second coordinate
  {
    int upper = std::max(aBlock1.second, aBlock2.second);
    //use the top left calculation for the one with the largest y coordinate
    lTopLeftX = mBoundaryWidth + aBlock1.first * ( mBlockWidth + mBoundaryWidth ) ;//-1 ; //move to the left to cover the boundary
    lTopLeftY = mBoundaryWidth + upper * ( mBlockHeight + mBoundaryWidth ) - mBoundaryWidth;

    blockRender(mRenderer, lTopLeftX, lTopLeftY, mBlockWidth, mBoundaryWidth );
    return;
  }
  if (aBlock1.second == aBlock2.second) // they differ in first coordinate
  {
    int upper = std::max(aBlock1.first, aBlock2.first);
    //use the top left calculation for the one with the largest y coordinate
    lTopLeftX = mBoundaryWidth + upper * ( mBlockWidth + mBoundaryWidth ) - mBoundaryWidth ; //move to the left to cover the boundary
    lTopLeftY = mBoundaryWidth + aBlock1.second * ( mBlockHeight + mBoundaryWidth ) ;// + 1;

    blockRender(mRenderer, lTopLeftX, lTopLeftY, mBoundaryWidth, mBlockHeight );
    return;
  }



}

Maze::Maze(int aBlocksX, int aBlocksY, SDL_Renderer* aRenderer) : 
  mBlocksX{aBlocksX}, mBlocksY{aBlocksY}, mRenderer{aRenderer}
{
  SDL_GetRendererOutputSize(mRenderer, &mScreenWidth, &mScreenHeight);
  // std::cout << "address of mRenderer is " << mRenderer << "\n";
  mBlockWidth = mScreenWidth / mBlocksX - mBoundaryWidth;
  mBlockHeight = mScreenHeight / mBlocksY - mBoundaryWidth;

  //set mVisited
  mVisited = std::vector<std::vector<bool>> ( mBlocksX, std::vector<bool>(mBlocksY) ); 

}

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

  std::vector< std::pair< int, int > > lPossibleNextSteps;

  //check north
  if (validNeighbour(lCurrentX, lCurrentY - 1 )) lPossibleNextSteps.emplace_back(std::make_pair(lCurrentX, lCurrentY - 1));

  //check east
  if (validNeighbour(lCurrentX + 1, lCurrentY )) lPossibleNextSteps.emplace_back(std::make_pair(lCurrentX + 1, lCurrentY ));
  
  //check south
  if (validNeighbour(lCurrentX, lCurrentY + 1 )) lPossibleNextSteps.emplace_back(std::make_pair(lCurrentX, lCurrentY + 1 ));
  
  //check west
  if (validNeighbour(lCurrentX - 1, lCurrentY )) lPossibleNextSteps.emplace_back(std::make_pair(lCurrentX - 1, lCurrentY ));

  //if there's no valid neighbours, modify aCurrent to be the top element from the stack and return
  //TODO check if this isn't just going to set it to the same value as before
  if (!lPossibleNextSteps.size())
  {
    mStack.pop_back(); // before this line, aCurrent should match the top element on the stack

    aCurrent = mStack.back();
    return;
  }

  else 
  {
    //choose one of the elements from the possibilities
    aCurrent = lPossibleNextSteps.at(std::rand() % lPossibleNextSteps.size());

    //mark this place as visited
    mVisited[aCurrent.first][aCurrent.second] = true;

    //we can render a connection between the new aCurrent, at the location at the top of the stack
    renderFull(aCurrent, mStack.back());

    //push this new location to the stack
    mStack.push_back(aCurrent);
    return;
  }
}

bool Maze::allVisited()
{
  for (int i = 0; i < mBlocksX; i++)
  for (int j = 0; j < mBlocksY; j++)
  {
    if (!mVisited[i][j]) return false;
  }
  return true;
}

void Maze::rendermaze()
{
  StopWatch s = {"rendermaze"};
  
  std::pair<int, int> lCurrent = {0,0};
  mVisited[0][0] = 1;
  mStack.push_back(lCurrent);

  int updateEveryFrame = 0;

  while (!allVisited())
  // while (!mStack.empty())
  {
    //move on to next neighbour
    getNeighbour(lCurrent);

    if ((updateEveryFrame % 5) == 0) SDL_RenderPresent(mRenderer); //update every few operations

    updateEveryFrame++;
  }
  SDL_RenderPresent(mRenderer);
}