#ifndef PRODUCERCONSUMER_H
#define PRODUCERCONSUMER_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <chrono>
#include "BoundedBuffer.h"

using namespace std;
typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;


// DO NOT modify any code here
void InitProducerConsumer(int p, int c, int psleep, int csleep, int items);
void *producer(void *);
void *consumer(void *);

#endif
