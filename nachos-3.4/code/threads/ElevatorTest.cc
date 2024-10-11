#include "copyright.h"
#include "system.h"
#include "synch.h"
#include "elevator.h"

void ElevatorTest(int numFloors, int numPersons) {

    if(numFloors <= 0)
    {
        printf("Invalid number of floors entered.\n");
        return;
    }
    // Create elevator thread
    Elevator(numFloors);

    for (int i = 0 ; i < numPersons; i++) {
        int atFloor = (Random() % numFloors) + 1; // choose a random atFloor
        int toFloor = -1 ;
        do {
            toFloor = (Random() % numFloors) + 1; // choose a random toFloor
        } while (atFloor == toFloor) ;

        ArrivingGoingFromTo(atFloor, toFloor);

        for(int i =0 ; i< 1000000; i++) {
            currentThread->Yield();
        }
    }

}