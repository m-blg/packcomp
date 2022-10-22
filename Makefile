
BUILD_DIR = ./target
SRC_DIR = ./src
TEST_DIR = ./tests

CC = clang
CFLAGS = -g -Wall
LIB = libbincompare.so



all: $(BUILD_DIR)/$(LIB)

run: 
ifdef test
	@echo "test: $(test)"
	@make $(BUILD_DIR)/tests/$(test)
	$(BUILD_DIR)/tests/$(test)
endif

$(BUILD_DIR)/tests/%: $(TEST_DIR)/%.cc $(SRC_DIR)/*.cc $(SRC_DIR)/*.h
	@mkdir -p $(BUILD_DIR)/tests
	$(CC) $(CFLAGS) $(TEST_DIR)/$(notdir $@).cc -o $@

$(BUILD_DIR)/$(LIB): $(SRC_DIR)/*.cc $(SRC_DIR)/*.h
	$(CC) $(CFLAGS) -fPIC -shared $< -o $@


clean:
	$(RM) -r target

.PHONY: all run clean
