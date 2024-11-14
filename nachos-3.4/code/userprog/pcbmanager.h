#ifndef PCBMANAGER_H
#define PCBMANAGER_H

#include "bitmap.h"
#include "pcb.h"
#include "synch.h"

class PCB;


class PCBManager {

    public:
        PCBManager(int maxProcesses);
        ~PCBManager();

        PCB* AllocatePCB();
        int DeallocatePCB(PCB* pcb);

    private:
        BitMap* bitmap;
        PCB** pcbs;
        Lock* pcbManagerLock;

};



#endif // PCBMANAGER_H