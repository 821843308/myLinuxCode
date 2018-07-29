.PHONY:all

all:server client

server:udp_server.c
	gcc -o $@ $^

client:udp_client.c
	gcc -o $@ $^

.PHONY:clean
clean:
	rm -rf server client
