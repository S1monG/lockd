# Simple Makefile for lockd
# Builds all .c files under src/ and runs unit tests under test/

CC := gcc
CFLAGS := -Wall -Wextra -O3 -g
LDLIBS := -lgpiod -lmicrohttpd # Link against libgpiod and libmicrohttpd for GPIO control and HTTP server

SRCDIR := src
TESTDIR := test
INCLUDES := -I$(SRCDIR) -I$(TESTDIR)
BUILDDIR := build
BINDIR := bin
TARGET := $(BINDIR)/lockd
TESTBIN := $(BINDIR)/run_tests

SRC := $(shell find $(SRCDIR) -name '*.c')
APPSRC := $(shell find $(SRCDIR)/app -name '*.c')
LIBSRC := $(filter-out $(SRCDIR)/app/%.c,$(SRC)) # all source files except src/app
LIBOBJ := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(LIBSRC))
APPOBJ := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(APPSRC))
TESTSRC := $(TESTDIR)/test_main.c
TESTVENDORSRC := $(TESTDIR)/vendor/unity.c
TESTOBJ := $(patsubst $(TESTDIR)/%.c,$(BUILDDIR)/%.test.o,$(TESTSRC))
TESTVENDOROBJ := $(BUILDDIR)/vendor/unity.test.o
DEPS := $(LIBOBJ:.o=.d) $(APPOBJ:.o=.d) $(TESTOBJ:.o=.d) $(TESTVENDOROBJ:.o=.d)

.PHONY: all
all: $(TARGET)

$(TARGET): $(LIBOBJ) $(APPOBJ) | $(BINDIR)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

$(TESTBIN): $(TESTOBJ) $(TESTVENDOROBJ) $(LIBOBJ) | $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ $(LDLIBS) -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(BUILDDIR)/%.test.o: $(TESTDIR)/%.c | $(BUILDDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(BINDIR) $(BUILDDIR):
	mkdir -p $@

-include $(DEPS)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(BINDIR)

.PHONY: run
run: all
	$(TARGET)

.PHONY: test
test: $(TESTBIN)
	@echo "Running unit tests..."
	@$(TESTBIN)

.PHONY: help
help:
	@echo "Usage: make [target]"
	@echo "Targets: all (default), clean, run, test"
