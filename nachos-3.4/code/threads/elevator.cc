#include "copyright.h"
#include "system.h"
#include "synch.h"
#include "elevator.h"
#include "unistd.h"


int nextPersonID = 1;
Lock *personIDLock = new Lock("PersonIDLock");


ELEVATOR *e;


void ELEVATOR::start() {

    while(1) {
       
        bool active = false; //set active to false
        for (int i = 0; i < numFloors; i++) {
            if (personsWaiting[i] > 0 || occupancy > 0) { //check if any active person or not
                active = true;
                break;
            }
        }

        if (!active){
            currentThread->Yield(); //if no active person yield the current thread
        }
        
        while(active){
            
            elevatorLock->Acquire(); //acquire the lock
            
            if (leaving[currentFloor-1]) { 
            leaving[currentFloor-1]->Broadcast(elevatorLock);
            // leaving[currentFloor-1]=0;
        }
        while (occupancy < maxOccupancy && personsWaiting[currentFloor-1] > 0) {
            entering[currentFloor-1]->Signal(elevatorLock);  // Signal one person to enter
            personsWaiting[currentFloor-1]--;  // Decrement waiting persons atFloor
            occupancy++;  // Increment occupancy
        }
        elevatorLock->Release(); 
        for(int j =0 ; j< 1000000; j++) {  //Spin for some time
                    currentThread->Yield();
                }
        // sleep(1);
        if (goingUp) {
            if (currentFloor < numFloors) {
                currentFloor++; 
                if(currentFloor==numFloors){
                    goingUp = false; 
                } // Move up
            } 
        } else {
            if (currentFloor > 1) {
                currentFloor--; 
                if(currentFloor==1){
                    goingUp = true; 
                }  // Move down
            } 
        }
        
        printf("Elevator arrives on floor %d\n", currentFloor);

        for (int i = 0; i < numFloors; i++) {  // check for active condition
            if (personsWaiting[i] > 0 || occupancy > 0) {
                active = true;
                break;
            }
            else {
                active=false;
            }
        }
        
        // if (!active){
        //     currentThread->Yield();
        // }
        
        }

        

      
    }
}

void ElevatorThread(int numFlors) {

    printf("Elevator with %d floors was created!\n", numFlors);

    e = new ELEVATOR(numFlors);

    e->start();


}

ELEVATOR::ELEVATOR(int numFlors) {
    currentFloor = 1;
    occupancy = 0;
    goingUp=true;
    this->numFloors=numFlors;
    maxOccupancy=5;
    entering = new Condition*[numFlors];
    leaving = new Condition*[numFlors];
    personsWaiting = new int[numFlors];
    elevatorLock = new Lock("ElevatorLock");

    for (int i = 0; i < numFloors; i++) {
        entering[i] = new Condition("Entering " + i); // Intialize entering condition
        leaving[i] = new Condition("Leaving " + i);  // Intialize Leaving condition
        personsWaiting[i] = 0;  // Initialize waiting count to 0
    }

    
}


void Elevator(int numFloors) {
    // Create Elevator Thread
    Thread *t = new Thread("Elevator");
    t->Fork(ElevatorThread, numFloors);
}


void ELEVATOR::hailElevator(Person *p) {
    
    elevatorLock->Acquire(); //Acquire elevatorLock;
    
    personsWaiting[p->atFloor-1]++; //Increment waiting persons atFloor
    while (currentFloor != p->atFloor) {
        entering[p->atFloor-1]->Wait(elevatorLock); //Wait for elevator to arrive atFloor
    }
    printf("Person %d got into the elevator.\n", p->id);
    personsWaiting[p->atFloor-1]--; //Decrement persons waiting atFloor
    occupancy++; //Increment persons inside elevator

    while (currentFloor != p->toFloor) {
        leaving[p->toFloor-1]->Wait(elevatorLock); //Wait for elevator to reach toFloor
    }
    

    
    // 9. Get out of the elevator
    printf("Person %d got out of the elevator.\n", p->id);
    // leaving[p->toFloor-1]--;
    // 10. Decrement persons inside elevator
    occupancy--;
    // 11. Release elevatorLock;
    elevatorLock->Release();
    // Move elevator in the current direction
    

}


void PersonThread(int person) {

    Person *p = (Person *)person;

    printf("Person %d wants to go from floor %d to %d\n", p->id, p->atFloor, p->toFloor);

    e->hailElevator(p);

}

int getNextPersonID() {
    int personID = nextPersonID;
    personIDLock->Acquire();
    nextPersonID = nextPersonID + 1;
    personIDLock->Release();
    return personID;
}


void ArrivingGoingFromTo(int atFloor, int toFloor) {


    // Create Person struct
    Person *p = new Person;
    p->id = getNextPersonID();
    p->atFloor = atFloor;
    p->toFloor = toFloor;

    // Creates Person Thread
    Thread *t = new Thread("Person " + p->id);
    t->Fork(PersonThread, (int)p);

}