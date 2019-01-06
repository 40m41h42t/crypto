CC = g++
LDFLAGS = -L/usr/local/opt/openssl/lib
CPPFLAGS = -I/usr/local/opt/openssl/include

all: binary
	./binary

binary: main.cpp dsa.o
	$(CC) main.cpp dsa.o -o binary -lssl -lcrypto $(LDFLAGS) $(CPPFLAGS)

dsa.o: dsa.cpp
	$(CC) dsa.cpp -c -o dsa.o

.PHONY:
clean:
	rm binary *.o