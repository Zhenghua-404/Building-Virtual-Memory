struct Node{
    struct PCB* apcb;
    struct Node* next;
};
extern struct PCB* head;
extern struct PCB* tail;
extern struct Node* nhead;
extern struct Node* ntail;
extern FILE* ram2 [10];
void myinit(FILE* p);
int scheduler();

