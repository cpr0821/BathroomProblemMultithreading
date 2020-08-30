#include "Driver.cpp"
#include "MyBathroomProblemSolver.cpp"
#include <iostream>
#include <string>
#include "Tester.cpp"

int main(int argc, char *argv[])
{
	if(argc < 5)
	{
		std::cout << "Too few arguments" << std::endl;
		return 0;
	}
	int n = std::stoi(argv[1]);//Number od toilets
	int men = std::stoi(argv[2]);//Number of men
	int women = std::stoi(argv[3]);//Number of women
	int iterations = std::stoi(argv[4]);//Number of iterations to arrive and leave the bathroom
	BathroomProblemSolverInterface* solver = new MyBathroomProblemSolver(n);
	Tester tester(solver, men, women, n);
	tester.begin(iterations);

	//Driver::drive(solver, men, women, iterations);

	return 0;
}
