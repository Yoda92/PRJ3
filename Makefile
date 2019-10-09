CXX=arm-rpizw-gcc
PROG=main
SOURCES=main.c inputHandler.c controller.c
OBJECTS=$(SOURCES:.c=.o)

$(PROG): $(OBJECTS)
	$(CXX) $(OBJECTS) inputHandler.h controller.h -o $@

all: $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(PROG)

%.o: %.c
	$(CXX) -c $<

clean:
	rm $(OBJECTS) $(PROG)

.PHONY: all clean