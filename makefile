PROG = tests
CC = g++
CPPFLAGS = -g -Wall -I include -I test -I src -DTEST_PC_CPP
LDFLAGS = # libs
OBJS = sometests.o arduino.o controller.o config.o state.o moving.o

$(PROG) : $(OBJS)
	$(CC) $(LDFLAGS) -o $(PROG) $(OBJS)

sometests.o : test/sometests.cpp include/*.h
	$(CC) $(CPPFLAGS) -c test/sometests.cpp

arduino.o : test/arduino.cpp test/arduino.h include/*.h
	$(CC) $(CPPFLAGS) -c test/arduino.cpp

controller.o : src/controller.cpp src/controller.h include/*.h
	$(CC) $(CPPFLAGS) -c src/controller.cpp

config.o : src/config.cpp src/config.h include/*.h
	$(CC) $(CPPFLAGS) -c src/config.cpp

state.o : src/state.cpp src/state.h include/*.h
	$(CC) $(CPPFLAGS) -c src/state.cpp

moving.o : src/moving.cpp src/moving.h include/*.h
	$(CC) $(CPPFLAGS) -c src/moving.cpp

