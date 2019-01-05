CC = g++
LDFLAGS = -L/usr/local/opt/openssl/lib
CPPFLAGS = -I/usr/local/opt/openssl/include

all: binary
	./binary

binary: main.cpp rsa.o
	$(CC) main.cpp rsa.o -o binary -lssl -lcrypto $(LDFLAGS) $(CPPFLAGS)

rsa.o: rsa.cpp
	$(CC) rsa.cpp -c -o rsa.o

.PHONY:
clean:
	rm binary *.o