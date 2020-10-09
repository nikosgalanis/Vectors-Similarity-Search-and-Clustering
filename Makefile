TARGET_EXEC = vectors_search_clustering

CC = g++

BUILD_DIR = ./build
SRC_DIRS = ./programs ./modules
INC_DIRS = ./include

SRCS := $(shell find $(SRC_DIRS) -name *.cc)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CC_FLAGS = -Wall -g -I$(INC_DIRS)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
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