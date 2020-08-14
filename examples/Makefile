# Makefile to build examples

EXAMPLES = hello_world hello_world_stack_4k

all:
	for example in $(EXAMPLES) ; do \
		$(MAKE) -$(MAKEFLAGS) -C ./$$example ; \
	done

clean:
	for example in $(EXAMPLES) ; do \
		$(MAKE) -$(MAKEFLAGS) -C ./$$example clean; \
	done