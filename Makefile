CC =		gcc

CFLAGS =	-g -Wall
LDFLAGS =	-L/usr/lib/i386-linux-gnu -lcurl -lm

all:		client

client:		src/client.c
	$(CC) $(CFLAGS) src/client.c src/cJSON.c src/parser.c $(LDFLAGS) -o oess-client

clean:
	-rm -f oess-client src/*.o src/*.a src/a.out src/core src/core.* src/*.core
