empty :=
space := $(empty) $(empty)

SRCDIR := src
INCDIR := include
BUILDDIR := build
OBJDIR := $(BUILDDIR)/obj
DEPDIR := $(BUILDDIR)/deps
TARGETDIR := $(BUILDDIR)/target
BINDIR := .

MY_FLAGS := -I$(BINDIR) -I$(INCDIR) $(shell pkg-config --cflags --libs opencv4)

CXX = clang++
CXXFLAGS = $(MY_FLAGS) -Wall -Wextra -Wno-unused-result -Wno-unused-command-line-argument -std=c++17
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
	CXXFLAGS += -Og -ggdb3 -DDEBUG=1
	# Overflow protection
	CXXFLAGS += -D_FORTIFY_SOURCE=2 -fstack-protector-strong -fcf-protection
	CXXFLAGS += -Wl,-z,defs -Wl,-z,now -Wl,-z,relro
	CXXFLAGS += -D_GLIBCXX_ASSERTIONS
	CXXFLAGS += -fsanitize=address -fsanitize=undefined -fsanitize=leak
endif

CXXFLAGS += -MMD -MP

TARGET := $(TARGETDIR)/$(maketype)/Face_ID

SRCS := $(wildcard $(SRCDIR)/*.cpp)
SRCS += $(wildcard $(SRCDIR)/**/*.cpp)

OBJS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/$(maketype)/%.o,$(SRCS))

.PHONY: all
all : $(TARGET)

.PHONY: getTarget
getTarget :
	@echo $(TARGET)

.PHONY: init
init :
	-@rm -rf build $(wildcard *.exe)
	@mkdir -p $(SRCDIR) $(INCDIR) $(OBJDIR)/{debug,release} $(DEPDIR) $(TARGETDIR)/{debug,release}
	@for i in $(wildcard *.cpp) $(wildcard *.c); do mv ./$$i $(SRCDIR)/$$i; done
	@for i in $(wildcard *.hpp) $(wildcard *.h); do mv ./$$i $(INCDIR)/$$i; done
	@$(file >compile_flags.txt)
	@$(foreach i,$(CXXFLAGS),$(file >>compile_flags.txt,$(i)))

$(TARGET) : $(OBJS)
	-@echo LD $(maketype) "ALL ->" $@ && \
		$(LD) -o $@ $(OBJS) $(LDFLAGS)

$(OBJDIR)/$(maketype)/%.cpp.o : $(SRCDIR)/%.cpp
	@$(eval CUR_DEP := $(patsubst $(SRCDIR)/%,$(DEPDIR)/%.d,$<))
	@mkdir -p $(@D) $(dir $(CUR_DEP))
	-@echo CXX $(maketype) $< "->" $@ && \
		$(CXX) -c $< -o $@ -MF $(CUR_DEP) $(CXXFLAGS)

DEPS := $(patsubst $(SRCDIR)/%,$(DEPDIR)/%.d,$(SRCS))

.PHONY: clean
clean : 
	-$(RM) $(OBJDIR)/$(maketype)/* $(DEPDIR)/* $(TARGET)

.PHONY: debug
debug : $(TARGET)
	@export ASAN_OPTIONS=detect_leaks=0; \
		$(DEBUGGER) $(TARGET)

-include $(DEPS)
