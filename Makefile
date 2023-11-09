GCC := gcc
OPT := -g -Wall -Wextra -finput-charset=UTF-8 -Wno-unused-parameter -Wno-format-extra-args -Wformat=0
LDFLAGS :=
TARGET := woojinc

SRCDIR := src
INCDIR := $(SRCDIR)/../Include
BUILDDIR := build
SRC_FILES := $(wildcard $(SRCDIR)/*.c)
OBJ_FILES := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRC_FILES))
GCCFLAGS := $(OPT) -I$(INCDIR) -I$(SRCDIR)/../Environment -I..
TESTBUILDDIR := $(BUILDDIR)/test
TESTDIR := test
TEST_SUBDIRS := $(wildcard $(TESTDIR)/*)
TEST_FILES := $(wildcard $(TESTDIR)/*.c)
TEST_OBJ_FILES := $(patsubst $(TESTDIR)/%/%.c,$(TESTBUILDDIR)/%.o,$(TEST_FILES))

$(BUILDDIR)/$(TARGET): $(OBJ_FILES)
	$(GCC) $(GCCFLAGS) $^ -o $@
	del /Q build\*.o

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(GCC) $(GCCFLAGS) -c $< -o $@

define test-dir-rule
SUBDIR_OBJ := $$(patsubst $(1)/%.c,$(TESTBUILDDIR)/$(notdir $(1))/%.o,$$(wildcard $(1)/*.c))

$(TESTBUILDDIR)/$(notdir $(1)).exe: $$(SUBDIR_OBJ)
	$(GCC) $(GCCFLAGS) $$^ -o $$@

$$(SUBDIR_OBJ): $(TESTBUILDDIR)/$(notdir $(1))/%.o: $(1)/%.c
	@if not exist "$(TESTBUILDDIR)/$(notdir $(1))" mkdir "$(TESTBUILDDIR)/$(notdir $(1))"
	$(GCC) $(GCCFLAGS) -c $$< -o $$@
endef

$(foreach subdir,$(TEST_SUBDIRS),$(eval $(call test-dir-rule,$(subdir))))
test: $(patsubst $(TESTDIR)/%,$(TESTBUILDDIR)/%.exe,$(TEST_SUBDIRS))
	for /d %%i in (build\test\*) do rmdir /s /q "%%i"

clean:
	del /Q build\*
