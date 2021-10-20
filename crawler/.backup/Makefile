CFLAGS=-Wall -pedantic -std=c11 -g -I../utils -L../lib
LIBS=-lutils -lcurl

crawler:	crawler.c 
					gcc $(CFLAGS) crawler.c $(LIBS) -o $@

clean:
					rm -f *.o crawler

