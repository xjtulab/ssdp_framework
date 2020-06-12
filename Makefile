VPATH = include:src
myhfile := $(inclde/wildcard *.h)

run:  main.o SSDP_API.o SSDP_LOG.o
	g++ -o run  main.o SSDP_API.o SSDP_LOG.o -ldl

main.o: main.cpp $(myhfile)
	g++ -c main.cpp -I include
SSDP_API.o:SSDP_API.cpp $(myhfile)
	g++ -c src/SSDP_API.cpp -I include
SSDP_LOG.o:SSDP_LOG.cpp $(myhfile)
	g++ -c src/SSDP_LOG.cpp -I include
clean:
	rm run *.o
	rm tmp/*