# nRF application makefile
PROJECT_NAME = $(shell basename "$(realpath ./)")

# Configurations
NRF_IC = nrf52832
SDK_VERSION = 15
SOFTDEVICE_MODEL = blank

# Source and header files with yakindu stuff
APP_HEADER_PATHS += . src-gen/ src/ 
APP_SOURCE_PATHS += . src-gen/
APP_SOURCES = $(notdir $(wildcard ./*.c)) $(notdir $(wildcard src-gen/*.c))

# Path to base of nRF52-base repo
NRF_BASE_DIR = ../../nrf52x-base/

# Include board Makefile (if any)
include ../../boards/buckler_revC/Board.mk

# Include main Makefile
include $(NRF_BASE_DIR)make/AppMakefile.mk
