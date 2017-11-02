#Compiler
CC=g++

#FLAGS
CFLAGS=-std=c++1z

PROG=tfstat
TARGET=tfstat

$(TARGET):src/$(PROG).cpp
	$(CC) $(CFLAGS) -c src/Common.cpp -o lib/Common.o
	$(CC) $(CFLAGS) -c src/traffic.cpp -o lib/traffic.o
	$(CC) $(CFLAGS) -c src/Database.cpp -o lib/Database.o
	$(CC) $(CFLAGS) -c src/tfstat.cpp -o lib/tfstat.o
	$(CC) $(CFLAGS) lib/Common.o lib/traffic.o lib/Database.o lib/tfstat.o -o main.out
clean:
	$(RM) main
	$(RM) lib/Common.o
	$(RM) lib/traffic.o
	$(RM) lib/Database.o
	$(RM) lib/tfstat.o
#	$(RM) lib/Database.a
quick:
	$(CC) $(CFLAGS) -o $(TARGET) src/$(PROG).cpp lib/traffic.a
