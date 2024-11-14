// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "addrspace.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void doExit(int status){
    // DEBUG('a', "in do exit.\n");
    int pid = 99;
    printf("System Call: [%d] invoked [Exit]\n", pid);
    printf("Process [%d] exits with [%d]\n", pid, status);
    delete currentThread->space;
    currentThread->Finish();
}

void incrementPC() {
    int oldPCReg = machine->ReadRegister(PCReg);

    machine->WriteRegister(PrevPCReg,oldPCReg);
    machine->WriteRegister(PCReg,oldPCReg+4);
    machine->WriteRegister(NextPCReg, oldPCReg+8);
}

void childFunction(int pid) {
    currentThread->RestoreUserState();
    // int addr = machine->ReadRegister(PCReg);

    // Restore page table for child
    currentThread->space->RestoreState();
    // int pagetable = currentThread->space->GetNumPages();
    

    machine->Run();
}

int doFork(int functionAddr){

    // 1. check if sufficient memory exists to create new process
   if(currentThread->space->GetNumPages () <= mm->GetFreePageCount()) {
        DEBUG('a', "inside the if in doFork\n");
        // SaveUserState for the parent thread
        currentThread->SaveUserState();
        DEBUG('a', "after  saving state of current thread\n");
        //Create a new address space for child by copying parent address space
        AddrSpace* childAddrspace = new AddrSpace(currentThread->space);
        if(childAddrspace==NULL){
            DEBUG('a', "nooooo\n");
        }
        DEBUG('a', "after creating address space for child\n");
        // Create a new thread for the child and set its addrSpace
        Thread* childThread = new Thread("ChildThread");
        DEBUG('a', "after creating  thread for child\n");
        childThread->space = childAddrspace;
        DEBUG('a', "after mapping addrespace to thread\n");

        //Create a PCB for the child and connect it all up
        PCB* childpcb = pcbManager->AllocatePCB();
        DEBUG('a', "after allocating pcb to child\n");
        // printf("")
        childpcb->thread = childThread;
        DEBUG('a', "after mapping pcb to  with child threadchild\n");

        // set parent for child pcb
        childpcb->parent = currentThread->space->pcb;
        DEBUG('a', "after childpcb->parent = currentThread->space->pcb\n");
        // set child for parent pcb
        currentThread->space->pcb->AddChild(childpcb);
        DEBUG('a', "currentThread->space->pcb->AddChild(childpcb)\n");
        //initialize pcb in childAddSpace
        childThread->space->pcb = childpcb;
        DEBUG('a', "hildThread->space->pcb = childpcb;\n");

        // Set up machine register for child and save it to child thread
        machine->WriteRegister(PrevPCReg,functionAddr-4);
        machine->WriteRegister(PCReg,functionAddr);
        machine->WriteRegister(NextPCReg, functionAddr+4);
        DEBUG('a', "after writing all registers\n");
        childThread->SaveUserState();
        currentThread->RestoreUserState();
        childThread->Fork(childFunction, childpcb->pid);
        DEBUG('a', "after childThread->Fork\n");

        // Restore register state of parent user-level process
        int pcreg = machine->ReadRegister(PCReg);
        printf("Process [%d] Fork: start at address [%d] with [%d] pages memory\n",childpcb->pid, pcreg, currentThread->space->GetNumPages());

        return childpcb->pid;
        
   }
   else {
    return -1;
   }

}

void doYield() {
    currentThread->Yield();
}

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);  // register 2 contain number of sys call that is being invoke in user level program

    if ((which == SyscallException) && (type == SC_Halt)) {
	DEBUG('a', "Shutdown, initiated by user program.\n");
   	interrupt->Halt();
    } 
    else if ((which == SyscallException) && (type == SC_Exit)) {
        printf("System Call: [%d] invoked Exit\n",currentThread->space->pcb->pid);
        doExit(machine->ReadRegister(4));
    }
    else if ((which == SyscallException) && (type == SC_Yield)) {
        printf("System Call: [%d] invoked Yield\n",currentThread->space->pcb->pid);
        doYield();
        incrementPC();
    }
    else if ((which == SyscallException) && (type == SC_Fork)) {
        printf("System Call: [%d] invoked Fork\n",currentThread->space->pcb->pid);
        DEBUG('a', "fork, initiated by user program.\n");
        int ret = doFork(machine->ReadRegister(4));
        machine->WriteRegister(2, ret);
        incrementPC();
    }
    else {
	printf("Unexpected user mode exception %d %d\n", which, type);
	ASSERT(FALSE);
    }
}
