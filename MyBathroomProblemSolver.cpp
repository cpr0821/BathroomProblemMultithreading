#include "BathroomProblemSolverInterface.cpp"
#include <stdexcept>
#include <semaphore.h>
#include <pthread.h>
#include <string>
#include <algorithm> //min
#include <fcntl.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

// Man = 0, Woman = 1

class MyBathroomProblemSolver : public BathroomProblemSolverInterface
{
	int NumberOfToilets;//Number of toilets
  int SwitchThreshold; //the threshold for moving to the next phase
  gender turn; //the gender in the bathroom currently (either woman or man)
  int waiting[2] = {0,0}; //the number of people of each gender waiting to use
  int completed = 0; //the number of people who have left in the current phase
  int inside = 0; //number of people inside
  sem_t safeToUseMen; //semaphore for waiting to enter the bathroom
  
  sem_t safeToUseWomen; //semaphore for waiting to enter the bathroom
  
  sem_t mutex;
     //semaphore for mutual exclusion
  
  public:
	MyBathroomProblemSolver(int n)
	{
		this->NumberOfToilets = n;
    this->SwitchThreshold = (2 * n);
    sem_init (&safeToUseMen, 0, 0);
    sem_init (&safeToUseWomen, 0, 0);
    sem_init (&mutex, 0, 1);
    
	}
	void arriveAtTheBathroom(gender type) override
	{
    sem_wait(&mutex); // wait to enter the critical section
    waiting[type] = waiting[type] + 1; //increment the number of waiters for the gender
    
    // check if can enter the bathroom immediately
    if (!(OkayToEnter(type) || OkayToSwitch(type))){
      sem_post(&mutex); //leave the critical section
      if(type == MAN){
        sem_wait(&safeToUseMen); // wait until it is appropriate to use the bathroom
      }else if(type == WOMAN){
        sem_wait(&safeToUseWomen);
      }
    }else{
      if(turn != type) // my gender is incompatible with current phase
        SwitchGender(type); //switch the gender type so they can use the bathroom
      waiting[type] = waiting[type] - 1; //decrement number of waiters
      inside++; // increment number of people inside
      sem_post(&mutex); // leave the critical section
    }
    return; //can enter the bathroom now
	}

	void leaveTheBathroom(gender type) override
	{
		sem_wait(&mutex); //wait to enter the critical section
    inside--; // decrement number of people inside
    completed++; // increment number of people who have left
    
    if (OkayToEnter(type)){
      waiting[type] = waiting[type] - 1; //decrement number of waiters
      inside++; //increment number of occupents
      if(type == MAN){
        sem_post(&safeToUseMen); // wait until it is appropriate to use the bathroom
      }else if(type == WOMAN){
        sem_post(&safeToUseWomen);
      } //wake up a person of my gender
    }else if((type == WOMAN && OkayToSwitch(MAN)) || (type == MAN && OkayToSwitch(WOMAN) )){
      if(type == WOMAN){
        SwitchGender(MAN);
        // calculate the number of the other gender to wake up
        inside = min(NumberOfToilets, waiting[MAN]); // set the number of occupants
        waiting[MAN] = waiting[MAN]-inside; //adjust the number of waiters
        for(int i = 0; i < inside; i++){ // wake up people of other gender
          sem_post(&safeToUseMen);
        }  
      } else if(type == MAN){
        SwitchGender(WOMAN);
        // calculate the number of the other gender to wake up
        inside = min(NumberOfToilets, waiting[WOMAN]); // set the number of occupants
        waiting[WOMAN] = waiting[WOMAN]-inside; //adjust the number of waiters
        for(int i = 0; i < inside; i++){ // wake up people of other gender
          sem_post(&safeToUseWomen);
        }  
      }
    }
    sem_post(&mutex); //leave the critical section
    
    return;
	}
 
  /* test whether it is okay to allow a waiting person to use the restroom, returns false if 
  bathroom is full or no more people of the current gender will be allowed*/
  bool OkayToEnter(gender type){
    if((turn == type) && (waiting[type] > 0) && (inside < NumberOfToilets)
     && ((inside + completed < SwitchThreshold) || (waiting[1-type] == 0)))
      return true;
    else
      return false;
  }
  
  // test where it is okay to switch genders or change phases
  bool OkayToSwitch(gender type){
    if((turn != type) && (inside == 0) && (waiting[type] > 0) && 
    ((inside + completed >= SwitchThreshold) || (waiting[1-type] == 0))) 
      return true;
    else
      return false;
  }
  
  // switch the genders and go to next phase
  void SwitchGender(gender type){
    completed = 0; // reset the count
    turn = type; // switch the gender
    
    return;
  }
};
