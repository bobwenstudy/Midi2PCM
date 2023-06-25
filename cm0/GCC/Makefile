#
# 'make'        build executable file 'main'
# 'make clean'  removes all .o and executable files
#
.DEFAULT_GOAL := all

ifeq ($(OS),Windows_NT)
TOOLCHAIN_DIR 	= D:\Program Files (x86)\Arm GNU Toolchain arm-none-eabi\12.2 mpacbti-rel1
else
TOOLCHAIN_DIR = /usr/gcc-arm-none-eabi-7-2018-q2-update
endif

# define the target
TARGET		  	?= main





COMPILE_PREFIX 	+= $(TOOLCHAIN_DIR)/bin/arm-none-eabi-

# define the C compiler to use
CC      := $(COMPILE_PREFIX)gcc
CXX     := $(COMPILE_PREFIX)g++
OBJDUMP := $(COMPILE_PREFIX)objdump
OBJCOPY := $(COMPILE_PREFIX)objcopy
GDB     := $(COMPILE_PREFIX)gdb
AR      := $(COMPILE_PREFIX)ar
SIZE    := $(COMPILE_PREFIX)size

## cpu setting ##
CPU_ARCH := cortex-m0

LINKER_SCRIPT ?= gcc_arm.ld



# define any compile-time flags
CFLAGS  := -Os
CFLAGS  += -g

CFLAGS += -mthumb -mcpu=$(CPU_ARCH)

# warning param setting
CFLAGS	+= -Wall
# think use -Os.
CFLAGS	+= -Wno-unused-function
CFLAGS	+= -Wno-unused-variable

CFLAGS	+= -Wstrict-prototypes
CFLAGS	+= -Wshadow
# CFLAGS	+= -Werror

# spec c version
CFLAGS  += -std=c99

# for makefile depend tree create
CFLAGS  += -MMD -MP

# for weak.
CFLAGS  += -fno-common

## MAKEFILE COMPILE MESSAGE CONTROL ##
ifeq ($(V),1)
	Q=
else
	Q=@
endif

# Put functions and data in their own binary sections so that ld can
# garbage collect them
ifeq ($(NOGC),1)
GC_CFLAGS =
GC_LDFLAGS =
else
GC_CFLAGS = -ffunction-sections -fdata-sections
GC_LDFLAGS = -Wl,--gc-sections -Wl,--check-sections
endif


CFLAGS	+= $(GC_CFLAGS)

# define ld params
LDFLAGS	:=
LDFLAGS	+= $(GC_LDFLAGS)
LDFLAGS += --specs=nosys.specs
LDFLAGS += -T $(LINKER_SCRIPT)


# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS :=
LFLAGS += -lm

# define output directory
OUTPUT_PATH	:= output

# define source directory
SRC		:= 

# define include directory
INCLUDE	:=

# define lib directory
LIB		:= 





ROOT_PATH := ../..

INCLUDE	+= $(ROOT_PATH)/TinySoundFont

MAIN_PATH := $(ROOT_PATH)/cm0
include $(MAIN_PATH)/build.mk







ifeq ($(OS),Windows_NT)
ifdef ComSpec
	WINCMD:=$(ComSpec)
endif
ifdef COMSPEC
	WINCMD:=$(COMSPEC)
endif

SHELL:=$(WINCMD)

MAIN	:= $(TARGET).elf
ECHO=echo
SOURCEDIRS	:= $(SRC)
INCLUDEDIRS	:= $(INCLUDE)
LIBDIRS		:= $(LIB)
FIXPATH = $(subst /,\,$1)
RM			:= del /q /s
MD			:= mkdir
else
MAIN	:= $(TARGET).elf
ECHO=echo
SOURCEDIRS	:= $(shell find $(SRC) -type d)
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)
FIXPATH = $1
RM = rm -rf
MD	:= mkdir -p
endif

# define any directories containing header files other than /usr/include
INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))
@echo INCLUDES: $(INCLUDES)

# define the C libs
LIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))

# define the C source files
SOURCES		:= $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))

# define the C object files 
OBJDIR = $(OUTPUT_PATH)/obj/1/2/3/4/5
OBJECTS			:= $(patsubst %, $(OBJDIR)/%, $(SOURCES:.c=.o))
OBJ_MD			:= $(addprefix $(OBJDIR)/, $(SOURCEDIRS))





ALL_DEPS := $(OBJECTS:.o=.d)

# include dependency files of application
ifneq ($(MAKECMDGOALS),clean)
-include $(ALL_DEPS)
endif

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

OUTPUT_MAIN		:= $(OUTPUT_PATH)/$(MAIN)
OUTPUT_TARGET	:= $(OUTPUT_PATH)/$(TARGET)

# Fix path error.
#OUTPUT_MAIN := $(call FIXPATH,$(OUTPUT_MAIN))

.PHONY: clean help info

info:
	@$(ECHO) Current Configuration: APP=$(APP) PORT=$(PORT) CHIPSET=$(CHIPSET)

all: | info $(MAIN) $(DEBUG_REPORT)
	@$(ECHO) Start Build Image.
	$(OBJCOPY) -v -O binary $(OUTPUT_MAIN) $(OUTPUT_TARGET).bin
	"$(OBJDUMP)" --source --all-headers --demangle --line-numbers --wide $(OUTPUT_MAIN) > $(OUTPUT_TARGET).lst
	"$(OBJDUMP)" -S -d $(OUTPUT_MAIN) > $(OUTPUT_TARGET).dump
	@$(ECHO) Print Size
	$(Q)@$(SIZE) --format=berkeley $(OUTPUT_MAIN)
#	@$(ECHO) Executing 'all' complete!

# mk path for object.
$(OBJ_MD):
	$(Q)if not exist "$@" $(Q)$(MD) $(call FIXPATH, $@)

# mk output path.
$(OUTPUT_PATH):
	$(Q)if not exist "$@" $(Q)$(MD) $(call FIXPATH, $@)

$(OBJDIR):
	$(Q)if not exist "$@" $(Q)$(MD) $(call FIXPATH, $@)

$(MAIN): | $(OUTPUT_PATH) $(OBJDIR) $(OBJ_MD) $(OBJECTS) 
	@$(ECHO) Linking    : "$@"
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) -Wl,-Map,$(OUTPUT_TARGET).map -o $(OUTPUT_MAIN) $(OBJECTS) $(LFLAGS) $(LIBS)

# Static Pattern Rules [targets ...: target-pattern: prereq-patterns ...]
# $(OBJECTS): $(OBJDIR)/%.o : %.c $(AUTOCONFIG_H)
$(OBJECTS): $(OBJDIR)/%.o : %.c
	@$(ECHO) Compiling  : "$<"
	$(Q)$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@



clean:
#	$(RM) $(OUTPUT_MAIN)
#	$(RM) $(OBJECTS)
#	$(RM) $(OBJDIR)
	$(Q)$(RM) $(call FIXPATH, $(OUTPUT_PATH))
	@$(ECHO) Cleanup complete!

run: all
	./$(OUTPUT_MAIN)
	@$(ECHO) Executing 'run: all' complete!

ram_report:
	python $(MAIN_PATH)/scripts/footprint/size_report -k $(OUTPUT_MAIN) -z $(OUTPUT_PATH) -o $(OUTPUT_PATH) --workspace=$(OUTPUT_PATH) -d 99 ram

rom_report:
	python $(MAIN_PATH)/scripts/footprint/size_report -k $(OUTPUT_MAIN) -z $(OUTPUT_PATH) -o $(OUTPUT_PATH) --workspace=$(OUTPUT_PATH) -d 99 rom

all_report:
	python $(MAIN_PATH)/scripts/footprint/size_report -k $(OUTPUT_MAIN) -z $(OUTPUT_PATH) -o $(OUTPUT_PATH) --workspace=$(OUTPUT_PATH) -d 99 all

code_format:
	python code_format.py

