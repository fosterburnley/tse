CFLAGS=-Wall -pedantic -std=c11 -g -I../utils -L../lib
LIBS=-lutils -lcurl

pagetest:	pagetest.c 
					gcc $(CFLAGS) pagetest.c $(LIBS) -o $@

clean:
					rm -f *.o pagetest

