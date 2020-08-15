#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>

int fd;
struct sockaddr_in client_socket_addr, other;
socklen_t add;

void listen_ (){
	
	char buf[20];
	for(int i=0; i<5; i){
		recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&other, &add);
		printf("\n[server]: %s\n", buf);
	}
}	

void type(){

	printf("\nHELP----------\nEnter /quit to end this session\n\n");
	printf("You are the client. Type your message and hit enter to send.\n");
	char buf[100];

	for(int i=0; i<5; i){
		printf("[client]: >> ");
		fgets(buf, 100, stdin);
		if(strcmp(buf, "/quit\n") == 0){
			close(fd);
			exit(0);
		}else{
			sendto(fd, buf, sizeof(buf), 0, (struct sockaddr *)&client_socket_addr, sizeof(client_socket_addr));
		}
	}
}

void init (){

	client_socket_addr.sin_family = AF_INET;
	client_socket_addr.sin_port = htons(9876);
	client_socket_addr.sin_addr.s_addr = INADDR_ANY;
	
	sendto(fd, "12345678", sizeof("12345678"), 0, (struct sockaddr *)&client_socket_addr, sizeof(client_socket_addr));

    pthread_t thread_id, thread_id1;

	// listen_();
	pthread_create(&thread_id, NULL, listen_, NULL);
	pthread_create(&thread_id1, NULL, type, NULL);
	pthread_join(thread_id, NULL);

}
int main (){

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	
	init();

	close(fd);

	return 0;
}