#ifndef BPS_INTERFACE
#define BPS_INTERFACE
enum gender {MAN, WOMAN};//MAN=0, WOMAN=1

class BathroomProblemSolverInterface
{
public:
       	virtual void arriveAtTheBathroom(gender type) = 0;
	virtual void leaveTheBathroom(gender type)    = 0;
};
#endif
