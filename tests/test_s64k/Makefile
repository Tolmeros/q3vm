# Makefile to build test

PARTS = test_vm test_bytecode

all:
	for part in $(PARTS) ; do \
		$(MAKE) -$(MAKEFLAGS) -C ./$$part ; \
	done
	./test_vm/q3vm_test test_bytecode/test.qvm

clean:
	for part in $(PARTS) ; do \
		$(MAKE) -$(MAKEFLAGS) -C ./$$part clean; \
	done