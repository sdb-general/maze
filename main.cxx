#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
// #include <assert.h>
// #include "include/func.hpp"



#include "boost/program_options.hpp"
using namespace boost::program_options;
SDL_Event event;


const int screenBPP = 32;


int main(int argc, const char *argv[])
{
  // add programme options
  variables_map vm;
  try
  {
    
    options_description desc{"Options"};
    desc.add_options()

      ("sw", value<int>() -> default_value(1600), "Screen Width")
      ("sh", value<int>() -> default_value(900), "Screen Height")

      ;

    
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << "\n";
    abort();
  }

/*

SETTING CONFIGS -----------------------------------------------------------------------

*/

  const int screenWidth = vm["sw"].as<int>() ;
  const int screenHeight = vm["sh"].as<int>();


/*

SETTING   SCREEN -----------------------------------------------------------------------

*/


  // if(SDL_Init(SDL_INIT_VIDEO) != 0){
  //   std::cout << "couldn't init\n"; return 1;
  // }
  // SDL_Window* screen = SDL_CreateWindow("mandelbrot",
  //                                       SDL_WINDOWPOS_UNDEFINED,
  //                                       SDL_WINDOWPOS_UNDEFINED,
  //                                       screenWidth,screenHeight, 0);
  // assert(screen);
  // SDL_Renderer* lRenderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_SOFTWARE);
  // assert(lRenderer);
  // SDL_SetRenderDrawColor(lRenderer, 86, 29, 94, 0xFF); 
  // // SDL_SetRenderDrawColor(lRenderer, 0xFF, 0, 0, 255);
  // // SDL_RenderClear(renderer);
  // SDL_RenderPresent(lRenderer); //updates the window
  
/*

RENDERING -----------------------------------------------------------------------

*/


  // bool quit = false;
  // while (quit == false){
  //   while( SDL_PollEvent(&event) )
  //   {
  //     if (event.type == SDL_QUIT) quit = true;
  //   }
  // }
  
  // SDL_DestroyWindow(screen);
  // SDL_Quit();
  // return 0;
}