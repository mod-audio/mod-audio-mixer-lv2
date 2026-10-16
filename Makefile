#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

include dpf/Makefile.base.mk

PREFIX ?= /usr
libdir ?= $(PREFIX)/lib

MIXER_MONO = mod-mixer
MIXER_STEREO = mod-mixer-stereo

all: plugins gen

# --------------------------------------------------------------

libs:
	$(MAKE) -C common

dgl:
ifeq ($(HAVE_DGL),true)
	$(MAKE) -C dpf/dgl
endif

plugins:
	$(MAKE) all -C plugins/$(MIXER_MONO)
	$(MAKE) all -C plugins/$(MIXER_STEREO)

ifneq ($(CROSS_COMPILING),true)
gen: plugins dpf/utils/lv2_ttl_generator
	#@$(CURDIR)/dpf/utils/generate-ttl.sh
	cp plugins/$(MIXER_MONO)/lv2-data/* bin/$(MIXER_MONO).lv2/
	cp plugins/$(MIXER_STEREO)/lv2-data/* bin/$(MIXER_STEREO).lv2/
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
	install -d $(DESTDIR)$(libdir)/lv2/$(MIXER_MONO).lv2
	install -d $(DESTDIR)$(libdir)/lv2/$(MIXER_STEREO).lv2

	install -m 644 bin/$(MIXER_MONO).lv2/*.so  $(DESTDIR)$(libdir)/lv2/$(MIXER_MONO).lv2/
	install -m 644 bin/$(MIXER_MONO).lv2/*.ttl $(DESTDIR)$(libdir)/lv2/$(MIXER_MONO).lv2/
	install -m 644 bin/$(MIXER_STEREO).lv2/*.so  $(DESTDIR)$(libdir)/lv2/$(MIXER_STEREO).lv2/
	install -m 644 bin/$(MIXER_STEREO).lv2/*.ttl $(DESTDIR)$(libdir)/lv2/$(MIXER_STEREO).lv2/


clean:
	#$(MAKE) clean -C dpf/dgl
	$(MAKE) clean -C dpf/utils/lv2-ttl-generator
	$(MAKE) clean -C plugins/$(MIXER_MONO)
	$(MAKE) clean -C plugins/$(MIXER_STEREO)
	rm classes/*.o classes/*.d
	rm -rf bin build

# --------------------------------------------------------------

.PHONY: plugins
