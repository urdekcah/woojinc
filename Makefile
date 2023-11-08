GCC := gcc
OPT := -g -Wall -Wextra -finput-charset=UTF-8 -Wno-unused-parameter -Wno-format-extra-args -Wformat=0 # -Wall -Wextra -std=c++17 -finput-charset=UTF-8
LDFLAGS :=
TARGET := woojinc

SRCDIR := src
INCDIR := $(SRCDIR)/../Include
BUILDDIR := build
SRC_FILES := $(wildcard $(SRCDIR)/*.c)
OBJ_FILES := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRC_FILES))
GCCFLAGS := $(OPT) -I$(INCDIR) -I$(SRCDIR)/../Environment -I..

$(BUILDDIR)/$(TARGET): $(OBJ_FILES)
	$(GCC) $(GCCFLAGS) $^ -o $@
	del /Q build\*.o

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(GCC) $(GCCFLAGS) -c $< -o $@

clean:
	del /Q build\*