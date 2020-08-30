# BathroomProblemMultithreading
This program uses semaphores to switch the gender allowed in a bathroom. 

Group Members:
Camryn Rogers
Orlando Garcia

Compile:

g++ -std=c++11 -g -o main main.cpp -pthread -lrt

Run: 

./main [arg1] [arg2] [arg3] [arg4] 

arg1 = number of toilets

arg2 = number of men

arg3 = number of women

arg4 = number of iterations

it works with -lpthred as well
