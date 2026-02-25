all:
	gcc src/main.c src/net.c -o sebServer

clean:
	rm -rf build