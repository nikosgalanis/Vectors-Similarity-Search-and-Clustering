LSH_TARGET_EXEC = lsh

LSH_BUILD_DIR = ./build/lsh
LSH_SRC_DIRS = programs/common programs/LSH 

LSH_INC_DIRS = algorithms/LSH/headers/ algorithms/BruteForce/headers/ algorithms/common/ programs/common/ utils/

LSH_INC_FLAGS := $(addprefix -I,$(LSH_INC_DIRS))

LSH_SRCS := $(shell find $(LSH_SRC_DIRS) -name *.cc)

LSH_OBJS := $(LSH_SRCS:%=$(LSH_BUILD_DIR)/%.o)
LSH_DEPS := $(LSH_OBJS:.o=.d)


$(LSH_BUILD_DIR)/$(LSH_TARGET_EXEC): $(LSH_OBJS)
	$(CC) $(LSH_OBJS) -o $@

# c source
$(LSH_BUILD_DIR)/%.cc.o: %.cc
	$(MKDIR_P) $(dir $@)
	$(CC) -c $(CC_FLAGS) $< -o $@ -lm