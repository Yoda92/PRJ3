CXX=arm-rpizw-gcc
PROG=test
SOURCES=server.c testServer.c
OBJECTS=$(SOURCES:.c=.o)

$(PROG): $(OBJECTS)
	$(CXX) $(OBJECTS) server.h -o $@

all: $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(PROG)

%.o: %.c
	$(CXX) -c $<

clean:
	rm $(OBJECTS) $(PROG)

.PHONY: all clean