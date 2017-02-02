CFLAGS = -g -O
CC = g++
LINK = g++ 
OFILES = huffman.o pqueue.o trace.o binary1.o
OFILES2 = unhuffman.o pqueue.o trace.o binary1.o

test: huffman
	./huffman

debug: huffman
	gdb ./huffman

huffman: $(OFILES)
	$(LINK) -o huffman $(OFILES)

unhuffman: $(OFILES2)
	$(LINK) -o unhuffman $(OFILES2)

unhuffman.o: unhuffman.cpp trace.h tree.h binary.h
	$(CC) $(CFLAGS) -c unhuffman.cpp

pqueue.o: pqueue.cpp pqueue.h
	$(CC) $(CFLAGS) -c pqueue.cpp

trace.o: trace.cpp trace.h
	$(CC) $(CFLAGS) -c trace.cpp

binary1.o: binary1.cpp binary.h
	$(CC) $(CFLAGS) -c binary1.cpp

huffman.o: huffman.cpp tree.h pqueue.h trace.h binary.h
	$(CC) $(CFLAGS) -c huffman.cpp

clean:
	rm $(OFILES) huffman
