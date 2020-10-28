# Makefile for lsh target
LSH_TARGET_EXEC = lsh

# Directories
LSH_EXEC_DIR = ./executables/lsh
LSH_BUILD_DIR = ./build/lsh
LSH_SRC_DIRS = programs/common programs/LSH 
LSH_INC_DIRS = algorithms/Search/LSH/headers/ algorithms/Search/BruteForce/headers/ algorithms/common/ programs/common/ utils/
# Flags
LSH_INC_FLAGS := $(addprefix -I,$(LSH_INC_DIRS))
# Files
LSH_SRCS := $(shell find $(LSH_SRC_DIRS) -name *.cc)
LSH_HEADS:= $(shell find $(LSH_INC_DIRS) -name '*.h')
LSH_OBJS := $(LSH_SRCS:%=$(LSH_BUILD_DIR)/%.o)
LSH_DEPS := $(LSH_OBJS:.o=.d)


$(LSH_EXEC_DIR)/$(LSH_TARGET_EXEC): $(LSH_OBJS)
	@echo Building lsh...
	@$(MKDIR_P) $(dir $@)
	@$(CC) $(LSH_OBJS) -o $@

# cc source
$(LSH_BUILD_DIR)/%.cc.o: %.cc $(LSH_HEADS)
	@$(MKDIR_P) $(dir $@)
	@$(CC) -c $(CC_FLAGS) $< -o $@ -lm