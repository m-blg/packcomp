CLI_VERSION = 1.0.0
LIB_VERSION = 1.0.0

LIB_VERSION_MAJOR = $(word 1, $(subst ., , $(LIB_VERSION)))

BUILD_DIR = ./target
SRC_DIR = ./src
TEST_DIR = ./tests
INSTALL_PREFIX = /usr

CC = g++
INCS = -Isrc
LIBS = -lcurl -ljson-c

CPPFLAGS = -DVERSION=\"${CLI_VERSION}\"
CFLAGS = -g -Wall $(INCS)
LDFLAGS = $(LIBS)

LIB_LNAME = libpackcomp.so
LIB_SONAME = $(LIB_LNAME).$(LIB_VERSION_MAJOR)
LIB = $(LIB_LNAME).$(LIB_VERSION)
CLI_BIN = packcomp

CLI_SRC = cli/main.cc

SRCS = $(shell find $(SRC_DIR) -name "*.cc")
HS = $(shell find $(SRC_DIR) -name "*.cc")
SRCS_D1 = $(shell find $(SRC_DIR) -maxdepth 1 -name "*.cc")


all: lib cli

.PHONY: deb
deb:
	@echo "$(LIB_VERSION_MAJOR)"
	@echo "$(SRCS)"
	@echo "$(HS)"
	@echo "$(SRCS_D1)"

cli: $(BUILD_DIR)/$(CLI_BIN)
lib: $(BUILD_DIR)/$(LIB)


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

$(BUILD_DIR)/$(LIB): $(SRCS) $(HS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -fpic -shared -Wl,-soname,$(LIB_SONAME) $(SRCS_D1) -o $@ $(LDFLAGS)
	ln -s $(LIB) $(BUILD_DIR)/$(LIB_SONAME)
	ln -s $(LIB_SONAME) $(BUILD_DIR)/$(LIB_LNAME)

install: all
	mkdir -p $(INSTALL_PREFIX)/bin
	cp -f $(BUILD_DIR)/$(CLI_BIN) $(INSTALL_PREFIX)/bin
	chmod 755 $(INSTALL_PREFIX)/bin/$(CLI_BIN)
	mkdir -p $(INSTALL_PREFIX)/lib
	cp -f $(BUILD_DIR)/$(LIB) $(INSTALL_PREFIX)/lib
	ln -s $(LIB) $(INSTALL_PREFIX)/lib/$(LIB_SONAME)
	ln -s $(LIB_SONAME) $(INSTALL_PREFIX)/lib/$(LIB_LNAME)

uninstall:
	rm -f $(INSTALL_PREFIX)/bin/$(CLI_BIN)\
	      $(INSTALL_PREFIX)/lib/$(LIB_LNAME)\
	      $(INSTALL_PREFIX)/lib/$(LIB_SONAME)\
	      $(INSTALL_PREFIX)/lib/$(LIB)

clean:
	$(RM) -r target

.PHONY: all cli build run install uninstall clean
