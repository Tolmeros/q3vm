# Makefile to build test

TESTS = test_s64k test_s4k

all:
	for test in $(TESTS) ; do \
		$(MAKE) -$(MAKEFLAGS) -C ./$$test ; \
	done

clean:
	for test in $(TESTS) ; do \
		$(MAKE) -$(MAKEFLAGS) -C ./$$test clean; \
	done