CFLAGS=-Wall -pedantic -std=c11 -I. -g
OFILES=queue.o hash.o webpage.o pageio.o indexio.o lqueue.o lhash.o

all: $(OFILES)
	ar cr ~/engs50/tse/lib/libutils.a $(OFILES)

%.o: %.c %.h
	gcc $(CFLAGS) -c $<

hash.o: hash.c hash.h
	gcc $(CFLAGS) -c hash.c

webpage.o: webpage.c webpage.h
	gcc $(CFLAGS) -c webpage.c

pageio.o:	pageio.c pageio.h
	gcc $(CFLAGS) -c pageio.c

indexio.o:	indexio.c indexio.h
						gcc $(CFLAGS) -c indexio.c

lqueue.o:   lqueue.c queue.h
						gcc $(CFLAGS) -c lqueue.c 

lhash.o: lhash.c hash.c
	gcc $(CFLAGS) -c lhash.c

#ltest: ltest.o list.o
#	gcc $(CFLAGS) ltest.o list.o -o ${@}

qtest: qtest.o queue.o
	gcc $(CFLAGS) qtest.o queue.o -o ${@}

htest: htest.o hash.o queue.o
	gcc $(CFLAGS) htest.o hash.o queue.o -o ${@}

webpage: webpage.o queue.o hash.o
	gcc $(CFLAGS) webpage.o -o ${@}

index:	indexio.o webpage.o queue.o hash.o
	gcc $(CFLAGS) indexio.o -o ${@}


clean:
	rm -f *.o qtest htest webpage index
