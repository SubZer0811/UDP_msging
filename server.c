#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<pthread.h>
#include <ncurses.h>
#include <unistd.h> 

int fd;
struct sockaddr_in server_socket_addr, client_sock_addr;
socklen_t add;

void listen_ (){
	
	char buf[20];
	for(int i=0; i<5; i){
		recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&client_sock_addr, &add);
		printw("\n[client]: %s\n", buf);
	}
}	

void type(){

	printw("\nHELP----------\nEnter /quit to end this session\n\n");
	printw("You are the server. Type your message and hit enter to send.\n");

	char str[100], str2[2];
	char ch;
	printw(">>");
    while (1) {
        if (kbhit()) {
			ch = getch();

			if(ch == '\n'){
				printw("[server]: %s\n>> ", str);
				if(strcmp(str, "/quit") == 0){
					close(fd);
					endwin();
					exit(0);
				}
				sendto(fd, str, sizeof(str), 0, (struct sockaddr *)&client_sock_addr, sizeof(client_sock_addr));
				str[0] = '\0';
			}
			else{
				str2[0] = ch;
				str2[1] = '\0';
				strcat(str, str2);
			}
            
        } else {
            sleep(0.01);
        }
    }
}

void init (){

	server_socket_addr.sin_family = AF_INET;
	server_socket_addr.sin_port = htons(9876);
	server_socket_addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(fd, (struct sockaddr*)&server_socket_addr, sizeof(server_socket_addr)) < 0){
		printw("Bind failed\n");
		exit(0);
	}
	printw("\nServer successfully created\n");
	pthread_t thread_id, thread_id1;
	
	printw("\n[.] Waiting for client ...\n");
	char buf[8];
	recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&client_sock_addr, &add);
	printw("[+] Client connected\n");
	refresh();
	pthread_create(&thread_id, NULL, listen_, NULL);
	pthread_create(&thread_id1, NULL, type, NULL);
	pthread_join(thread_id, NULL);

}

int kbhit()
{
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}

int main (){


	initscr();
    cbreak();
	// noecho();
    scrollok(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	printw(">>\n");
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(fd < 0){
		printf("Could not create socket\n");
		exit(0);
	}
	add = sizeof(client_sock_addr);

	init();

	close(fd);
	return 0;
}