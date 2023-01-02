// in this file we will 
#include <SDL2/SDL.h>
#include <vector>
#include <deque>
#include <map>

void blockRender( SDL_Renderer* aRenderer, int aTopLeftX, int aTopLeftY, int aXWidth, int aYWidth );

void populateBlocks( SDL_Renderer* aRenderer, int aBlocksX, int aBlocksY );

void populateBlocksWithBoundaries( SDL_Renderer* aRenderer, int aBlocksX, int aBlocksY );

struct Block
{
  //constuctors with coordinates
  Block(const int& aX, const int& aY, Block* aPrevious);
  Block(std::pair<const int, const int>& aCoords, Block* aPrevious);

  //constructor just for 0,0
  Block( int& aX, int& aY);
  Block(std::pair<int, int>& aCoords);

  //coordinates of our block location
  //useful to have, even though we will store
  //the coords as the keys in a std::map 
  const int mX;
  const int mY;

  //list of neighbours we can move to
  std::vector <Block*> mNeighbours; 

  //to add: some member functions to modify this list
};

class Maze
{
public:

  int mBlocksX;
  int mBlocksY;

  int mBlockWidth;
  int mBlockHeight;

  int mScreenWidth;
  int mScreenHeight;

  const int mBoundaryWidth = 2;

  SDL_Renderer* mRenderer;

  std::deque< std::pair< const int, const int>> mStack;

  std::vector<std::vector<bool>> mVisited;

  void rendermaze();

  //gets neighbour if one exists, or pops one off the stack
  void getNeighbour(std::pair<int, int>& aCurrent);

  bool validNeighbour(int aX, int aY);

  void renderFull ( std::pair<int, int> aBlock);
  void renderFull ( std::pair<int, int> aBlock1, std::pair<int, int> aBlock2);

  bool allVisited();

  void modifyBlockList(std::pair<int, int> aPrevious, std::pair<int, int> aCurrent);

  std::map< std::pair<int,int>, Block > mMazeRep;

  std::vector<Block> aStarSolver(std::pair<int, int>& aStart, std::pair<int, int>& aGoal);

  //constructor
  Maze(int aBlocksX, int aBlocksY, SDL_Renderer* aRenderer);


};


