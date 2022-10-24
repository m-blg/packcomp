VERSION = 1.0

BUILD_DIR = ./target
SRC_DIR = ./src
TEST_DIR = ./tests
INSTALL_PREFIX = /usr

CC = g++
INCS = -Isrc
LIBS = -lcurl -ljson-c

CPPFLAGS = -DVERSION=\"${VERSION}\"
CFLAGS = -g -Wall $(INCS)
LDFLAGS = $(LIBS)

LIB = libpackcomp.so
CLI_BIN = packcomp

CLI_SRC = cli/main.cc

SRCS = $(shell find $(SRC) -name "*.cc")
HS = $(shell find $(SRC) -name "*.cc")



all: $(BUILD_DIR)/$(LIB) $(BUILD_DIR)/$(CLI_BIN)

# .PHONY: deb
# deb:
# 	@echo "$(SRCS)"

cli: $(BUILD_DIR)/$(CLI_BIN)
	


run: 
ifdef test
	@echo "test: $(test)"
	@make $(BUILD_DIR)/tests/$(test)
	$(BUILD_DIR)/tests/$(test)
endif

build: 
ifdef test
	@echo "test: $(test)"
	@make $(BUILD_DIR)/tests/$(test)
endif

$(BUILD_DIR)/$(CLI_BIN): $(BUILD_DIR)/$(LIB) $(CLI_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CLI_SRC) -o $@ $(LDFLAGS) -lpackcomp -L$(BUILD_DIR)

$(BUILD_DIR)/tests/%: $(TEST_DIR)/%.cc $(SRCS) $(HS)
	@mkdir -p $(BUILD_DIR)/tests
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TEST_DIR)/$(notdir $@).cc -o $@ $(LDFLAGS)

$(BUILD_DIR)/$(LIB): $(SRCS) $(HS) $(LDFLAGS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -fpic -shared $< -o $@

install: all
	mkdir -p $(INSTALL_PREFIX)/bin
	cp -f $(BUILD_DIR)/$(CLI_BIN) $(INSTALL_PREFIX)/bin
	chmod 755 $(INSTALL_PREFIX)/bin/$(CLI_BIN)
	mkdir -p $(INSTALL_PREFIX)/lib
	cp -f $(BUILD_DIR)/$(LIB) $(INSTALL_PREFIX)/lib

uninstall:
	rm -f $(INSTALL_PREFIX)/bin/$(CLI_BIN)
	rm -f $(INSTALL_PREFIX)/lib/$(LIB)

clean:
	$(RM) -r target

.PHONY: all cli build run install uninstall clean
