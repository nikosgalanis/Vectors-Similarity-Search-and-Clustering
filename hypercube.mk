# Makefile for hypercube target
HC_TARGET_EXEC = cube

# Directories
HC_BUILD_DIR = ./build/hypercube
HC_SRC_DIRS = programs/common programs/Hypercube 
HC_INC_DIRS = algorithms/Search/Hypercube/headers/ algorithms/Search/BruteForce/headers/ algorithms/common/ programs/common/ utils/
# Flags
HC_INC_FLAGS := $(addprefix -I,$(HC_INC_DIRS))
# Files
HC_SRCS := $(shell find $(HC_SRC_DIRS) -name *.cc)
HC_HEADS:= $(shell find $(HC_INC_DIRS) -name '*.h')
HC_OBJS := $(HC_SRCS:%=$(HC_BUILD_DIR)/%.o)
HC_DEPS := $(HC_OBJS:.o=.d)


$(HC_BUILD_DIR)/$(HC_TARGET_EXEC): $(HC_OBJS)
	@echo Building cube
	@$(CC) $(HC_OBJS) -o $@

# c source
$(HC_BUILD_DIR)/%.cc.o: %.cc $(HC_HEADS)
	@$(MKDIR_P) $(dir $@)
	@$(CC) -c $(CC_FLAGS) $< -o $@ -lm