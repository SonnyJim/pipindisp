CFLAGS += -pg -g -Wall
LIBS += -lbcm2835
pipindisp:pipindisp.o GC9A01.o
	gcc $(CFLAGS) -o pipindisp pipindisp.o GC9A01.o $(LIBS)
GC9A01.o:GC9A01.c GC9A01.h
	gcc $(CFLAGS)   -c GC9A01.c -lbcm2835
pipindisp.o:pipindisp.c GC9A01.h 
	gcc $(CFLAGS) -c pipindisp.c -lbcm2835
clean:
	rm GC9A01.o pipindisp.o pipindisp
