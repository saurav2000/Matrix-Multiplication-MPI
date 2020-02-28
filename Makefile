all: blocking collective nonblocking serial

blocking: blocking.o matrixMult.o
	mpic++ blocking.o matrixMult.o -o blocking

collective: collective.o matrixMult.o
	mpic++ collective.o matrixMult.o -o collective

nonblocking: nonblocking.o matrixMult.o
	mpic++ nonblocking.o matrixMult.o -o nonblocking

serial.o: serial.cpp matrixMult.h
	g++ -c serial.cpp

blocking.o: blocking.cpp matrixMult.h
	mpic++ -c blocking.cpp

collective.o: collective.cpp matrixMult.h
	mpic++ -c collective.cpp

nonblocking.o: nonblocking.cpp matrixMult.h
	mpic++ -c nonblocking.cpp

serial: serial.o matrixMult.o
	g++ serial.o matrixMult.o -o serial

matrixMult.o: matrixMult.cpp matrixMult.h
	g++ -c matrixMult.cpp

clean:
	rm *.o blocking collective nonblocking serial

