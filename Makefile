server:
	gcc server.c -o prog -lrt
	./prog

client:
	gcc client.c -o prog -lrt
	./prog
