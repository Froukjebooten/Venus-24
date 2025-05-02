.PHONY: all clean satellite robot test

all: satellite robot

satellite:
	$(MAKE) -C satellite

robot:
	$(MAKE) -C robot

test: satellite-test robot-test

satellite-test:
	$(MAKE) -C satellite test

robot-test:
	$(MAKE) -C robot test

clean:
	$(MAKE) -C satellite clean
	$(MAKE) -C robot clean 