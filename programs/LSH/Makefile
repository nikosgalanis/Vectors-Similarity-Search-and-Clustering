TARGET_EXEC = lsh

CC = g++

BUILD_DIR = ./build
SRC_DIRS = ../../algorithms/LSH/src ../common/ ./
INC_DIRS = ../../algorithms/LSH/headers ../../algorithms/LSH/headers ../../algorithms/common/ ../common/

SRCS := $(shell find $(SRC_DIRS) -name *.cc)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CC_FLAGS = -Wall -g -I$(INC_DIRS) -std=c++11

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