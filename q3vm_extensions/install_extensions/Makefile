ifeq ($(OS),Windows_NT)
	CLEANUP = rm -f
	MKDIR = mkdir
	TARGET_EXTENSION=.exe
	LCCTOOLPATH=bin/win32
	# Linker flag: remove unused code
	LINK_FLAGS := -Wl,--gc-sections
else
	CLEANUP = rm -f
	MKDIR = mkdir -p
	TARGET_EXTENSION=
	LCCTOOLPATH=bin/linux
	# Remove dead code at link time
	# special case for macOS
	ifeq ($(shell uname -s),Darwin)
		export LINKFLAGS += -Wl,-dead_strip
	else
		export LINKFLAGS += -Wl,--gc-sections
	endif
endif

CC=$(TOOLCHAIN)gcc
LINK := $(CC)
CFLAGS += -std=c89
CFLAGS += -fdata-sections -ffunction-sections -fno-strict-aliasing
CFLAGS += -fmessage-length=0 -MMD -fno-common -MP -MF"$(@:%.o=%.d)"
CFLAGS += -Wall -Wextra
CFLAGS += -O2 -ggdb
#CFLAGS=-g -Wall -Wextra -O2

TARGET_BASE = q3vmx
TARGET = $(TARGET_BASE)$(TARGET_EXTENSION)

# Header files
INCLUDE_PATH := ../../src/vm

# Source folders
#SRC_SUBDIRS := ../../src
SRC_SUBDIRS := ./
SRC_SUBDIRS += ../../src/vm

OBJDIR := build
SOURCES = $(foreach dir, $(SRC_SUBDIRS), $(wildcard $(dir)/*.c))
C_SRCS = $(SOURCES)
VPATH = $(SRC_SUBDIRS)
OBJ_NAMES = $(notdir $(C_SRCS))
OBJS = $(addprefix $(OBJDIR)/,$(OBJ_NAMES:%.c=%.o))
C_DEPS = $(OBJS:%.o=%.d)
C_INCLUDES = -I$(INCLUDE_PATH)
LOCAL_LIBRARIES = -lm

$(OBJDIR)/%.o: %.c
	@echo 'CC: $<'
	@$(CC) $(CFLAGS) $(C_INCLUDES) -c -o "$@" "$<"

all: $(TARGET)

$(TARGET): $(OBJDIR) $(OBJS)
	$(LINK) $(LINK_FLAGS) -o"$@" $(OBJS) $(LOCAL_LIBRARIES)
	@echo 'Executable created: '$@

clean:
	@echo 'Cleanup...'
	$(CLEANUP) $(OBJDIR)/*.d
	$(CLEANUP) $(OBJDIR)/*.o
	$(CLEANUP) $(TARGET)
	$(CLEANUP) $(TARGET_BASE).dmp
	$(CLEANUP) ./*.gcov

