CC = g++
CFLAGS = -std=c++11 -g

all : clean OOP5.o
	$(CC) -o OOP5 OOP5.o

OOP5.o : OOP5.cpp
	$(CC) -c $(CFLAGS) -o $@ OOP5.cpp

clean : 
	rm -f *.o OOP5
