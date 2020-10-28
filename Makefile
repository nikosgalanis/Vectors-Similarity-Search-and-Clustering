.PHONY: clean


CC = g++

CC_FLAGS = -Wall -g -std=c++11 -Ofast

BUILD_DIR = ./build


all: lsh cube 
	@echo all ready


#including these .mk files for lsh cube and clustering
include lsh.mk

include hypercube.mk

# include clustering.mk


lsh: $(LSH_BUILD_DIR)/$(LSH_TARGET_EXEC)
	@echo lsh ready

cube: $(HC_BUILD_DIR)/$(HC_TARGET_EXEC)
	@echo cube ready

clean:
	$(RM) -r $(BUILD_DIR)
clean_lsh:
	$(RM) -r $(BUILD_DIR)/lsh
clean_cube:
	$(RM) -r $(BUILD_DIR)/hypercube

# Default Arguments for path I am in
LSH_ARGS  = -d misc/datasets/train-images-idx3-ubyte  -q misc/querysets/t10k-images-idx3-ubyte -k 4 -L 4 -o lsh_out -N 1 -R 10000
CUBE_ARGS = -d misc/datasets/train-images-idx3-ubyte  -q misc/querysets/t10k-images-idx3-ubyte -k 14 -M 10 -probes 2 -o out_hyper -N 1 -R 10000

run_lsh: lsh
	./$(BUILD_DIR)/lsh/lsh $(LSH_ARGS)
run_cube: cube
	./$(BUILD_DIR)/hypercube/cube $(CUBE_ARGS)

valgrind_lsh:
	valgrind ./$(BUILD_DIR)/lsh/lsh $(LSH_ARGS)
valgrind_cube:
	valgrind ./$(BUILD_DIR)/hypercube/cube $(CUBE_ARGS)
	

-include $(DEPS)

MKDIR_P ?= mkdir -p