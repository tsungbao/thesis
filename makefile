CC := g++
CFLAGS_DEBUG := -std=c++17 -Wall -DDEBUG -g
CFLAGS_TEST := -std=c++17 -Wall -DTEST -g

SRCS := $(wildcard ./src/*.cpp) 

HEADERS := $(SRCS: .cpp =.h)

test.o : $(SRCS) ${HEADERS} ./test/basic_test.cpp
	$(CC) $(CFLAGS_TEST) $(SRCS) ./test/basic_test.cpp -o ./bin/$@ 

debug.o : $(SRCS) ${HEADERS} ./test/basic_test.cpp
	$(CC) $(CFLAGS_DEBUG) $(SRCS) ./test/basic_test.cpp -o ./bin/$@ 
install.o : $(SRCS)
	$(CC) $(CFLAGS) $^ -o ./bin/$@