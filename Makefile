CC = g++
CFLAGS = -std=c++11 -g

all : clean OOP5.o
	$(CC) -c -o OOP5 OOP5.o

tests : clean OOP5_tests.o
	$(CC) -o OOP5 OOP5_tests.o

OOP5.o : OOP5.cpp
	$(CC) -c $(CFLAGS) -o $@ OOP5.cpp

OOP5_tests.o : OOP5_tests.cpp
	$(CC) -c $(CFLAGS) -o $@ OOP5_tests.cpp

clean : 
	rm -f *.o OOP5
