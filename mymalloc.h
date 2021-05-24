#define malloc(X) mymalloc(X, __FILE__, __LINE__)
#define free(X) myfree(X, __FILE__, __LINE__)

void *mymalloc(size_t length, char* file, int line);
void myfree(void* ptr, char* file, int line);

//typedef our memory pointer
typedef struct ptrNode{
  char* address;
  size_t length;
  int index;
  struct ptrNode* next;
}node_t;

//typedef a boolean F,T
typedef enum{F,T} boolean;
