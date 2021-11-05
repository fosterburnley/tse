CFLAGS=-Wall -pedantic -std=c11 -g -I../utils -L../lib
LIBS=-lutils -lcurl

querier:	query.c 
					gcc $(CFLAGS) query.c $(LIBS) -o $@

clean:
					rm -f *.o query

