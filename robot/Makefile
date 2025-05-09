CC = gcc
CXX = g++
CFLAGS += -Wall -Wextra -I.
CXXFLAGS += -std=c++14 $(CFLAGS)
TEST_CFLAGS += $(CFLAGS) $(GTEST_CFLAGS)
TEST_LDFLAGS += $(GTEST_LDFLAGS) $(LDFLAGS)

SRCS = main.c
OBJS = $(addprefix Compiledir/,$(SRCS:.c=.o))
TARGET = Compiledir/robot

TEST_SRCS = tests/robot_test.cpp
TEST_OBJS = $(addprefix Compiledir/,$(TEST_SRCS:.cpp=.o))
TEST_TARGET = Compiledir/robot_test

.PHONY: all clean test

all: Compiledir $(TARGET)

test: Compiledir $(TEST_TARGET)
	./$(TEST_TARGET)

Compiledir:
	mkdir -p Compiledir Compiledir/tests

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(TEST_LDFLAGS) -o $@ $^

Compiledir/tests/%.o: tests/%.cpp
	$(CXX) $(TEST_CFLAGS) -c $< -o $@

Compiledir/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

Compiledir/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf Compiledir 