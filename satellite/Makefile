CC = gcc
CXX = g++
CFLAGS += -Wall -Wextra -I. $(SDL_CFLAGS)
CXXFLAGS += -std=c++14 $(CFLAGS)
TEST_CFLAGS += $(CFLAGS) $(GTEST_CFLAGS)
LDFLAGS += $(SDL_LDFLAGS)
TEST_LDFLAGS += $(GTEST_LDFLAGS) $(LDFLAGS)

SRCS = main.c map.c robot_events.c map_visualizer.c
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
	$(CC) -o $@ $^ $(LDFLAGS)

$(TEST_TARGET): $(TEST_OBJS) $(filter-out Compiledir/main.o, $(OBJS))
	$(CXX) $(TEST_LDFLAGS) -o $@ $^

Compiledir/tests/%.o: tests/%.cpp
	$(CXX) $(TEST_CFLAGS) -c $< -o $@

Compiledir/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

Compiledir/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf Compiledir 