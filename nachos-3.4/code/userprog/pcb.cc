#include "pcb.h"

PCB::PCB(int id) {

    
    pid = id;
    // Intialize the rest of the PCB object
    parent = NULL;
    children = new List();
    thread = NULL;

}

PCB::~PCB() {
    delete children;
}


void PCB::AddChild(PCB* pcb) {

    children->Append(pcb);
    
}

int PCB::RemoveChild(PCB* pcb) {
    return children->RemoveItem(pcb);
}