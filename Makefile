empty :=
space := $(empty) $(empty)

SRCDIR := src
INCDIR := include
BUILDDIR := build
OBJDIR := $(BUILDDIR)/obj
DEPDIR := $(BUILDDIR)/deps
TARGETDIR := $(BUILDDIR)/target
EXAMPLESDIR := examples
BINDIR := .

MY_FLAGS := -I$(BINDIR) -I$(INCDIR)

# Packages used
MY_FLAGS += $(shell PKG_CONFIG_PATH=$$(find /opt -iname pkgconfig -printf '%p:' 2>/dev/null)$$PKG_CONFIG_PATH \
	pkg-config --cflags --libs openvino opencv4)

CXX = clang++
CXXFLAGS = $(MY_FLAGS) -Wall -Wextra -std=c++17
# Silence some warning messages
CXXFLAGS += -Wno-unused-command-line-argument
LD = clang++
LDFLAGS = $(CXXFLAGS)
DEBUGGER = lldb

ifeq ($(RELEASE), 1)
	maketype := release
	CXXFLAGS += -O2 -ftree-vectorize -fomit-frame-pointer -march=native
	# Link Time Optimization
	CXXFLAGS += -flto
else
	maketype := debug
	CXXFLAGS += -Og -ggdb3
	# Overflow protection
	CXXFLAGS += -D_FORTIFY_SOURCE=2 -fstack-protector-strong -fcf-protection
	CXXFLAGS += -Wl,-z,defs -Wl,-z,now -Wl,-z,relro
	CXXFLAGS += -D_GLIBCXX_ASSERTIONS
	CXXFLAGS += -fsanitize=address -fsanitize=undefined -fsanitize=leak
endif

CXXFLAGS += -MMD -MP

SHELL := /bin/bash

SRCS := $(shell find $(SRCDIR) -name *.cpp)

OBJS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/$(maketype)/%.o,$(SRCS))
DEPS := $(patsubst $(SRCDIR)/%,$(DEPDIR)/%.d,$(SRCS))

COMPILE = echo CXX $(maketype) $(2) "->" $(1) && \
		$(CXX) -c $(2) -o $(1) -MF $(3) $(CXXFLAGS)
LINK = echo LD $(maketype) $(1) $(2) && \
		$(LD) -o $(2) $(3) $(LDFLAGS)

$(EXAMPLESDIR)/% : $(OBJDIR)/$(maketype)/$(EXAMPLESDIR)/%.cpp.o $(filter-out $(OBJDIR)/$(maketype)/main.cpp.o,$(OBJS))
	-@$(call LINK,"",$@,$<)

.PRECIOUS: $(OBJDIR)/$(maketype)/$(EXAMPLESDIR)/%.cpp.o
$(OBJDIR)/$(maketype)/$(EXAMPLESDIR)/%.cpp.o : $(EXAMPLESDIR)/%.cpp
	@$(eval CUR_DEP := $(patsubst $(EXAMPLESDIR)/%,$(DEPDIR)/%.d,$<))
	@mkdir -p $(@D) $(dir $(CUR_DEP))
	@$(eval DEPS += $(CUR_DEP))
	-@$(call COMPILE,$@,$<,$(CUR_DEP))
	
TARGET := $(TARGETDIR)/$(maketype)/Face_ID

.PHONY: all
all : $(TARGET)

.PHONY: getTarget
getTarget :
	@echo $(TARGET)

.PHONY: init
init :
	-@rm -rf build
	@mkdir -p $(SRCDIR) $(INCDIR) $(EXAMPLESDIR)
	@for i in $(wildcard *.cpp); do mv ./$$i $(SRCDIR)/$$i; done
	@for i in $(wildcard *.hpp); do mv ./$$i $(INCDIR)/$$i; done
	@$(file >compile_flags.txt)
	@$(foreach i,$(CXXFLAGS),$(file >>compile_flags.txt,$(i)))

$(TARGET) : $(OBJS)
	@mkdir -p $(@D)
	-@$(call LINK,"ALL ->",$@,$(OBJS))

$(OBJDIR)/$(maketype)/%.cpp.o : $(SRCDIR)/%.cpp
	@$(eval CUR_DEP := $(patsubst $(SRCDIR)/%,$(DEPDIR)/%.d,$<))
	@mkdir -p $(@D) $(dir $(CUR_DEP))
	-@$(call COMPILE,$@,$<,$(CUR_DEP))

.PHONY: clean
clean : 
	-rm -rf $(OBJDIR)/$(maketype)/* $(DEPDIR)/* $(TARGETDIR)/$(maketype)/*

.PHONY: debug
debug : $(TARGET)
	@export ASAN_OPTIONS=detect_leaks=0; \
		$(DEBUGGER) $(TARGET)

-include $(DEPS)
