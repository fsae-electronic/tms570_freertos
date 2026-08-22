TARGET   = tms570_freertos

# Toolchain — the project uses the custom arm-none-eabi toolchain built from
# ../programming/tms570_debug_test/Dockerfile (GCC 12.3 + newlib, big-endian).
#
# Build options:
#   1. Inside Docker (recommended):
#        docker build -t tms570-toolchain ../programming/tms570_debug_test/
#        docker run --rm -v "$(CURDIR):/project" tms570-toolchain make
#
#   2. Toolchain installed at /opt/arm-none-eabi-be (from Dockerfile locally):
#        make
#
#   3. Arch Linux system toolchain with newlib:
#        sudo pacman -S arm-none-eabi-newlib
#        make TOOLCHAIN_BIN=""
#
# Override TOOLCHAIN_BIN to point at any arm-none-eabi toolchain bin directory.
TOOLCHAIN_BIN ?= /opt/arm-none-eabi-be/bin

ifneq ($(TOOLCHAIN_BIN),)
  _T := $(TOOLCHAIN_BIN)/arm-none-eabi-
else
  _T := arm-none-eabi-
endif

CC      = $(_T)gcc
CXX     = $(_T)g++
AS      = $(_T)gcc
LD      = $(_T)g++
OBJCOPY = $(_T)objcopy
SIZE    = $(_T)size

CPU_FLAGS = -mcpu=cortex-r4 -mfpu=vfpv3-d16 -mfloat-abi=hard -mbig-endian -marm

CFLAGS   = $(CPU_FLAGS) -Os -Wall -Wextra -ffunction-sections -fdata-sections -g3
CXXFLAGS = $(CPU_FLAGS) -Os -Wall -Wextra -ffunction-sections -fdata-sections -g3 \
           -fno-exceptions -fno-rtti -fno-threadsafe-statics
ASFLAGS  = $(CPU_FLAGS) -x assembler-with-cpp

LDSCRIPT = halcogen/sys_link.ld
LDFLAGS  = $(CPU_FLAGS) -nostartfiles -Wl,--gc-sections \
           -Wl,-Map=$(TARGET).map -T $(LDSCRIPT)

INCLUDES = \
    -Iinclude \
    -Ift81x_driver \
    -Imsgpack/include \
    -Icobs

# ---- Source files -----------------------------------------------------------

SRCS_C :=
SRCS_C += $(wildcard app/*.c)
SRCS_C += $(wildcard drivers/*.c)
SRCS_C += $(wildcard halcogen/*.c)
SRCS_C += $(wildcard rtos/*.c)
SRCS_C += $(wildcard ti_fee/*.c)
SRCS_C += $(wildcard ft81x_driver/*.c)
SRCS_C += $(wildcard cobs/*.c)
# msgpack/src excluded: sysdep.h is missing (needs `git submodule update --init`
# and a platform sysdep.h before the msgpack-c library can be compiled).
# Re-enable once msgpack integration is implemented.

# C++ sources — exclude simulation/bench-test entry point by default
SRCS_CXX := $(wildcard app/*.cpp)
SRCS_CXX := $(filter-out app/test_main_ecu.cpp, $(SRCS_CXX))

# Assembly sources (GNU as syntax .s files only)
SRCS_S :=
SRCS_S += $(wildcard halcogen/*.s)
SRCS_S += $(wildcard rtos/*.s)

# ---- Object files -----------------------------------------------------------

OBJS  := $(patsubst %.c,   build/%.o, $(SRCS_C))
OBJS  += $(patsubst %.cpp, build/%.o, $(SRCS_CXX))
OBJS  += $(patsubst %.s,   build/%.o, $(SRCS_S))

# ---- Targets ----------------------------------------------------------------

.PHONY: all clean size docker-build

all: $(TARGET).elf

$(TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^ -lgcc -lc -lnosys
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

# C compilation
build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# C++ compilation
build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Assembly compilation (preprocessed .s files)
build/%.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $(INCLUDES) -c $< -o $@

size: $(TARGET).elf
	$(SIZE) $<

# Build inside Docker using the project toolchain image
DOCKER_IMAGE ?= tms570-toolchain
docker-build:
	docker run --rm -v "$(CURDIR):/project" $(DOCKER_IMAGE) make

clean:
	rm -rf build $(TARGET).elf $(TARGET).bin $(TARGET).map
