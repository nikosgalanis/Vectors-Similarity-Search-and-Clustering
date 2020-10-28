.PHONY: clean


CC = g++

CC_FLAGS = -Wall -g -std=c++11  -Wno-reorder -Ofast

BUILD_DIR = ./build
EXEC_DIR = ./executables

# Compile all
all: lsh cube cluster
	@echo all ready


#including these .mk files for lsh cube and clustering
include lsh.mk

include hypercube.mk

include clustering.mk

# Compilation
lsh: $(LSH_EXEC_DIR)/$(LSH_TARGET_EXEC)
	@echo lsh ready

cube: $(HC_EXEC_DIR)/$(HC_TARGET_EXEC)
	@echo cube ready

cluster: $(CL_EXEC_DIR)/$(CL_TARGET_EXEC)
	@echo cluster ready

# Cleaning
clean:
	$(RM) -r $(BUILD_DIR) $(EXEC_DIR)
clean_lsh:
	$(RM) -r $(BUILD_DIR)/lsh $(EXEC_DIR)/lsh
clean_cube:
	$(RM) -r $(BUILD_DIR)/hypercube $(EXEC_DIR)/hypercube
clean_cluster:
	$(RM) -r $(BUILD_DIR)/clustering $(EXEC_DIR)/clustering

# Default Arguments for path I am in
LSH_ARGS  = -d misc/datasets/train-images-idx3-ubyte  -q misc/querysets/t10k-images-idx3-ubyte -k 4 -L 4 -o executables/lsh/lsh_out -N 1 -R 10000
CUBE_ARGS = -d misc/datasets/train-images-idx3-ubyte  -q misc/querysets/t10k-images-idx3-ubyte -k 14 -M 100 -probes 50 -o executables/hypercube/cube_out -N 1 -R 10000
CLUSTER_LLOYDS_ARGS = -i misc/datasets/train-images-idx3-ubyte -c config/cluster.conf -o executables/clustering/cluster_lloyds_out -m Lloyds 
CLUSTER_LLOYDS_COMPLETE_ARGS = -i misc/datasets/train-images-idx3-ubyte -c config/cluster.conf -o executables/clustering/cluster_lloyds_complete_out -complete -m Lloyds 
CLUSTER_LSH_ARGS = -i misc/datasets/train-images-idx3-ubyte -c config/cluster.conf -o executables/clustering/cluster_lsh_out -m Range_Search_LSH
CLUSTER_LSH_COMPLETE_ARGS = -i misc/datasets/train-images-idx3-ubyte -c config/cluster.conf -o executables/clustering/cluster_lsh_complete_out -complete -m Range_Search_LSH
CLUSTER_CUBE_ARGS = -i misc/datasets/train-images-idx3-ubyte -c config/cluster.conf -o executables/clustering/cluster_cube_out -m Range_Search_Hypercube
CLUSTER_CUBE_COMPLETE_ARGS = -i misc/datasets/train-images-idx3-ubyte -c config/cluster.conf -o executables/clustering/cluster_cube_complete_out -complete -m Range_Search_Hypercube

# Running
run_lsh: lsh
	./$(EXEC_DIR)/lsh/lsh $(LSH_ARGS)
run_cube: cube
	./$(EXEC_DIR)/hypercube/cube $(CUBE_ARGS)
run_cluster_lloyds: cluster
	./$(EXEC_DIR)/clustering/cluster $(CLUSTER_LLOYDS_ARGS)
run_cluster_lloyds_complete: cluster
	./$(EXEC_DIR)/clustering/cluster $(CLUSTER_LLOYDS_COMPLETE_ARGS)
run_cluster_lsh: cluster
	./$(EXEC_DIR)/clustering/cluster $(CLUSTER_LSH_ARGS)
run_cluster_lsh_complete: cluster
	./$(EXEC_DIR)/clustering/cluster $(CLUSTER_LSH_COMPLETE_ARGS)
run_cluster_cube: cluster
	./$(EXEC_DIR)/clustering/cluster $(CLUSTER_CUBE_ARGS)
run_cluster_cube_complete: cluster
	./$(EXEC_DIR)/clustering/cluster $(CLUSTER_CUBE_COMPLETE_ARGS)

# Valgrind
valgrind_lsh: lsh
	valgrind ./$(EXEC_DIR)/lsh/lsh $(LSH_ARGS)
valgrind_cube: cube
	valgrind ./$(EXEC_DIR)/hypercube/cube $(CUBE_ARGS)
valgrind_cluster_lloyds: cluster
	valgrind ./$(EXEC_DIR)/clustering/cluster $(CLUSTER_LLOYDS_ARGS)
valgrind_cluster_lloyds_complete: cluster
	valgrind ./$(EXEC_DIR)/clustering/cluster $(CLUSTER_LLOYDS_COMPLETE_ARGS)
valgrind_cluster_lsh: cluster
	valgrind ./$(EXEC_DIR)/clustering/cluster $(CLUSTER_LSH_ARGS)
valgrind_cluster_lsh_complete: cluster
	valgrind ./$(EXEC_DIR)/clustering/cluster $(CLUSTER_LSH_COMPLETE_ARGS)
valgrind_cluster_cube: cluster
	valgrind ./$(EXEC_DIR)/clustering/cluster $(CLUSTER_CUBE_ARGS)
valgrind_cluster_cube_complete: cluster
	valgrind ./$(EXEC_DIR)/clustering/cluster $(CLUSTER_CUBE_COMPLETE_ARGS)
	

-include $(DEPS)

MKDIR_P ?= mkdir -p