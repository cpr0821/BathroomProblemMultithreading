#include "BathroomProblemSolverInterface.cpp"
#include <atomic>
#include <cassert>
#include "Driver.cpp"
#include <iostream>

class Tester : public BathroomProblemSolverInterface
{
public:
	int factor, toilets;
	BathroomProblemSolverInterface* solution;
	int men;
	int women;
	
	std::atomic<int> counter;
	//counter is managed such that 
	//counter%factor = # of men in bathroom
	//counter/factor = # of women in bathroom 

	Tester(BathroomProblemSolverInterface* solution, int men, int women, int toilets)
	{
		this->factor = 10 + men * 10;// to avoid conflicts
		this->toilets = toilets;
		this->solution = solution;
		counter.store(0);
		this->men = men;
		this->women = women;
	}

	void begin(int iterations)
	{
		Driver::drive(this, men, women, iterations);
		std::cout << "Test successful\n";
	}

	void arriveAtTheBathroom(gender type) override
	{
		solution->arriveAtTheBathroom(type);
		int counter_value = counter.fetch_add(type*factor + (1-type));
		sanity_check(counter_value);
	}

	void leaveTheBathroom(gender type) override
	{
		int counter_value = counter.fetch_sub(type*factor + (1-type));
		solution->leaveTheBathroom(type);
		sanity_check(counter_value);
	}

	void sanity_check(int counter_value)
	{
		//int counter_value = counter.load();
		int men = counter_value % factor;
		int women = counter_value / factor;
		assert(men <= toilets);
	    assert(women <= toilets);
	    assert(men * women == 0);
	}
};
