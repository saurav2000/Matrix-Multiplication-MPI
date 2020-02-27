blocking: blocking.o matrixMult.o
	mpic++ blocking.o matrixMult.o -o blocking

blocking.o: blocking.cpp matrixMult.h
	mpic++ -c blocking.cpp

matrixMult.o: matrixMult.cpp matrixMult.h
	g++ -c matrixMult.cpp

clean:
	rm *.o