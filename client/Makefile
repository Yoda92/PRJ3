CXX=arm-rpizw-gcc
PROG=test
SOURCES=client.c testClient.c
OBJECTS=$(SOURCES:.c=.o)

$(PROG): $(OBJECTS)
	$(CXX) $(OBJECTS) client.h -o $@

all: $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(PROG)

%.o: %.c
	$(CXX) -c $<

clean:
	rm $(OBJECTS) $(PROG)

.PHONY: all clean