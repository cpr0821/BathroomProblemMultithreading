#ifndef DRIVER
#define DRIVER
#include "BathroomProblemSolverInterface.cpp"
#include <random>
#include <chrono>
#include <thread>
#include <vector>

class Driver
{
	static void run(BathroomProblemSolverInterface* solver, gender type, int iterations)
	{
		auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		std::mt19937_64 engine(seed);
		std::uniform_int_distribution<> dist(1, 100);
		for(int i = 0; i < iterations; i++)
		{
			solver->arriveAtTheBathroom(type);
			std::this_thread::sleep_for(std::chrono::milliseconds(dist(engine)));
			solver->leaveTheBathroom(type);
			std::this_thread::sleep_for(std::chrono::milliseconds(dist(engine)));
		}
	}

public:
	static void drive(BathroomProblemSolverInterface* solver, int men, int women, int iterations)
	{
		int i;
		std::vector<std::thread> threads;
		for(i = 0; i < men; i++)
		{
			threads.push_back(std::thread(&Driver::run, solver, MAN, iterations));
		}
		for(i = 0; i < women; i++)
		{
			threads.push_back(std::thread(&Driver::run, solver, WOMAN, iterations));
		}
		for(auto &t: threads)
		{
			t.join();
		}
	}
};
#endif
