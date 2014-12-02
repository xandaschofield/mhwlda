CC=		g++

OBJS=		strtokenizer.o dataset.o utils.o model.o walker.o
MAIN=		lda
FLAGS=		-fopenmp -O3 -std=c++11 -o
OFLAGS=		-fopenmp -O3 -std=c++11 -c -o
 
all:	$(OBJS) $(MAIN).cpp
	$(CC) $(FLAGS) $(MAIN) $(MAIN).cpp $(OBJS)
	strip $(MAIN)

walker.o: walker.h walker.cpp
	$(CC) $(OFLAGS) walker.o walker.cpp

strtokenizer.o:	strtokenizer.h strtokenizer.cpp
	$(CC) $(OFLAGS) strtokenizer.o strtokenizer.cpp

dataset.o:	dataset.h dataset.cpp
	$(CC) $(OFLAGS) dataset.o dataset.cpp

utils.o:	utils.h utils.cpp
	$(CC) $(OFLAGS) utils.o utils.cpp

model.o:	model.h model.cpp walker.o
	$(CC) $(OFLAGS) model.o model.cpp

test:
	

clean:
	rm $(OBJS) 
	rm $(MAIN)

