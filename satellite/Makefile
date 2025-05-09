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

# Test benches
TEST_BENCH_SRCS = test_benches/spiral_main.c
TEST_BENCH_OBJS = $(addprefix Compiledir/,$(TEST_BENCH_SRCS:.c=.o))
TEST_BENCH_TARGETS = $(addprefix Compiledir/,$(TEST_BENCH_SRCS:.c=))

.PHONY: all clean test test_benches run-test-bench

all: Compiledir $(TARGET)

test: Compiledir $(TEST_TARGET)
	./$(TEST_TARGET)

test_benches: Compiledir $(TEST_BENCH_TARGETS)

run-test-bench: test_benches
	@if [ -z "$(BENCH)" ]; then \
		echo "Error: Please specify a test bench to run using BENCH=<bench_name>"; \
		echo "Available test benches:"; \
		ls test_benches/*.c | sed 's/.*\///;s/\.c$$//'; \
		exit 1; \
	fi
	@if [ ! -f "Compiledir/test_benches/$(BENCH)" ]; then \
		echo "Error: Test bench '$(BENCH)' not found"; \
		echo "Available test benches:"; \
		ls test_benches/*.c | sed 's/.*\///;s/\.c$$//'; \
		exit 1; \
	fi
	./Compiledir/test_benches/$(BENCH)

Compiledir:
	mkdir -p Compiledir Compiledir/tests Compiledir/test_benches

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(TEST_TARGET): $(TEST_OBJS) $(filter-out Compiledir/main.o, $(OBJS))
	$(CXX) $(TEST_LDFLAGS) -o $@ $^

# Rule for test bench executables
Compiledir/test_benches/%: Compiledir/test_benches/%.o $(filter-out Compiledir/main.o, $(OBJS))
	$(CC) -o $@ $^ $(LDFLAGS)

Compiledir/tests/%.o: tests/%.cpp
	$(CXX) $(TEST_CFLAGS) -c $< -o $@

Compiledir/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

Compiledir/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf Compiledir 