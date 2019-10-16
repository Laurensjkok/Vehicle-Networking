OPTFLAGS=-O3 -Wall -ffast-math -fsingle-precision-constant -std=c99


# code for each tile.
MB1_DIR=.tt_can_mb1
MB2_DIR=.tt_can_mb2
MB3_DIR=.tt_can_mb3
MB4_DIR=.tt_can_mb4
OBSERVER_DIR=.observer

# shared code and header files.
MB_SHARED=mb_shared

# force use of FPGA board connected to VGA framegrabber
USE_VGA_GRABBER?=0

# specify timeout
USER_TIMEOUT?=60

include .platform/platform.mk
