MICROPYTHON_TOP = ../micropython

# Include the main makefile fragment to build the MicroPython component.

USER_C_MODULES=displayMod/
include $(MICROPYTHON_TOP)/ports/embed/embed.mk
