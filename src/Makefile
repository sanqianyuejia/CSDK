# compiler
CC=gcc

# compiler flags
CFLAGS=-g -Wall -fPIC

# X-VPR Related
CFLAGS += -I/usr/local/include/json-c -I/usr/local/lib/curl
LDFLAGS += -L/usr/local/lib -ljson-c -lcurl

PROGRAM=libxvpr.so.1.2.1
SOURCES=xbusiness_vpr.c xbusiness_vad.c xbusiness_misc.c xbusiness_aid.c
OBJECTS=xbusiness_vpr.o xbusiness_vad.o xbusiness_misc.o xbusiness_aid.o

program: $(OBJECTS)
	$(CC) -shared -Wl,-soname,libxvpr.so.1 -o $(PROGRAM) $(OBJECTS) $(LDFLAGS)

$(OBJECTS): $(SOURCES)
	$(CC) -c $(CFLAGS) $(LDFLAGS) $(SOURCES)

clean:
	rm -f *.o
	rm -f $(PROGRAM)
	rm -f *~

