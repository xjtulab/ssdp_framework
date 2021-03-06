VPATH = include:src
myhfile := $(inclde/wildcard *.h)

#为arm
ARMCC =/home/osrc/toolchain/gcc-arm-8.2-2018.08-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++

#为x86
CC = g++
ARM = arm-linux-gnueabihf-
objects = src/SSDP_LOG.o src/SSDP_API.o src/commandprocess.o src/SSDPServer.o src/DeviceDSP.o src/DeviceFPGA.o src/DspPublisherTwo.o src/HelloWorld.o src/DeviceZED.o src/FPGAPublisher.o
# src/DspPublisher.o src/HelloWorld.o

run: main.o $(objects)
ifdef tar
	$(ARMCC) -w -o run main.o $(objects)  -L arm_build/lib/ lib/*.a  -ldl -lpthread -D ARM_BUILD -lmicrocdr -lmicroxrcedds_client 
	# lib/*.a
	cp run test
	cp -r devices test
	cp *.xml test
	cp dds_agent/* test
	tar -czf test.tar.gz test
else
	$(CC) --static -o run main.o $(objects) -ldl -lpthread -lmicrocdr -lmicroxrcedds_client
# -lmicrocdr -lmicroxrcedds_client
endif 

main.o: main.cpp $(myhfile)
ifdef tar
	$(ARMCC) -w -c main.cpp -I include -D ARM_BUILD  -I arm_build/include/
else
	$(CC) -c main.cpp -I include 
endif

$(objects):%.o:%.cpp
ifdef tar
	$(ARMCC) -w -c $< -o $@ -I include -D ARM_BUILD -I arm_build/include/
else
	$(CC) -c $< -o $@ -I include 
endif

.PHONY: clean
clean:
	# rm tmp/*
	rm run src/*.o *.o
	rm -rf test/*
	rm test.tar.gz 
