struct PCB{
    FILE* PC;
    char nameString[100];
    int pageTable[10];
    int PC_page;  //which page it is at now
    int PC_offset;  //which offset it is at now
    int pages_max;  //max page number
};
struct PCB* makePCB(FILE* p);
