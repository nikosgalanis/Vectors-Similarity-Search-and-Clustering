TARGET_EXEC = lsh

CC = g++

BUILD_DIR = ./build/lsh
SRC_DIRS = algorithms/LSH/src programs/common programs/LSH

INC_DIRS = algorithms/LSH/headers/ algorithms/BruteForce/headers algorithms/common/ programs/common/

INC_FLAGS := $(addprefix -I,$(INC_DIRS))

SRCS := $(shell find $(SRC_DIRS) -name *.cc)

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CC_FLAGS = -Wall -g -std=c++11

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@

# c++ source
$(BUILD_DIR)/%.cc.o: %.cc
	$(MKDIR_P) $(dir $@)
	$(CC) -c $(CC_FLAGS) $< -o $@ -lm