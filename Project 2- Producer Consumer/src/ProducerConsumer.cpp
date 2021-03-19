#include "ProducerConsumer.h"
typedef std::chrono::high_resolution_clock Time;
// typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> sec;

// TODO: add BoundedBuffer, locks and any global variables here
BoundedBuffer *buff;
int p, c, psleep, csleep;
// int c;
// int psleep;
// int csleep;
volatile int items2;
volatile int curitem;
volatile int consitems;
volatile int proditem;
volatile int toggle;
pthread_mutex_t prodcon_lock=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t prodcon_lock2= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t printlock= PTHREAD_MUTEX_INITIALIZER;

pthread_t * prodthreads;
pthread_t * consthreads;
ofstream file;
auto start= Time::now();



void InitProducerConsumer(int p, int c, int psleep, int csleep, int items) {
toggle=0;
proditem=0;
remove("output.txt");
start= Time::now();
toggle=1;

  //TODO: constructor to initialize variables declared
  //also see instruction for implementation
file.open("output.txt");
pthread_mutex_init(&prodcon_lock,NULL);
pthread_mutex_init(&prodcon_lock2,NULL);
pthread_mutex_init(&printlock,NULL);

buff= new BoundedBuffer(7);
curitem=0;
consitems=0;
p=p;
c=c;
psleep=psleep;
csleep=csleep;
items2=items;


//create an array for the producer threads
prodthreads= new pthread_t[p];
int * prodnumber= new int[p];
for(int i=0;i<p;i++){
  prodnumber[i]=i;
}
for(int i=0;i<p;i++){
  // cout<<"i made a prod"<<endl;
  pthread_create(&prodthreads[i],NULL,producer,(void*)(&prodnumber[i]));
}
//create an array for consumer threads

consthreads= new pthread_t[c];
int * consnumber= new int[c];
for(int i=0;i<c;i++){
  consnumber[i]=i;
}
for(int i=0;i<c;i++){
  // cout<<"i made a cons"<<endl;
  pthread_create(&consthreads[i],NULL,consumer,(void*)(&consnumber[i]));
}
//join on both sets of threads.

for(int i=0;i<p;i++){
  pthread_join(prodthreads[i],NULL);
}
  for(int i=0;i<c;i++){
  pthread_join(consthreads[i],NULL);
  }
// close the file and delete it
// std::cout << std::ifstream("output.txt").rdbuf();
file.close();

  
}


void* producer(void* threadID) {
int b=0;
while(1){
// sleep and create random integer
usleep(psleep*1000);

// check if it is possible for the thread to produce an item
pthread_mutex_lock(&prodcon_lock);
if(curitem==items2){
  int * p= NULL;
  pthread_mutex_unlock(&prodcon_lock);
  return p;
}else{
  b= curitem++;
}
pthread_mutex_unlock(&prodcon_lock);

int c = *((int *) threadID);
int a= rand();
buff->append(a);

auto elapsed= Time::now();
fsec fs=elapsed-start;
sec d= std::chrono::duration_cast<sec>(fs);


pthread_mutex_lock(&printlock);
file<<"Producer #"<<c<<", time = "<<d.count()<<", producing data item #"<<b<<", item value="<<a<<"\n";
pthread_mutex_unlock(&printlock);

}
}




void* consumer(void* threadID) {
  //TODO: consumer thread, see instruction for implementation

int a=0;
while(true){
usleep(csleep*1000);
pthread_mutex_lock(&prodcon_lock);
if(consitems==items2){
  int * p= NULL;
  pthread_mutex_unlock(&prodcon_lock);
  return p;
}else{
  a=consitems++;
   // cout<<a<<"consumer"<<endl;
}
pthread_mutex_unlock(&prodcon_lock);

int c = *((int *) threadID);
int rem= buff->remove();

auto elapsed= Time::now();
fsec fs=elapsed-start;
sec d= std::chrono::duration_cast<sec>(fs);


pthread_mutex_lock(&printlock);  
file<<"Consumer #"<<c<<", time = "<<d.count()<<", consuming data item with value="<<rem<<"\n";
pthread_mutex_unlock(&printlock);

}
}
