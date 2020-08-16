all:
	gcc server.c -lpthread -lncurses -o server
	gcc client.c -lpthread -lncurses -o client