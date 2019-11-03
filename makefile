all: result

result: sometests.o arduino.o controller.o config.o state.o moving.o
	g++ sometests.o arduino.o controller.o config.o state.o moving.o -o sometests

sometests.o: test/sometests.cpp
	g++ -c test/sometests.cpp -DTEST_PC_CPP -I include -I test -I src

arduino.o: test/arduino.cpp
	g++ -c test/arduino.cpp -DTEST_PC_CPP -I include -I test -I src

controller.o: src/controller.cpp
	g++ -c src/controller.cpp -DTEST_PC_CPP -I include -I test -I src

config.o: src/config.cpp
	g++ -c src/config.cpp -DTEST_PC_CPP -I include -I test -I src

state.o: src/state.cpp
	g++ -c src/state.cpp -DTEST_PC_CPP -I include -I test -I src

moving.o: src/moving.cpp
	g++ -c src/moving.cpp -DTEST_PC_CPP -I include -I test -I src
