# Makefile for bin/test
# created with makefile-creator


####################
#Settings:


CC =  clang++
CFLAGS =  -O4 -Wall -Wextra -pedantic -std=c++11 
CLIBS = 
INCLUDES = 
TARGET = bin/test
OBJECTS = build/test.o


####################
#Rules:


$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(CLIBS)

build/%.o:
	@if test ! -d 'build'; then mkdir 'build'; echo "created 'build'" ; fi
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm build/*.o

all: $(TARGET)


####################
#Dependencies:


build/test.o: src/test/test.cpp src/include/basic_number.hpp 

