blocking: blocking.o matrixMult.o
	mpic++ blocking.o matrixMult.o -o blocking

collective: collective.o matrixMult.o
	mpic++ collective.o matrixMult.o -o collective

blocking.o: blocking.cpp matrixMult.h
	mpic++ -c blocking.cpp

collective.o: collective.cpp matrixMult.h
	mpic++ -c collective.cpp

matrixMult.o: matrixMult.cpp matrixMult.h
	mpic++ -c matrixMult.cpp

clean:
	rm *.o