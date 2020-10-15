TARGET_EXEC = lsh

CC = g++

ABS_DIR = $(shell realpath ../../)
BUILD_DIR = ./build
ABS_DIRS = $(ABS_DIR)/algorithms/LSH/src $(ABS_DIR)/programs/common .
SRC_DIRS = ../../algorithms/LSH/src ../common .
INC_DIRS = ../../algorithms/LSH/headers/ ../../algorithms/BruteForce/headers/ ../../algorithms/common/ ../common/

INC_FLAGS := $(addprefix -I,$(INC_DIRS))

SRCS := $(shell find $(ABS_DIRS) -name *.cc)
$(info $$SRCS is [${SRCS}])
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CC_FLAGS = -Wall -g -std=c++11

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@

# c++ source
$(BUILD_DIR)/%.cc.o: %.cc
	$(MKDIR_P) $(dir $@)
	$(CC) -c $(CC_FLAGS) $< -o $@ -lm


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

run:
	./$(BUILD_DIR)/$(TARGET_EXEC)

valgrind:
	valgrind ./$(BUILD_DIR)/$(TARGET_EXEC)
	

-include $(DEPS)

MKDIR_P ?= mkdir -p