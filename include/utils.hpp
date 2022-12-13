#include <iostream>
#include <chrono>


struct StopWatch
{
	StopWatch( std::string message );

	std::chrono::high_resolution_clock::time_point mStartTime;
	std::string mMessage;
	
	virtual ~StopWatch(); 
};