all:
	gcc echoClient.c -o client
	gcc echoServer.c -o server

clean:
	rm -f client
	rm -f server