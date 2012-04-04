# Makefile for compiling Compositus

CC=g++

CFLAGS=-c -Wall -O3



all: bin/Compositus



obj/Document.o: src/Document.cpp src/Document.h src/Signature.h
	$(CC) $(CFLAGS) src/Document.cpp -o obj/Document.o



obj/main.o: src/main.cpp src/DocumentSet.h src/Cluster.h
	$(CC) $(CFLAGS) src/main.cpp -o obj/main.o



bin/Compositus: obj/main.o obj/Document.o
	$(CC) obj/main.o obj/Document.o -o bin/Compositus


	
clean: cleano
	rm -rf bin/Compositus obj/*.o
