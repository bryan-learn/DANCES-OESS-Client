CC =		gcc

CFLAGS =	-g -Wall
LIBS =		-liperf -lcurl -lm -ljson-c

all:	client example

client:		
	$(CC) $(CFLAGS) src/circuit-test.c src/cJSON.c src/parser.c src/coua.c $(LIBS) -o oess-ua

example:	
	$(CC) $(CFLAGS) src/example-client.c src/cJSON.c src/parser.c src/coua.c $(LIBS) -o oess-example

clean:
	-rm -f oess-example oess-ua src/*.o src/*.a src/a.out src/core src/core.* src/*.core
