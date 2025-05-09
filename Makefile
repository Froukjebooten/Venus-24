.PHONY: all clean satellite robot test run-test-bench

all: satellite robot

satellite:
	$(MAKE) -C satellite

run-satellite: satellite
	./satellite/Compiledir/satellite

robot:
	$(MAKE) -C robot

test: satellite-test robot-test

satellite-test:
	$(MAKE) -C satellite test

robot-test:
	$(MAKE) -C robot test

run-test-bench:
	@if [ -z "$(BENCH)" ]; then \
		echo "Error: Please specify a test bench to run using BENCH=<bench_name>"; \
		echo "Available test benches:"; \
		ls satellite/test_benches/*.c | sed 's/.*\///;s/\.c$$//'; \
		exit 1; \
	fi
	$(MAKE) -C satellite run-test-bench BENCH=$(BENCH)

clean:
	$(MAKE) -C satellite clean
	$(MAKE) -C robot clean 