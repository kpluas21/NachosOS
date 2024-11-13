#ifndef MEMORY_H
#define MEMORY_H

#include "bitmap.h"

class MemoryManager{

    public:
        MemoryManager();
        ~MemoryManager();

        int AllocatePage(); // this will tell us which frame/physicalpage is available for the process
        int DeallocatePage(int which); //de allocate frame/physical page
        unsigned int GetFreePageCount(); 

    private:
        BitMap *bitmap;



};


#endif // MEMORY_H