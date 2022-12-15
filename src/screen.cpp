#include "../include/screen.hpp"
#include "../include/utils.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include <mutex>

std::mutex mutex;

void blockRender( SDL_Renderer* aRenderer, int aTopLeftX, int aTopLeftY, int aXWidth, int aYWidth )
{
  // StopWatch s = {"rendering"};
  SDL_SetRenderDrawColor(aRenderer, std::rand() % 256, std::rand() % 256, std::rand() % 256, 0xFF);
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
  SDL_RenderPresent(aRenderer);
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
}

