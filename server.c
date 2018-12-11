// Server side C/C++ program to demonstrate Socket programming

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include "Slice.h"

#define PORT 8080

struct Slice* slice;

void* run(void* arg){

	while (1) {
        int client_socket = Slice_pop(slice);
        if (client_socket == -1)
        	continue;
        	
		char buffer[1024] = {0};
		char *hello = "Hello from server";
        // reads a buffer with maximum size 1024 from socket.
		int valread = read(client_socket, buffer, 1024);
		if (valread < 0) {
		perror("read");
		exit(EXIT_FAILURE);
		}
		printf("(s = %d) %s\n", valread, buffer);

		// writes to client socket
		send(client_socket, hello, strlen(hello), 0);
		printf("Hello message sent\n");
	}

	return 0;
}

/*int main(){
	struct Slice* slice = Slice_new(10);

	pthread_t t1, t2;

	pthread_create(&t1,NULL,run,(void*)slice);
	pthread_create(&t2,NULL,run,(void*)slice);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("Start: %d, End: %d\n", slice->start, slice->end);
}*/

int main(int argc, char const *argv[]) {
  slice = Slice_new(10);
  int threadCount = 4;
  int i;
  
  for (i = 0; i < threadCount; i++){
	  pthread_t t;
	  pthread_create(&t,NULL, run, NULL);
  }

  // creates socket file descriptor
  int server_fd;
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT); // host to network -- coverts the ending of the given integer
  const int addrlen = sizeof(addrlen);

  // binding
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // listening on server socket with backlog size 3.
  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  printf("Listen on %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

  // accepting client
  // accept returns client socket and fills given address and addrlen with client address information.
  
  
while (1) {
  int client_socket;
  if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }
  
printf("0\n");
  Slice_push(slice, client_socket);
  
  printf("Hello client %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

  

}
  return 0;
}
