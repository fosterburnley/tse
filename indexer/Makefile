CFLAGS=-Wall -pedantic -std=c11 -g -I../utils -L../lib
LIBS=-lutils -lcurl

indexer:	indexer.c 
					gcc $(CFLAGS) indexer.c $(LIBS) -o $@

clean:
					rm -f *.o indexer

