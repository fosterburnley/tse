CFLAGS=-Wall -pedantic -std=c11 -g -I../utils -L../lib
LIBS=-lutils -lcurl

query: query.c
	gcc $(CFLAGS) query.c $(LIBS) -o $@

clean:
	rm -f *.o query
