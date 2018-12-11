#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

struct Slice {
	int size;
	int start;
	int end;
	int* value;
	pthread_mutex_t mutex;
};

struct Slice* Slice_new(int size);
void Slice_push(struct Slice* slice, int a);
int Slice_pop(struct Slice* slice);
