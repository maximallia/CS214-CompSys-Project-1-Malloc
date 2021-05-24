#include <stdio.h>
#include <stdlib.h>
#include<ctype.h>
#include<errno.h>

#include "mymalloc.h"


//an example memory from lecture
//a static global variable is only available for this file
//start as zero initially
static char memory[4096];

node_t* header = NULL;

size_t memoryRemaining = 4096;//total:4096 and memory left


void reallocateMem(int flag);//calculate memory
void makeNode(node_t* temp, size_t length, int index);//create new Node
void* mymalloc(size_t length, char* file, int line);//return address for ptr
void myfree(void* ptr, char* file, int line);//no return needed,free memory


void reallocateMem(int flag){

  node_t* curr = header;
  size_t usedMem = 0;
  //size_t freedMem = 0; we thought of add freedMem to global memoryRemaing, but...
  //size_t tempMem = memoryRemaining;
  //after each workturn, the memory does not add up
  //^ this is caused by freeing
  //^^thus we decided to use 4096 to find remaing mem, instead of global memoryRemaining
  while(curr != NULL){
    usedMem += curr->length;
    curr = curr->next;
  }
  if(flag == 0){
    printf("Old Memory: %ld -> ", memoryRemaining);
    memoryRemaining = 4096 - usedMem;
  }
  if(flag==1){
    printf("Old Memory: %ld -> ", memoryRemaining);
    memoryRemaining =  4096 - usedMem;
  }

  //memoryRemaining = tempMem - usedMem;

  printf("New Memory: %ld\n", memoryRemaining);
  //return tempMem;
}

void makeNode(node_t* temp, size_t length, int index){

  temp->address = &memory[index];
  //&memory[index] points to memory[index]'s value, which is the address
  //we want the address to input into ptr
  //cannot use memory[index], error
  temp->length = length;
  temp->index = index;
  temp->next = NULL;
  
}


void* mymalloc(size_t length, char* file, int line)
{
  //you would need to put an actual implementation here
  //printf("name: %s, line: %d,  allocate %ld bytes\n", file, line, length);
  //printf("Byte 0 is %x\n", memory[0]);
  //some code dealing with memory...

  int index = 0;
  size_t memLen = length;
  node_t* curr = header;

  //no more memory error
  if(memLen > memoryRemaining){
    printf("ERROR: filename %s line %d, exceeds memory length. Left Mem: %ld,\
 Needed Mem: %ld\n", file, line, memoryRemaining, memLen);
    return NULL;//no address return
  }

   
  //empty mem, first mem block
  //printf("curr addr: %p\n",curr->address);
  if(curr == NULL ){
    //printf("curr is NULL, adding newNode\n");
    header = calloc(1, sizeof(node_t) );
    header->next = NULL;
    curr = header;

    node_t* newNode = calloc(1, sizeof(node_t));
    makeNode(newNode, memLen, index);
    curr->next = newNode;
    
    reallocateMem(0);
    return newNode->address;
    //ptr's address
  }
  //curr exists, but next is null
  //if next is null it would cause seg fault error if ignored
  else if(curr->next == NULL){
    //printf("curr:%p exists, next is NULL, adding Newnode after\n", curr->address);

    node_t* newNode = calloc(1, sizeof(node_t));
    makeNode(newNode, memLen, index);
    curr->next = newNode;
    
    reallocateMem(0);
    return newNode->address;
    //ptr's address
  }

  //two nodes exist, no seg fault
  //memory already mymalloced
  //a new condition: will usedMem+memLen exceed limit?
  //can't use memLen compare remainingMemory,
  else{
    boolean malloced = F;
    //malloced is to check if mymalloced has occured;
    //node_t* newNode = calloc(1,sizeof(node_t));
    //better to put newNode into while loop

    int usedMem = 0;
    int limit = 4096;
    //to check if usedMem would exceed 4096
    
    while(curr->next != NULL &&
	  (usedMem+memLen < limit) ){
      //curr!=NULL lead to error
      //usedMem<4096, to make sure there is still memory
      //usedMem will addup all memory in mem[]
      //so we won't used remainingMemory, instead we set a limit
      
      curr = curr->next;
      node_t* nextNode = curr->next;
      //check if null
      //int remainMem = 0;//used to iterate through mem.
      
      if(nextNode != NULL){//check for error, next node to curr can't be null
	//or else cause seg fault error
	
	int currEndMem = (nextNode->index) - (curr->index + curr->length);
	//curr->index+curr->length is the mem from curr's index to length
	// ^ which is the end of curr
	//nextNode->index = index of where the next node is
	//currEndMem = the space mem betw. end of curr and nextNode

	if(memLen <= currEndMem){
	  //if memLen <= remainMem that means enough space for mymalloc

	  node_t* newNode = calloc(1,sizeof(node_t));
	  index = curr->index + curr->length;
	  //our index for newNode

	  makeNode(newNode, memLen, index);
	  newNode->next = nextNode->next;
	  //malloced complete
	  
	  malloced = T;//record a complete mymalloc
	  reallocateMem(0);
	  return newNode->address;
	  //for ptr input
	}

      }
      //if not enough space between two blocks
      //we will add curr->length to usedMem to move to next block
      else{
	usedMem += curr->length;
	//also to make sure if usedMem <= remainingMemory
      }
      
    }

    //reached end without mallocing, we need to check if enough space is left
    if(malloced == F){

      if ( (curr->index+curr->length)+memLen <= limit){
	//check if enough space for adding newNode
	//limit=4096 is our total memory

	index = curr->index+curr->length;

	node_t* newNode = calloc(1,sizeof(node_t));
	makeNode(newNode, memLen, index);
	curr->next = newNode;
	//malloced complete
	
	malloced = T;
	reallocateMem(0);
	return newNode->address;
	//ptr address
      }

      //no mymalloced and no memory space, print ERROR
      printf("ERROR: File %s line %d, no memory space for length %ld\n", file, line, memLen);
      return NULL;
      
    }
  }
}
  

void myfree(void* ptr, char* file, int line){

  //reallocateMem(1);
  //it is a mess if we put it here, most likely need to put it after freeing
  //heh, bad idea lol
  
  if(ptr == NULL){
    printf("ERROR: File %s line %d freeing a NULL\n", file, line);
    return;
  }
    
  node_t* curr;
  node_t* preNode;
  node_t* post;
  
  curr = header->next;
  preNode = header;
  post = curr->next;

  boolean freed = F;
  //to record whether myfree has successfully freed pointer

  //printf("Entering myfree while loop\n");
  while(curr != NULL){
    //if curr->next!=NULL, the mem[] with only one mem block would
    //return error, don't want this
    
    if(curr->address == ptr){//addr found, free curr,pre->post
      preNode->next = post;
      printf("Freed address: %p\n", curr->address);
      
      //curr = NULL, seg error, alternative FOUND!!!
      //ptr is same address so making ptr NULL works
      ptr = NULL;
      //nulling should be similar to the original function free()
      //curr=NULL failed, ptr=NULL works
      
      freed = T;
      //myfree completed
      //printf("freeing ptr complete\n");

      break;
    }
    else if (curr->next == NULL) break;
    //needed to prevent seg error
    //it is here because we already checked if curr==ptr
    //not in the while-loop for one mem-block situations

    else {
      //if we are clear of curr->next==NULL, iterate
      preNode = preNode->next;
      curr = curr->next;
      post = post->next;
      //move everything to the next node
      //printf("iterate to next for myfree...\n");
    }
    
  }
  if(freed == T){
    //if myfree has freed a pointer
    reallocateMem(1);
    //calculate memory after freeing
    return;
  }
  //freed==F
  //freeing ptr did not occur,cannot find ptr -> no-malloced ptr
  printf("ERROR: File %s line %d freeing a not-malloced pointer\n", file, line);
  
  
}
