# --------------------------------------------------------------
#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = mod-mixer

# --------------------------------------------------------------
# Files to build

FILES_DSP  = \
	mod-mixer.cpp \
	classes/onepole.cpp \
	classes/panning.cpp \
	classes/volumeSlider.cpp \
	classes/channelStrip.cpp \

# --------------------------------------------------------------
# Do some magic

include ../../dpf/Makefile.plugins.mk

ifeq ($(WIN32),true)
LINK_OPTS += -static -static-libgcc -static-libstdc++
endif

BUILD_CXX_FLAGS +=
# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(HAVE_DGL),true)
ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif
endif

ifeq ($(HAVE_DGL),true)
TARGETS += lv2_sep
else
TARGETS += lv2_dsp
endif

TARGETS += vst

all: $(TARGETS)

# --------------------------------------------------------------
