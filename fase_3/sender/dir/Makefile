CXX=arm-rpizw-g++
PROG=main
SOURCES=main.cpp inputHandler.cpp client.cpp controller.cpp
OBJECTS=$(SOURCES:.cpp=.o)

$(PROG): $(OBJECTS)
	$(CXX) $(OBJECTS) inputHandler.hpp client.hpp controller.hpp -o $@

all: $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(PROG)

%.o: %.cpp
	$(CXX) -c $<

clean:
	rm $(OBJECTS) $(PROG)

.PHONY: all clean