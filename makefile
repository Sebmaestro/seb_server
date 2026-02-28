all:
	gcc -Wall -Wextra src/main.c src/net.c src/http.c -o sebServer

clean:
	rm -rf build