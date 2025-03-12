# Copyright (C) Steve Jeong, 2025. <steve@how2flow.net>
#
# This makefile is intended for use with GNU make

# Vars #
build_dir = $(PWD)/build
include_dirs = $(PWD)/include
src_dirs = $(PWD)/src

srcs = $(wildcard $(src_dirs)/*.c)
objs = $(patsubst %.c, $(build_dir)/%.o, $(srcs))

# Architecture #
ifeq ($(CROSS_COMPILE), arm64)
  ARCH = aarch64-none-linux-gnu
else
  ifeq ($(CROSS_COMPILE), x86)
    ARCH = x86_64-linux-gnu
  endif
endif

# Params #
ARCH ?= $(shell dpkg-architecture -qDEB_HOST_MULTIARCH)
CC = ${ARCH}-gcc
CFLAGS = -g -I$(include_dirs)
LDFLAGS =
TARGET = main

# Functions #
all: $(TARGET)

$(TARGET): $(objs)
	@$(CC) -o $@ $(objs) $(CFLAGS) $(LDFLAGS)

$(build_dir)/%.o: %.c
	@mkdir -p $(shell dirname $@)
	@$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	@rm -rf $(build_dir) $(TARGET)
