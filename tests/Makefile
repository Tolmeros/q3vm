# Makefile to build test

TESTS = test_s64k test_s4k test_s3k test_s2k test_s1k test_s512

all:
	for test in $(TESTS) ; do \
		$(MAKE) -$(MAKEFLAGS) -C ./$$test ; \
	done

clean:
	for test in $(TESTS) ; do \
		$(MAKE) -$(MAKEFLAGS) -C ./$$test clean; \
	done