#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "copyright.h"

struct PersonThread {
    int id;
    int atFloor;
    int toFloor;
};

struct ElevatorThread {
    int numFloors;
    int currentFloor;
    int numPeopleIn;
};

void Elevator(int numFloors);
void ArrivingGoingFromTo(int atFloor, int toFloor);

#endif