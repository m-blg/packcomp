CLI_VERSION = 1.0.0
LIB_VERSION = 1.1.0

LIB_VERSION_MAJOR = $(word 1, $(subst ., , $(LIB_VERSION)))

BUILD_DIR = target
BUILD_DIR_TESTS = target/tests
SRC_DIR = src
TEST_DIR = tests
INSTALL_PREFIX = /usr
INSTALL_PREFIX_BIN = /usr/bin
INSTALL_PREFIX_LIB = /usr/lib64

CC = g++
INCS = -Isrc
LIBS = -l:libcurl.so.4 -l:libjson-c.so.5
TEST_LIBS = -l:libcriterion.so.3
# LIBS = -lcurl -ljson-c

DEBFLAGS = -g -Wall
RELFLAGS = -O2
MODE_FLAGS = $(RELFLAGS)

CPPFLAGS = -DVERSION=\"${CLI_VERSION}\"
CFLAGS = $(MODE_FLAGS) $(INCS)
LDFLAGS = $(LIBS)
TEST_LDFLAGS = $(LDFLAGS) $(TEST_LIBS)

LIB_LNAME = libpackcomp.so
LIB_SONAME = $(LIB_LNAME).$(LIB_VERSION_MAJOR)
LIB = $(LIB_LNAME).$(LIB_VERSION)
CLI_BIN = packcomp

CLI_SRC = cli/main.cc

SRCS = $(shell find $(SRC_DIR) -name "*.cc")
HS = $(shell find $(SRC_DIR) -name "*.cc")
# SRCS_D1 = $(shell find $(SRC_DIR) -maxdepth 1 -name "*.cc")
SRCS_D1 = $(SRC_DIR)/packcomp.cc
TEST_SRCS = $(shell find $(TEST_DIR) -maxdepth 1 -name "*.cc")
TESTS = $(BUILD_DIR_TESTS)/$(basename $(notdir $(TEST_SRCS)))


all: lib cli

.PHONY: deb check
deb:
	@echo "$(LIB_VERSION_MAJOR)"
	@echo "$(SRCS)"
	@echo "$(HS)"
	@echo "$(SRCS_D1)"
	@echo "$(TEST_SRCS)"
	@echo "$(TESTS)"

check:
	ldd -r $(BUILD_DIR)/$(CLI_BIN)
	ldd -r $(BUILD_DIR)/$(LIB)

cli: $(BUILD_DIR)/$(CLI_BIN)
lib: $(BUILD_DIR)/$(LIB)

test: $(TESTS)
	for test in $(TESTS) ; do \
		./$$test ; \
	done


run: 
ifdef test
	@echo "test: $(test)"
	@make $(BUILD_DIR_TESTS)/$(test)
	$(BUILD_DIR_TESTS)/$(test)
endif

build: 
ifdef test
	@echo "test: $(test)"
	@make $(BUILD_DIR_TESTS)/$(test)
endif

$(BUILD_DIR)/$(CLI_BIN): $(BUILD_DIR)/$(LIB) $(CLI_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CLI_SRC) -o $@ $(LDFLAGS) -lpackcomp -L$(BUILD_DIR)

$(BUILD_DIR_TESTS)/%: $(TEST_DIR)/%.cc $(SRCS) $(HS)
	@mkdir -p $(BUILD_DIR_TESTS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TEST_DIR)/$(notdir $@).cc -o $@ $(LDFLAGS) $(TEST_LDFLAGS)

$(BUILD_DIR)/$(LIB): $(SRCS) $(HS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -fPIC -shared -Wl,-soname,$(LIB_SONAME) $(SRCS_D1) -o $@ $(LDFLAGS)
	ln -s $(LIB) $(BUILD_DIR)/$(LIB_SONAME)
	ln -s $(LIB_SONAME) $(BUILD_DIR)/$(LIB_LNAME)

install: all
	mkdir -p $(INSTALL_PREFIX_BIN)
	cp -f $(BUILD_DIR)/$(CLI_BIN) $(INSTALL_PREFIX_BIN)
	chmod 755 $(INSTALL_PREFIX_BIN)/$(CLI_BIN)
	mkdir -p $(INSTALL_PREFIX_LIB)
	cp -f $(BUILD_DIR)/$(LIB) $(INSTALL_PREFIX_LIB)
	ln -s $(LIB) $(INSTALL_PREFIX_LIB)/$(LIB_SONAME)
	ln -s $(LIB_SONAME) $(INSTALL_PREFIX_LIB)/$(LIB_LNAME)

uninstall:
	rm -f $(INSTALL_PREFIX_BIN)/$(CLI_BIN)\
	      $(INSTALL_PREFIX_LIB)/$(LIB_LNAME)\
	      $(INSTALL_PREFIX_LIB)/$(LIB_SONAME)\
	      $(INSTALL_PREFIX_LIB)/$(LIB)

clean:
	$(RM) -r target

.PHONY: all lib cli build run test install uninstall clean
