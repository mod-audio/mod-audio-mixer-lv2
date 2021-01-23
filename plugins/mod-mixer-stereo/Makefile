# --------------------------------------------------------------
#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = mod-mixer-stereo

# --------------------------------------------------------------
# Files to build

FILES_DSP  = \
	mod-mixer-stereo.cpp \
	../../classes/onepole.cpp \
	../../classes/panning.cpp \
	../../classes/volumeSlider.cpp \
	../../classes/channelStrip.cpp \
	../../classes/levelMeter.cpp \

# --------------------------------------------------------------
# Do some magic

include ../../dpf/Makefile.plugins.mk

ifeq ($(WIN32),true)
LINK_OPTS += -static -static-libgcc -static-libstdc++
endif

BUILD_CXX_FLAGS += -I../../classes
# --------------------------------------------------------------
# Enable all possible plugin types

TARGETS += lv2_dsp

all: $(TARGETS)

# --------------------------------------------------------------
