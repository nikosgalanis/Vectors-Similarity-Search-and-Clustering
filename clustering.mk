# Makefile for clustering
CL_TARGET_EXEC = cluster

# Directories
# Directories
CL_EXEC_DIR = ./executables/clustering
CL_BUILD_DIR = ./build/clustering
CL_SRC_DIRS = programs/common programs/Clustering
CL_INC_DIRS = algorithms/Search/LSH/headers/ algorithms/Search/BruteForce/headers/ algorithms/common/ programs/common/ utils/ \
			  algorithms/Search/Hypercube/headers/ algorithms/Clustering
# Flags
CL_INC_FLAGS := $(addprefix -I,$(CL_INC_DIRS))
# Files
CL_SRCS := $(shell find $(CL_SRC_DIRS) -name *.cc)
CL_HEADS:= $(shell find $(CL_INC_DIRS) -name '*.h')
CL_OBJS := $(CL_SRCS:%=$(CL_BUILD_DIR)/%.o)
CL_DEPS := $(CL_OBJS:.o=.d)

$(CL_EXEC_DIR)/$(CL_TARGET_EXEC): $(CL_OBJS)
	@echo Building clustering...
	@$(MKDIR_P) $(dir $@)
	@$(CC) $(CL_OBJS) -o $@

# cc source
$(CL_BUILD_DIR)/%.cc.o: %.cc $(CL_HEADS)
	@$(MKDIR_P) $(dir $@)
	@$(CC) -c $(CL_FLAGS) $< -o $@ -lm
