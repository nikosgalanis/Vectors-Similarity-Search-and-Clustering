TARGET_EXEC = cube

CC = g++

BUILD_DIR = ./build/hypercube
SRC_DIRS = programs/common programs/Hypercube 

INC_DIRS = algorithms/Hypercube/headers/ algorithms/BruteForce/headers/ algorithms/common/ programs/common/ utils/

INC_FLAGS := $(addprefix -I,$(INC_DIRS))

SRCS := $(shell find $(SRC_DIRS) -name *.cc)

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CC_FLAGS = -Wall -g -std=c++11

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@

# c source
$(BUILD_DIR)/%.cc.o: %.cc
	$(MKDIR_P) $(dir $@)
	$(CC) -c $(CC_FLAGS) $< -o $@ -lm