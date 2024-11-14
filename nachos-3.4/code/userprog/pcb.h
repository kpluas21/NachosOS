#ifndef PCB_H
#define PCB_H
#include "list.h"
#include "thread.h"
// #include "pcbmanager.h"

class Thread;
// class PCBManager;
// extern PCBManager* pcbManager;
// #define "List.h"

class PCB {

    public:
        PCB(int id);
        ~PCB();
        int pid;
        PCB* parent;
        List* children; // keep track of children add or delete children
        Thread* thread; //control the execution of process for which this PCB is for
        void AddChild(PCB* pcb);
        int RemoveChild(PCB* pcb);

    private:
        
        // bool HasExisted();
        // void DeleteExitedChildrenSetParentNull();

};

#endif //PCB_H