HC_TARGET_EXEC = cube


HC_BUILD_DIR = ./build/hypercube
HC_SRC_DIRS = programs/common programs/Hypercube 

HC_INC_DIRS = algorithms/Hypercube/headers/ algorithms/BruteForce/headers/ algorithms/common/ programs/common/ utils/

HC_INC_FLAGS := $(addprefix -I,$(HC_INC_DIRS))

HC_SRCS := $(shell find $(HC_SRC_DIRS) -name *.cc)

HC_OBJS := $(HC_SRCS:%=$(HC_BUILD_DIR)/%.o)
HC_DEPS := $(HC_OBJS:.o=.d)


$(HC_BUILD_DIR)/$(HC_TARGET_EXEC): $(HC_OBJS)
	$(CC) $(HC_OBJS) -o $@

# c source
$(HC_BUILD_DIR)/%.cc.o: %.cc
	$(MKDIR_P) $(dir $@)
	$(CC) -c $(CC_FLAGS) $< -o $@ -lm