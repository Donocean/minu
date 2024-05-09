######################################
# target
######################################
TARGET = demo

# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
test.c \
src/minu.c \
src/minu_disp.c \
src/minu_anim.c \
src/minu_monitor.c \
src/minu_anim_easing.c \
utils/minu_mem.c \
utils/minu_vector.c \

#######################################
# binaries
#######################################
# PREFIX = arm-none-eabi-

# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
OBJDUMP=$(PREFIX)objdump
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
OBJDUMP=$(PREFIX)objdump
SZ = $(PREFIX)size
endif

#######################################
# CFLAGS
#######################################

# C defines
C_DEFS =  \
-DCUSTOM \

# C includes
C_INCLUDES =  \
-Iinc \
-Iutils \
-Iport \

CFLAGS += $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

# libraries
LIBS = -lc -lm

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf

# build the application
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LIBS) -o $@
	$(SZ) $@
	
$(BUILD_DIR):
	mkdir $@

# clean up
clean:
	@-rm -fR $(BUILD_DIR)

run:
	@./build/demo.elf
  
# dependencies
-include $(wildcard $(BUILD_DIR)/*.d)
