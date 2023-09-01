CFLAGS = -g -Wall -o

cshell: cshell
	gcc $(CFLAGS) cshell cshell.c

clean:
	rm -f cshell
	rm -f all *.o
