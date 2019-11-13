PROG = tests
CC = g++
CPPFLAGS = -g -Wall -I include -I test -I src -DTEST_PC_CPP
LDFLAGS = # libs
OBJS = sometests.o arduino.o controller.o state.o parser_my.o rs232.o

$(PROG) : $(OBJS)
	$(CC) $(LDFLAGS) -o $(PROG) $(OBJS)

sometests.o : test/sometests.cpp test/arduino.h src/controller.h src/parser_my.h src/state.h include/enums.h include/macros.h test/rs232.h
	$(CC) $(CPPFLAGS) -c test/sometests.cpp

arduino.o : test/arduino.cpp test/arduino.h
	$(CC) $(CPPFLAGS) -c test/arduino.cpp

controller.o : src/controller.cpp src/controller.h src/state.h include/enums.h include/macros.h
	$(CC) $(CPPFLAGS) -c src/controller.cpp

state.o : src/state.cpp src/state.h include/enums.h
	$(CC) $(CPPFLAGS) -c src/state.cpp

parser_my.o : src/parser_my.cpp src/parser_my.h src/vector.h
	$(CC) $(CPPFLAGS) -c src/parser_my.cpp

rs232.o : test/rs232.c test/rs232.h
	$(CC) $(CPPFLAGS) -c test/rs232.c
