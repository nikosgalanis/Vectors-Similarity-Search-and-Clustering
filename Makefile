.PHONY: clean

include lsh.mk

include hypercube.mk

include clustering.mk

lsh:

clean:
	$(RM) -r $(BUILD_DIR)

run:
	./$(BUILD_DIR)/$(TARGET_EXEC)

valgrind:
	valgrind ./$(BUILD_DIR)/$(TARGET_EXEC)
	

-include $(DEPS)

MKDIR_P ?= mkdir -p