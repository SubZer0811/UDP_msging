#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include <ncurses.h>
#include <unistd.h> 

int fd;
struct sockaddr_in client_socket_addr, other;
socklen_t add;

void listen_ (){
	
	char buf[20];
	for(int i=0; i<5; i){
		recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&other, &add);
		printw("\n[server]: %s\n", buf);
	}
}	

void type(){

	printw("\nHELP----------\nEnter /quit to end this session\n\n");
	printw("You are the client. Type your message and hit enter to send.\n");
	
		char str[100], str2[2];
	char ch;
	printw(">>");
    while (1) {
        if (kbhit()) {
			ch = getch();
            if(ch == '\n'){
				printw("[client]: %s\n>> ", str);
				if(strcmp(str, "/quit") == 0){
					close(fd);
					endwin();
					exit(0);
				}
				sendto(fd, str, sizeof(str), 0, (struct sockaddr *)&client_socket_addr, sizeof(client_socket_addr));
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

	client_socket_addr.sin_family = AF_INET;
	client_socket_addr.sin_port = htons(9876);
	client_socket_addr.sin_addr.s_addr = INADDR_ANY;
	
	sendto(fd, "12345678", sizeof("12345678"), 0, (struct sockaddr *)&client_socket_addr, sizeof(client_socket_addr));

    pthread_t thread_id, thread_id1;

	pthread_create(&thread_id, NULL, listen_, NULL);
	pthread_create(&thread_id1, NULL, type, NULL);
	pthread_join(thread_id, NULL);

}

int kbhit(void)
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
    scrollok(stdscr, TRUE);
	nodelay(stdscr, TRUE);

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	
	init();

	close(fd);

	return 0;
}