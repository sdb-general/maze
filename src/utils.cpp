#include "../include/utils.hpp"

StopWatch::StopWatch( std::string message="")
	{
  std::cout << "+-----------------+\n";
	if (message == "") {std::cout << " Beginning counter\n";}
	else {std::cout << " Beginning " << message << std::endl; mMessage = message;}
  std::cout << "+-----------------+\n";
	mStartTime = std::chrono::high_resolution_clock::now();
	}
StopWatch::~StopWatch()
	{
  std::cout << "+-----------------+\n";
	std::cout << " Ending operation ";
	if (mMessage!="") std::cout << mMessage;
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - mStartTime);
	std::cout << ", time taken " << duration.count() << " milliseconds"<< std::endl;
  std::cout << "+-----------------+\n";
	}
