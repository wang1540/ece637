# For Linux or any machines with gcc compiler
CC = gcc
CFLAGS = -Wall -pedantic -std=c99

# For SunOS
#CFLAGS = -Aa

all: Example

clean:
	/bin/rm *.o lab3_sec1

OBJ = tiff.o allocate.o randlib.o

Example: lab3_sec1.o $(OBJ) 
	$(CC) $(CFLAGS) -o lab3_sec1 $(OBJ) -lm

