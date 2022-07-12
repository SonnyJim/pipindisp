CFLAGS += -pg -Wall
tft:tft.o GC9A01.o
	gcc $(CFLAGS) -o tft tft.o GC9A01.o -lbcm2835
GC9A01.o:GC9A01.c GC9A01.h
	gcc $(CFLAGS)   -c GC9A01.c -lbcm2835
tft.o:tft.c GC9A01.h 
	gcc $(CFLAGS) -c tft.c -lbcm2835
clean:
	rm GC9A01.o tft.o tft
