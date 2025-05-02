CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -I. ${NIX_CFLAGS_COMPILE}
CXXFLAGS = -Wall -Wextra -I. ${NIX_CFLAGS_COMPILE} -std=c++11
TEST_LDFLAGS = -lgtest -lgtest_main -pthread ${NIX_LDFLAGS}

SRCS = main.c map.c robot_events.c
OBJS = $(addprefix Compiledir/,$(SRCS:.c=.o))
TARGET = Compiledir/satellite

TEST_SRCS = tests/map_test.cpp
TEST_OBJS = $(addprefix Compiledir/,$(TEST_SRCS:.cpp=.o))
TEST_TARGET = Compiledir/map_test

.PHONY: all clean test

all: Compiledir $(TARGET)

test: Compiledir $(TEST_TARGET)
	./$(TEST_TARGET)

Compiledir:
	mkdir -p Compiledir Compiledir/tests

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

$(TEST_TARGET): $(TEST_OBJS) $(filter-out Compiledir/main.o, $(OBJS))
	$(CXX) $(TEST_LDFLAGS) -o $@ $^

Compiledir/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

Compiledir/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf Compiledir 