.PHONY:all

all:client server

client:tcp_client.c
	gcc -o $@ $^
server:tcp_server.c
	gcc -o $@ $^

.PHONY:clean

clean:
	rm -f client server
