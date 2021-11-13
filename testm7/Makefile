CFLAGS=-Wall -pedantic -std=c11 -g -I../utils -L../lib
LIBS=-lutils -lcurl

#threads:	threads.c
#					gcc $(CFLAGS) threads.c $(LIBS) -o $@ -lpthread 

hashthreads:	hashthreads.c
							gcc $(CFLAGS) hashthreads.c $(LIBS) -o $@ -lpthread

clean:
	rm -f *.o threads hashthreads 

