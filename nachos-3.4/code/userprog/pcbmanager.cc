#include "pcbmanager.h"

PCBManager::PCBManager(int maxProcesses){

    bitmap = new BitMap(maxProcesses);
    pcbs = new PCB*[maxProcesses];
    for(int i = 0; i < maxProcesses; i++) {
        pcbs[i] = NULL;
    }
    pcbManagerLock = new Lock("PCB Manager Lock");


}

PCBManager::~PCBManager(){
    
    delete bitmap;
    delete pcbs;
}

PCB* PCBManager::AllocatePCB(){
    DEBUG('a', "in  AllocatePCB\n");
    pcbManagerLock->Acquire();
    DEBUG('a', "after acquiring the lock\n");
    int pid = bitmap->Find();
    DEBUG('a', "after getting the pid in the lock\n");
    pcbManagerLock->Release();
    DEBUG('a', "before assert\n");
    ASSERT(pid != -1);
    DEBUG('a', "before assigning pid\n");
    // printf("%d",pid);
    pcbs[pid] = new PCB(pid);
    return pcbs[pid];
    DEBUG('a', "after assigning pid\n");

}

int PCBManager::DeallocatePCB(PCB* pcb){

    //check if pcb is valid - check pcbs pid
    pcbManagerLock->Acquire();

    bitmap->Clear(pcb->pid);

    pcbManagerLock->Release();

    delete pcbs[pcb->pid];
    
}