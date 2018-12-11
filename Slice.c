
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct Slice {
	int size;
	int start;
	int end;
	int* value;
	pthread_mutex_t mutex;
};

void dump(int value){
	printf("Dumped: %d\n", value);
}

struct Slice* Slice_new(int size){
	struct Slice* slice = malloc(sizeof(struct Slice));
	slice->size = size;
	slice->start = slice->end = 0;
	slice->value = malloc(sizeof(int) * size);
	slice->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	return slice;
}

void Slice_push(struct Slice* slice, int a){

	pthread_mutex_lock(&slice->mutex);

	if ((slice->end + 1) % slice->size == slice->start){
		pthread_mutex_unlock(&slice->mutex);
		return;
	}

	slice->value[slice->end] = a;
	slice->end = (slice->end + 1) % slice->size;

	pthread_mutex_unlock(&slice->mutex);
	
printf("4\n");
}

int Slice_pop(struct Slice* slice){

	pthread_mutex_lock(&slice->mutex);

	if (slice->start == slice->end) {
		pthread_mutex_unlock(&slice->mutex);
		return -1;
	}

	int a = slice->value[slice->start];
	slice->start = (slice->start + 1) % slice->size;

	pthread_mutex_unlock(&slice->mutex);

	return a;
}
