# ----------------------------
# Makefile Options
# ----------------------------

NAME = CRSPGMLB
ICON = icon.png
DESCRIPTION = "Crisp Game Lib Portable TI84PCE edition"

COMPRESSED = YES
COMPRESSED_MODE = zx7
ARCHIVED = YES

# make printf available by debug.f
HAS_PRINTF = YES


CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

#EXTRA_C_SOURCES = ../../lib/cglp.c, ../../lib/cglp.h, ../../lib/menu.c, ../../lib/menuGameList.c, ../../lib/particle.c, ../../lib/random.c, ../../lib/sound.c, ../../lib/textPattern.c, ../../lib/vector.c
# ----------------------------

include $(shell cedev-config --makefile)
