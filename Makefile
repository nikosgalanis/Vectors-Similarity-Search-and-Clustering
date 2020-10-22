.PHONY: clean


CC = g++

CC_FLAGS = -Wall -g -std=c++11

BUILD_DIR = ./build

include lsh.mk

include hypercube.mk

# include clustering.mk

lsh:

hypercube:


clean:
	$(RM) -r $(BUILD_DIR)

run:
	./$(BUILD_DIR)/$(TARGET_EXEC)

valgrind:
	valgrind ./$(BUILD_DIR)/$(TARGET_EXEC)
	

-include $(DEPS)

MKDIR_P ?= mkdir -p