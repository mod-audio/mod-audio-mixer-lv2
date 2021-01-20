#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

include dpf/Makefile.base.mk

PREFIX ?= /usr
libdir ?= $(PREFIX)/lib

NAME = mod-mixer

all: plugins gen

# --------------------------------------------------------------

libs:
	$(MAKE) -C common

dgl:
ifeq ($(HAVE_DGL),true)
	$(MAKE) -C dpf/dgl
endif

plugins:
	$(MAKE) all -C plugins/$(NAME)

ifneq ($(CROSS_COMPILING),true)
gen: plugins dpf/utils/lv2_ttl_generator
	#@$(CURDIR)/dpf/utils/generate-ttl.sh
	cp plugins/$(NAME)/lv2-data/* bin/$(NAME).lv2/
ifeq ($(MACOS),true)
	@$(CURDIR)/dpf/utils/generate-vst-bundles.sh
endif

dpf/utils/lv2_ttl_generator:
	$(MAKE) -C dpf/utils/lv2-ttl-generator
else
gen:
endif

# --------------------------------------------------------------
install:
	install -d $(DESTDIR)$(libdir)/lv2/$(NAME).lv2

	install -m 644 bin/$(NAME).lv2/*.so  $(DESTDIR)$(libdir)/lv2/$(NAME).lv2/
	install -m 644 bin/$(NAME).lv2/*.ttl $(DESTDIR)$(libdir)/lv2/$(NAME).lv2/

clean:
	#$(MAKE) clean -C dpf/dgl
	$(MAKE) clean -C dpf/utils/lv2-ttl-generator
	$(MAKE) clean -C plugins/$(NAME)
	rm -rf bin build

# --------------------------------------------------------------

.PHONY: plugins
