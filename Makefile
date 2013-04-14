FLAGS= -g -Wall -lpthread -lm
TARGET= vns
LIBS=solver.c support.c hungarian.c hun_methods.c 

all: main.c $(LIBS)
	test -d bin||mkdir bin
	gcc $(FLAGS) -o bin/$(TARGET) main.c $(LIBS)
clean:
	rm -rf bin	
