#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "copyright.h"

struct PersonThread;
struct ElevatorThread;

void Elevator(int numFloors);
void ArrivingGoingFromTo(int atFloor, int toFloor);

#endif