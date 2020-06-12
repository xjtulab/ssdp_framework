VPATH = include

run:  main.o SSDP_API.o
	g++ -o run  main.o SSDP_API.o -ldl

main.o: main.cpp SSDP.h SSDP_API.h SSDP_pre_data.h base_app.hpp
	g++ -c main.cpp -I include
SSDP_API.o:SSDP_API.cpp SSDP.h SSDP_pre_data.h base_app.hpp
	g++ -c SSDP_API.cpp -I include

clean:
	rm run *.o