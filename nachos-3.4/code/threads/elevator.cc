#include "copyright.h"
#include "system.h"
#include "synch.h"

int nextPErsonID = 1;

Lock *personIDLock = new Lock("PersonIDLock");

typedef struct Person
{
    int id;
    int atFloor;
    int toFloor;
} Person;

void ElevatorThread(int numFloors)
{
    printf("Elevator with %d floors was created\n", numFloors);
}

void Elevator(int numFloors)
{

    // create Elevator Thread
    Thread *t = new Thread("elevator thread");
    t->Fork(ElevatorThread, numFloors);
}

void PersonThread(int person)
{

    Person *p = (Person *)person;
    printf("person %d wants to go from  floor %d to %d\n", (int)p->id, p->atFloor, p->toFloor);
}

int getNextPersonID()
{
    return nextPErsonID++; // it will return current value and update the value by one for next person
}

void ArrivingGoingFromTo(int atFloor, int toFloor)
{

    // create Person struct

    Person *p = new Person;
    p->id = getNextPersonID();
    p->atFloor = atFloor;
    p->toFloor = toFloor;

    // Create Person Thread
    Thread *t = new Thread("person " + p->id);
    t->Fork(PersonThread, (int)p);
}