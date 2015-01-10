CC=g++
CFLAGS=-c -O2 -Wall
LDFLAGS= -O2
SOURCES=spirals.cpp lodepng/lodepng.cpp
OBJECTS=$(SOURCES:.cpp=.o)
	EXECUTABLE=spirals

all: $(SOURCES) $(EXECUTABLE)
		
$(EXECUTABLE): $(OBJECTS) 
		$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
		$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE)
