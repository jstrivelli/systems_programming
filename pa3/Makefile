CC=gcc
CFLAGS=-Wall -g

build:
	make indexer.o sorted-list.o tokenizer.o index

index.o: indexer.c
	$(CC) $(CFLAGS) -c indexer.c

sorted-list.o: sorted-list.c sorted-list.h
	$(CC) $(CFLAGS) -c sorted-list.c

tokenizer.o: tokenizer.c tokenizer.h
	$(CC) $(CFLAGS) -c tokenizer.c

index: index.o sorted-list.o tokenizer.o
	$(CC) $(CFLAGS) indexer.o sorted-list.o tokenizer.o -o index

clean:
	-rm -rf *.o index *.dSYM

rebuild:
	make clean build
