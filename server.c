#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<pthread.h>

int fd;
struct sockaddr_in server_socket_addr, client_sock_addr;
socklen_t add;

void listen_ (){
	
	char buf[20];
	for(int i=0; i<5; i){
		recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&client_sock_addr, &add);
		printf("\n[client]: %s\n", buf);
		// printf("[server]: >> ");
	}
}	

void type(){

	printf("\nHELP----------\nEnter /quit to end this session\n\n");
	printf("You are the server. Type your message and hit enter to send.\n");

	for(int i=0; i<5; i){
		char buf[100];
		printf("[server]: >> ");
		fgets(buf, 100, stdin);
		if(strcmp(buf, "/quit\n") == 0){
			close(fd);
			exit(0);
		}else{
			sendto(fd, buf, sizeof(buf), 0, (struct sockaddr *)&client_sock_addr, sizeof(client_sock_addr));
		}
	}
}

void init (){

	server_socket_addr.sin_family = AF_INET;
	server_socket_addr.sin_port = htons(9876);
	server_socket_addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(fd, (struct sockaddr*)&server_socket_addr, sizeof(server_socket_addr)) < 0){
		perror("Bind failed\n");
		exit(0);
	}

	printf("Server successfully created\n");
	pthread_t thread_id, thread_id1;
	
	printf("\n[.] Waiting for client ...\n");
	char buf[8];
	recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&client_sock_addr, &add);
	printf("[+] Client connected\n");

	pthread_create(&thread_id, NULL, listen_, NULL);
	pthread_create(&thread_id1, NULL, type, NULL);
	pthread_join(thread_id, NULL);

}

int main (){

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(fd < 0){
		perror("Could not create socket\n");
		exit(0);
	}

	add = sizeof(client_sock_addr);
	init();

	close(fd);
	return 0;
}