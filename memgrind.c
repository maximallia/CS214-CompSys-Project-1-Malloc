#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "mymalloc.h"

//these arrays are used to input the time for workloads
//at most 50 iterations
//just in case we make array limit too 100
int timeA[101];
int timeB[101];
int timeC[101];
int timeD[101];
int timeE[101];

int workTurn = 1;
//used to show what turn in printf.
//MAX=50 according to instructions

int workTime(void(*f)(), struct timeval start, struct timeval end, int total);
int aveTime(int* arr, int counter);
void clearArr(void* arr[], int totalIdx,int startIdx,int endIdx, char testId);
void makeArr(void* arr[], int totalIdx);
void makeArrInt(int arr[], int totalIdx);

//3 function below made specifically for testE, unfortunately, removeIdx's don't worked
//it has a major bug, causes segmentation fault
//due to time, we decided to change testE
//if we have more time the search and remove meory would be very interesting to take on
int searchArr(int arr[], int totalIdx, int targetByte);
void removeIdx(void* malloced[], int targetIdx, int totalIdx);
void removeIdxInt(int arr[], int targetIdx, int totalIdx);

void testA();
void testB();
void testC();
void testD();
void testE();

void workPrinter(int* arr, void(*f)(), struct timeval start, struct timeval end, int iteration);//compiles everything, array=time array, *f=test


int workTime(void(*f)(), struct timeval start, struct timeval end, int total){
  //gettimeofday(struct timeval * tv, struct timezone * tz)
  //tv is struct timeval, number of seconds and microseconds
  //tz is struct timezone

  time_t secDiff;//seconds difference between start and end
  suseconds_t usecDiff;//microseconds difference
  
  gettimeofday(&start, NULL);
  (*f)();//calls the function f, () is the input
  gettimeofday(&end, NULL);

  secDiff = end.tv_sec - start.tv_sec;
  usecDiff = end.tv_usec - start.tv_usec;

  //secDiff*1000000 to make it same unit as microsec usecDiff
  total += (secDiff*1000000) + usecDiff;

  return total;
    
}


int aveTime(int* arr, int counter){
  int i=0;
  int final, totalTime=0;
  while(arr[i]){
    totalTime+= arr[i];
    i++;
  }

  final = totalTime/counter;
  return final;
}


//malloc() 1 byte and immediately free it - do this 120 times
void testA(){
  //straightforward, just mymalloc and myfree the same index temp[0]
  int i;
  int repeat = 120;
  void* temp[121];

  makeArr(temp, repeat);
  
  for(i=0; i<repeat; i++){
    printf("Turn%d. Number %d: TestA begins...\n",workTurn, i+1);
    temp[0] = malloc(1); //malloc 1 byte
    printf("Malloced address: %p\n", temp[0]);
    //reallocateMem(0); bad IDEA!!
    //printf("malloc(1) ptr = %p\n", ptr);
    free(temp[0]);
    temp[0]=NULL;
    //reallocateMem(1); STILL BAD IDEA!!
    //printf("TestA freed\n\n");
    printf("\n\n");
  }
  //clearArr unncessary, mem cleared for sure
}


void testB(){
  //similar to testA, just move myfree to own for-loop
  int i;
  int repeat = 120;
  void* temp[121];

  makeArr(temp, repeat);
  
  for(i=0; i<repeat; i++){
    printf("Turn%d. Number %d: TestB mymalloc begins...\n",workTurn, i+1);
    temp[i] = malloc(1);
    printf("Malloced address: %p\n", temp[i]);
    printf("\n");
  }
  printf("\n");
  for(i=0; i<repeat; i++){
    printf("Turn%d. Number %d: TestB myfree begins...\n",workTurn, i+1);
    free(temp[i]);
    temp[i] = NULL;
    printf("\n");
  }
  //clearArr unnecessary, since we are sure mem is cleared
}


void clearArr(void* arr[], int totalIdx,int startIdx, int endIdx, char testId){
  //thought of using this to clear testC,D,E
  //arr is malloced[]
  //startIdx is where myfree left off on malloced[],
  //endIdx is where mymalloced stopped input address into malloced[]
  
  int i = 0;
  int count = 1;
  //count the number of clears we did
   
  for(i=startIdx; i < endIdx; i++){
    if(arr[i] != NULL){
      printf("There are malloced pointers left...\n");
      printf("Turn%d.%d: Clearing rest of Test%c...\n", workTurn,count,testId);
      free(arr[i]);

      arr[i] = NULL;
      printf("\n");
      count++;
    }
  }

}

void makeArr(void* arr[], int totalIdx){
  int i;

  for(i=0; i<totalIdx; i++){
    arr[i]=NULL;
  }
  
}

void makeArrInt(int arr[], int totalIdx){
  int i;

  for(i=0; i<totalIdx; i++){
    arr[i]=0;
  }

}


void testC(){

  int i, j = 0;
  //i for for-loop, j for malloced[] store addresses
  int k = 0;
  //k for freeing addressed from malloced

  int random;//using %2, 0=mymalloc, 1=myfree

  void* temp[241];//at most 240
  void* malloced[121]; //at most 120 mallocs
  //temp records 240 mallocs and frees,
  //mymalloc records malloced addresses

  int repeat = 240;
  int maxMac = 120;
  //240 repeats according to instruct.
  //at most 120 mallocs according to instruct.
  
  int mallocCount = 0;
  //mallocCount is count if mymalloced for 120
  //if 120 is reached, only myfree is processed

  int diff = 0;
  //used to record difference betw. malloc and free
  //if diff > 0, we free and k++
  //if diff<= 0, we free and no k
  
  
  makeArr(temp, repeat);
  //make temp filled with NULL;
  makeArr(malloced, maxMac);
  //make malloced filled with NULL
  
  for(i=0;i<240;i++){
    //while(i<240){
    random = rand()%2;
    //this would give 0 or 1
    //0:mymalloc
    //1:myfree
    
    if(random == 0 &&
       (mallocCount <120) ){
      //malloc and mallocCount is < 120
      printf("Turn%d. Number %d: TestC mymalloc Num%d begins...\n",workTurn, i+1, mallocCount+1);
      temp[i] = malloc(1);
      printf("Malloced address: %p\n", temp[i]);
      diff++;
      printf("\n");

      malloced[j] = temp[i];
      //printf("malloced[%d]: %p\n",j, malloced[j]);
      //printf("temp[%d]; %p\n",i, temp[i]);
      j++;
      //store address into malloced[]
      
      mallocCount++;
      
    }
    else if((random == 1 && diff>0) ||
	    (mallocCount >= 120) ){
      //myfree
      //and if mallocCount>=120, then only free is implememted
      // diff>0 mean there has been mymalloced
      
      
      printf("Turn%d. Number %d: TestC myfree begins...\n",workTurn, i+1);
      //printf("k: %d\n", k);
      //printf("malloced[%d]= %p\n", k, malloced[k]);
      free(malloced[k]);
      
      //printf("Freed address: %p\n", malloced[k]);
      malloced[k]= NULL;
      k++;
      diff--;
      //free address in malloced[k], then null malloced[k]
      //diff decreases, malloced frees one index
      printf("\n");
      //error occured...
      //need to make temp[241] all NULL first
      //if just temp[241]->seg fault
      //created makeArr()
    }
    else i--;
    //else is i-- because myfree and mymalloc did not occur
    //run this loop again
  }

  clearArr(malloced, repeat,k, j, 'C');
  //safety precaution, incase error does happen
  //so then error in testC wont affect other workload
  //k is where myfree left off in malloced
  //j is where mymalloc last input index in malloced
  //'C' is for printf so we know testC is being freed
  
}

//search array for target value (ptr address), returns target's index
//if not found returns -1000, meaning ptr not in malloced[]
int searchArr(int arr[], int totalIdx, int targetByte){
  int i = 0;

  for(i=0; i<totalIdx; i++){
    if(arr[i] == targetByte){
      return i;
    }
  }

  return -1000;

}

//removes idx's element by moving all values to the left
void removeIdx(void* malloced[], int targetIdx, int totalIdx){
  int i=0;
  for(i = targetIdx; i< totalIdx-1; i++){
    malloced[i] = malloced[i+1];
  }

}

//void* is not int, we needed an function for int arr
//do not know how to have a function to accept all array types
//made another function...the not so smart way...please don't judge...
void removeIdxInt(int arr[], int targetIdx, int totalIdx){
  int i=0;
  for(i = targetIdx; i< totalIdx-1; i++){
    arr[i] = arr[i+1];
  }
}


//testD is very similar to testC, randomly malloc or free
// but unlike testC, testD can not only malloc 1 byte, but also to 100
//thus for testD, we had to create an integer array to store memory used
//since the memory are not the same
//each time myfree is implemented, it will release the oldest memory in memory array
void testD(){
  //structure similar to testC
  //except input ptr wont be 1, but random num betw. 1~100
  
  int i,j = 0;
  int k = 0;
  //for testD, k was not used
  
  int random = 0;
  int randNum = 0;
  //randNum is rand()%100 + 1

  //int randIdx = 0;
  //random idx to free in malloced
  //with removeIdx
  void* temp[121];
  void* malloced[121];

  int repeat = 120;
  int maxMac = 120;

  //int mallocCount = 0;
  //int diff = 0;
  //testC's mallocCount and diff won't work
  //the ptr bytes are not 1
  
  makeArr(temp, repeat);
  makeArr(malloced, maxMac);

  int limit = 4096;
  int usedMem = 0;
  int memByte[121];
  //using these as replacement for diff and mallocCount
  //memByte stores all mem used by mymalloc
  
  for(i=0; i< repeat; i++){
    //printf("Before rand");
    randNum = rand()%100+1;//1~100 random bytes
    random = rand()%2;//0 or 1
    //printf("after random");
    
    if(random == 0){
      if(usedMem + randNum < limit){
	printf("Turn%d. Number %d: TestD mymallocing %d byte memory begins...\n",workTurn,i+1, randNum);

	temp[i] = malloc(randNum);

	usedMem += randNum;
	memByte[j] = randNum;
	malloced[j] = temp[i];
	printf("Malloced address: %p\n", temp[i]);
	j++;
	printf("\n");
      }
      else i--;
    }
    
    else if(random == 1){
      if(usedMem>0){
	//there has been mymalloced before
	printf("Turn%d. Number %d: TestD myfree begins...\n",workTurn, i+1);
	free(malloced[k]);
	malloced[k] = NULL;
	
	usedMem -= (int) memByte[k];
	k++;
	printf("\n");
      }
      else i--;
     
    }
      
  }
  clearArr(malloced, maxMac, k, j, 'D');
  //unlike testC, it is not garanteed there won't be memory left since memories are not the same
  //so we must run clearArr, so next workload wont be affected
}

//after a failed attempt of using searchArr and removeIdx, we decided to use a simpler testE
//testE did not use rand()
//it starts with mymallocing a 1 byte, then continues to mymalloc
//with byte+23 for each iteration
//when the byte exceeds limit which is 4096, myfree would be implemented
//myfree will continue, until memory memory returns to 0
//then there would be a switch back to mymalloc and byte would start from 1
void testE(){

  void* temp[501];
  void* malloced[501];
  int memByte[501];
  int repeat = 500;
  
  makeArr(temp, repeat);
  makeArr(malloced, repeat);
  makeArrInt(memByte, repeat);
  
  int usedMem = 0;
  int limit = 4096;

  int numByte = 1;
  //used to implement how much memory we add
  int i,j = 0;
  //i for temp, j for malloced
  int k = 0;
  //for membyte[]
  boolean reverse = F;
  //identify if it is mymalloc(F) or myfree(T)
  for(i=0; i< repeat; i++){
    if( reverse == F ){

      printf("Turn%d. Number %d: TestE mymallocing %d byte memory begins...\n",workTurn,i+1, numByte);
      
      temp[i] = malloc(numByte);
      malloced[j] = temp[i];
      memByte[j] = numByte;
      
      printf("Malloced address: %p\n", temp[i]);
      
      j++;
      
      usedMem += numByte;
      numByte += 23;
      
      if (usedMem+numByte > limit){
	//this mean the next iteration would exceed limit, causing error
	//so to prevent this we change reverse to T, starting myfree function
	reverse = T;
	numByte -= 23;
      }
      printf("\n");
    }

    else if(reverse == T &&
	    malloced[k]!=NULL){
      //we added malloced[k]!=NULL in case k exceeds j
      //when that happens go to else, though k exceeds j, but the next mymalloc loop
      //would ensure j exceeds k, so no segmentation fault
      printf("Turn%d. Number %d: TestE myfree begins...\n",workTurn, i+1);
      
      free(malloced[k]);
      malloced[k] = NULL;

      usedMem -= memByte[k];
      
      k++;
      printf("\n");
    }

    else{//when myfree reaches the end where usermem can't go any lower
      reverse = F;
      numByte = 1;
    }//restarts loop
    
  }

  clearArr(malloced, repeat, k, j, 'E');
  //same reasons as D, because the memory used each time is not the same, there would be memroy
  //left in the malloced[], so we must implement clearArr to clear the malloced, so the next
  //workload won't be affected
}



void workPrinter(int* arr, void(*f)(), struct timeval start, struct timeval end, int iteration){
  
  //int arr[4096];
  //int iteration = 50;
  int timetemp = 0;
  int total = 0;
  int i,j = 0;

  for(i = 0; i < iteration; i++){
                                                                         
    //printf("%d turn: \n", i+1);
    timetemp = workTime((*f), start, end, total);
    //printf("workTime complete %d\n", i+1);
    arr[j] = timetemp;
    j++;

    workTurn++;
    //shows what turn it is for workload
    //50 in total according to instruct.
  }
  workTurn = 1;
  //rewind workTurn
  
  //return arr;
  //c can't return arrays...
  //made the funct() as void like createNode()
}


int main()
{
  //int total=0;
  //int timeA[4096];//array for work time A
  //int timeB[4096];
  //we designed time arrays as global var
  
  struct timeval start, end; //for time ave

  //stores ave time for work
  int finalA = 0;
  int finalB = 0;
  int finalC = 0;
  int finalD = 0;
  int finalE = 0;
  
  int iteration = 50;
    
  //timeA *******PASSED!!!!!!******
  workPrinter(timeA, testA, start, end, iteration);
  
  //timeB *********PASSED!!!!!******
  workPrinter(timeB, testB, start, end, iteration);

  //timeC *********PASSED!!!!!******  
  workPrinter(timeC, testC, start, end, iteration);

  //timeD *********PASSED!!!!********
  workPrinter(timeD, testD, start, end, iteration); 

  //timeE
  workPrinter(timeE, testE, start, end, iteration);
  
  finalA = aveTime(timeA, iteration);
  printf("The mean time for testA: %d microsecs\n", finalA);

  finalB = aveTime(timeB, iteration);
  printf("The mean time for testB: %d microsecs\n", finalB);

  finalC = aveTime(timeC, iteration);
  printf("The mean time for testC: %d microsecs\n", finalC);

  finalD = aveTime(timeD, iteration);
  printf("The mean time for testD: %d microsecs\n", finalD);     

  finalE = aveTime(timeE, iteration);
  printf("The mean time for testE: %d microsecs\n", finalE);
  
  return EXIT_SUCCESS;
}
