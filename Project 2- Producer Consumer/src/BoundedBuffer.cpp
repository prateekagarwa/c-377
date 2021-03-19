#include "BoundedBuffer.h"

  volatile int *buffer;
  volatile int buffer_size;
  volatile int buffer_cnt;
  volatile int buffer_first;
  volatile int buffer_last;

  pthread_mutex_t buffer_lock= PTHREAD_MUTEX_INITIALIZER;  // lock
  pthread_cond_t buffer_not_full= PTHREAD_COND_INITIALIZER ;   // Condition indicating buffer is not full
  pthread_cond_t buffer_not_empty= PTHREAD_COND_INITIALIZER;  // Condition indicating buffer is not empty

BoundedBuffer::BoundedBuffer(int N) {
  //TODO: constructor to initialize all the variables declared in BoundedBuffer.h
  buffer= new int[N];
  buffer_size=N;
  buffer_cnt=0;
  buffer_first=0;
  buffer_last= 0;
  pthread_mutex_init(&buffer_lock,NULL);
  pthread_cond_init(&buffer_not_full,NULL);   
  pthread_cond_init(&buffer_not_empty,NULL); 
  
}

BoundedBuffer::~BoundedBuffer() {
  //TODO: destructor to clean up anything necessary
delete [] buffer;
buffer=NULL;

}

void BoundedBuffer::append(int data) {
  
  //TODO: append a data item to the circular buffer
  pthread_mutex_lock(&buffer_lock);
  
  while(buffer_cnt==buffer_size){
   
  pthread_cond_wait(&buffer_not_full,&buffer_lock);
  }
  buffer[buffer_last]=data;
  buffer_last=(buffer_last+1)%buffer_size;
  buffer_cnt++;
  pthread_cond_signal(&buffer_not_empty);
  pthread_mutex_unlock(&buffer_lock);

}

int BoundedBuffer::remove() {
int temp;
  
//TODO: remove and return a data item from the circular buffer
pthread_mutex_lock(&buffer_lock);

while(buffer_cnt==0){
  
   pthread_cond_wait(&buffer_not_empty,&buffer_lock);
  }
  temp= buffer[buffer_first];
  buffer_first=(buffer_first+1)%buffer_size;
  buffer_cnt--;
  
pthread_cond_signal(&buffer_not_full);
pthread_mutex_unlock(&buffer_lock);
return temp;
}

bool BoundedBuffer::isEmpty() {
  //TODO: check is the buffer is empty
  if (buffer_cnt==0) return true;
  return false;
}
  