// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include <cstdio>
#include "synch.h"

// testnum is set in main.cc
int testnum = 1;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------
//HW1_LOCKS 
int SharedVariable;
Semaphore *sem = new Semaphore("Shared Variable Semaphore", 1);
Lock *loc = new Lock("Shared Variable Lock");
void SimpleThread(int which)
{
    int num, val;

    IntStatus oldLevel = interrupt->SetLevel(IntOff);
    for (num = 0; num < 5; num++) {
        #ifdef HW1_SEMAPHORES
        sem->P();
        #endif
        #ifdef HW1_LOCKS 
        loc->Acquire();
        #endif

        val = SharedVariable;
	printf("*** thread %d sees value %d\n", which, val);
        currentThread->Yield();
        SharedVariable = val+1;
        #ifdef HW1_SEMAPHORES
        sem->V();
        #endif
        #ifdef HW1_LOCKS 
        loc->Release();
        #endif
        currentThread->Yield();

        
        
        }
        
        #ifdef HW1_LOCKS 
        loc->Acquire();
        #endif
        #ifdef HW1_SEMAPHORES
        sem->P();
        #endif
        val = SharedVariable;
        printf("Thread %d sees final value %d\n", which, val);
        #ifdef HW1_SEMAPHORES
        sem->V();
        #endif
        #ifdef HW1_LOCKS 
        loc->Release();
        #endif

        (void) interrupt->SetLevel(oldLevel);
        }
    

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");
    
    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest(int n)
{
    for(int i=1; i<=n; i++){
        DEBUG('m', "Forking thread %d\n",i);
        char threadName[20];
        // std::string threadName = "forked thread " + std::to_string(i);
         snprintf(threadName, sizeof(threadName), "forked thread %d", i);
        //  printf(threadName);
        Thread *t = new Thread(threadName);
        // printf(t->getName());

        t->Fork(SimpleThread, i);
        DEBUG('m', "Forking thread %d\n complete",i);
    }
    SimpleThread(0);
    // switch (testnum) {
    // case 1:
	// ThreadTest1();
	// break;
    // default:
	// printf("No test specified.\n");
	// break;
    // }
}

