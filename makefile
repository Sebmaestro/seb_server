all:
	gcc src/main.c src/net.c src/http.c -o sebServer

clean:
	rm -rf build