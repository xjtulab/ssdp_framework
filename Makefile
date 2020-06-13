VPATH = include:src
myhfile := $(inclde/wildcard *.h)

CC = g++
ARM = arm-linux-gnueabihf-
objects = src/SSDP_LOG.o src/SSDP_API.o

run: main.o $(objects)
	$(CC) -o run main.o $(objects) -ldl

main.o: main.cpp $(myhfile)
	$(CC) -c main.cpp -I include
$(objects):%.o:%.cpp
	$(CC) -c $< -o $@ -I include 

#main.o: main.cpp $(myhfile)
#	g++ -c main.cpp -I include
#SSDP_API.o:SSDP_API.cpp $(myhfile)
#	g++ -c src/SSDP_API.cpp -I include
#SSDP_LOG.o:SSDP_LOG.cpp $(myhfile)
#	g++ -c src/SSDP_LOG.cpp -I include

.PHONY: clean
clean:
	rm run src/*.o *.o
	rm tmp/*