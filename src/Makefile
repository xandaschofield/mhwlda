CC=		g++

OBJS=		strtokenizer.o dataset.o utils.o model.o walker.o
MAIN=		lda
 
all:	$(OBJS) $(MAIN).cpp
	$(CC) -o $(MAIN) $(MAIN).cpp $(OBJS)
	strip $(MAIN)

walker.o: walker.h walker.cpp
	$(CC) -c -o walker.o walker.cpp

strtokenizer.o:	strtokenizer.h strtokenizer.cpp
	$(CC) -c -o strtokenizer.o strtokenizer.cpp

dataset.o:	dataset.h dataset.cpp
	$(CC) -c -o dataset.o dataset.cpp

utils.o:	utils.h utils.cpp
	$(CC) -c -o utils.o utils.cpp

model.o:	model.h model.cpp walker.o
	$(CC) -c -o model.o model.cpp

test:
	

clean:
	rm $(OBJS) 
	rm $(MAIN)

