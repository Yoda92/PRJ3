CXX=arm-rpizw-g++
PROG=main
SOURCES=main.cpp outputHandler.cpp server.cpp
OBJECTS=$(SOURCES:.cpp=.o)

$(PROG): $(OBJECTS)
	$(CXX) $(OBJECTS) outputHandler.hpp server.hpp -o $@

all: $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(PROG)

%.o: %.cpp
	$(CXX) -c $<

clean:
	rm $(OBJECTS) $(PROG)

.PHONY: all clean