all:
	gcc server.c -lpthread -o server
	gcc client.c -lpthread -o client