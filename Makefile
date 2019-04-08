all: project

project: main3.o head2.o
	c++ main3.o head2.o -o project

main3.o: main3.cpp
	c++ -c main3.cpp

head2.o: head2.cpp
	c++ -c head2.cpp

clean:
	rm -rf *.o project
