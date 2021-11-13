CFLAGS=-Wall -pedantic -std=c11 -g -I../utils -L../lib
LIBS=-lutils -lcurl

lindexer:		lindexer.c                                                                                                                                                                   
						gcc $(CFLAGS) lindexer.c $(LIBS) -o $@ -lpthread

indexer:		indexer.c
						gcc $(CFLAGS) indexer.c $(LIBS) -o $@

clean:
					rm -f *.o lindexer indexer

