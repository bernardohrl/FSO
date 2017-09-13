client:
	gcc client.c -o prog -lrt
	./prog

server:
	gcc server.c -o prog -lrt
	./prog
