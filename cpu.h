struct CPU{
    FILE* IP;
    int offset;
    char IR[1000];
    int quanta;
}cpu;
int runCPU();
