CXX=arm-rpizw-g++
PROG=main
SOURCES=main.cpp inputHandler.cpp outputHandler.cpp controller.cpp mcp3202Handler.cpp spiHandler.cpp
OBJECTS=$(SOURCES:.cpp=.o)

$(PROG): $(OBJECTS)
	$(CXX) $(OBJECTS) inputHandler.hpp outputHandler.hpp controller.hpp mcp3202Handler.hpp spiHandler.hpp -o $@

all: $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(PROG)

%.o: %.cpp
	$(CXX) -c $<

clean:
	rm $(OBJECTS) $(PROG)

.PHONY: all clean