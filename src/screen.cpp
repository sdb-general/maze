#include "../include/screen.hpp"
#include "../include/utils.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <queue>


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

  std::pair<int, int> lPrevious = aCurrent;

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

    //continue building representation of the maze
    modifyBlockList(lPrevious, aCurrent);

    //mark this place as visited
    mVisited[aCurrent.first][aCurrent.second] = true;

    //we can render a connection between the new aCurrent, at the location at the top of the stack
    renderFull(aCurrent, mStack.back());

    //push this new location to the stack
    mStack.push_back(aCurrent);
    return;
  }
}

void Maze::modifyBlockList(std::pair<int, int> aPrevious, std::pair<int, int> aCurrent)
{
  //get pointer to previous block
  Block* lPreviousBlockPtr = &( mMazeRep.at( aPrevious ) );

  // create a new block for the newly minted coord
  Block lNewBlock = {aCurrent.first, aCurrent.second, lPreviousBlockPtr};

  //modify the old block - amend its neighbour list
  
  // mMazeRep.at( aPrevious ) -> mNeighbours.push_back( &lNewBlock );
  mMazeRep.at( aPrevious ).mNeighbours.push_back( &lNewBlock );

  //add the newly minted block to our maze representation
  mMazeRep.insert( { aCurrent, lNewBlock } );

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

  //create a block for 0,0
  mMazeRep.insert( { lCurrent, Block(lCurrent) } );

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

//construct block with coords and default neighbour
Block::Block(const int& aX, const int& aY, Block* aPrevious):
  mX{aX}, mY{aY}, mNeighbours{aPrevious}
{}


Block::Block(std::pair<const int, const int>& aCoords, Block* aPrevious) :
  Block(aCoords.first, aCoords.second, aPrevious)
{}

Block::Block(int& aX, int& aY):
  mX{aX}, mY{aY}//, mNeighbours{aPrevious}
{}

Block::Block(std::pair<int, int>& aCoords) :
  Block(aCoords.first, aCoords.second)
{}

/*

  MAZE SOVING STUFF

*/


//SOLVER FUNCTION THAT RETURNS AN ORDERED LIST OF BLOCKS
//MAKE IT A MAZE MEMBER FUNCTION SO THAT WE HAVE ACCESS TO EVERYTHING
//WE ALREADY WORKED OUT 

std::vector<Block> Maze::aStarSolver(std::pair<int, int>& aStart, std::pair<int, int>& aGoal)
{
  //lambda to calc estimate of distance to cover
  auto heuristic = [](std::pair<int, int> aStart, std::pair<int, int> aEnd){ return (aEnd.first - aStart.first) + (aEnd.second - aStart.second); };

  //create openset, and 
  std::priority_queue<Block> lOpenSet;
  // blocks.insert( Block(aStart) );
}

