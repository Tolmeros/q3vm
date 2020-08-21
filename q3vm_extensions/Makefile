MODULES = install_extensions qvm_hash file_code_segment_instruction_pointers

all:
	for module in $(MODULES) ; do \
		$(MAKE) -$(MAKEFLAGS) -C ./$$module ; \
	done

clean:
	for module in $(MODULES) ; do \
		$(MAKE) -$(MAKEFLAGS) -C ./$$module clean; \
	done