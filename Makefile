
BUILD_DIR = ./target
SRC_DIR = ./src
TEST_DIR = ./tests

CC = clang
INCS = -Isrc
LIBS = -lcurl -ljson-c
CFLAGS = -g -Wall $(INCS)
LDFLAGS = $(LIBS)

LIB = packcomp.so

SRCS = $(shell find $(SRC) -name "*.cc")
HS = $(shell find $(SRC) -name "*.cc")



all: $(BUILD_DIR)/$(LIB)

# .PHONY: deb
# deb:
# 	@echo "$(SRCS)"

run: 
ifdef test
	@echo "test: $(test)"
	@make $(BUILD_DIR)/tests/$(test)
	$(BUILD_DIR)/tests/$(test)
endif

$(BUILD_DIR)/tests/%: $(TEST_DIR)/%.cc $(SRCS) $(HS)
	@mkdir -p $(BUILD_DIR)/tests
	$(CC) $(CFLAGS) $(TEST_DIR)/$(notdir $@).cc -o $@ $(LDFLAGS)

$(BUILD_DIR)/$(LIB): $(SRCS) $(HS) $(LDFLAGS)
	$(CC) $(CFLAGS) -fPIC -shared $< -o $@


clean:
	$(RM) -r target

.PHONY: all run clean
