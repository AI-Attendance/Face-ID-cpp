empty :=
space := $(empty) $(empty)

SRCDIR := src
INCDIR := include
OBJDIR := build/obj
DEPDIR := build/deps
BINDIR1 := .
BINDIR2 := ..

TARGET := $(BINDIR1)/Face_ID_
ifeq ($(RELEASE), 1)
	TARGET := $(TARGET)release
else
	TARGET := $(TARGET)debug
endif

MY_FLAGS := -I$(BINDIR1) -I$(INCDIR) $(shell pkg-config --cflags --libs opencv4)

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
	CXXFLAGS += -D_FORTIFY_SOURCE=2 -fstack-protector-strong -fstack-clash-protection -fcf-protection
	CXXFLAGS += -Wl,-z,defs -Wl,-z,now -Wl,-z,relro
	CXXFLAGS += -D_GLIBCXX_ASSERTIONS
	CXXFLAGS += -fsanitize=address -fsanitize=undefined -fsanitize=leak
endif

CXXFLAGS += -MMD -MP

SRCS := $(wildcard $(BINDIR2)/$(SRCDIR)/*.cpp)
SRCS += $(wildcard $(BINDIR2)/$(SRCDIR)/**/*.cpp)
SRCS := $(filter-out $(BINDIR2)/$(SRCDIR)/main.cpp,$(SRCS))

OBJS := $(patsubst $(BINDIR2)/$(SRCDIR)/%,$(BINDIR2)/$(OBJDIR)/$(maketype)/%.o,$(SRCS))
DEPS := $(patsubst $(BINDIR2)/$(SRCDIR)/%,$(BINDIR2)/$(DEPDIR)/%.d,$(SRCS))

SRCS := $(wildcard $(SRCDIR)/*.cpp)
SRCS += $(wildcard $(SRCDIR)/**/*.cpp)

OBJS += $(patsubst $(SRCDIR)/%,$(OBJDIR)/$(maketype)/%.o,$(SRCS))
DEPS += $(patsubst $(SRCDIR)/%,$(DEPDIR)/%.d,$(SRCS))

.PHONY: all
all : $(TARGET)

.PHONY: getTarget
getTarget :
	@echo $(TARGET)

.PHONY: init
init :
	-@rm -rf build $(wildcard *.exe)
	@mkdir -p $(SRCDIR) $(INCDIR) $(OBJDIR)/{debug,release} $(DEPDIR)
	@mkdir -p $(BINDIR2)/{$(SRCDIR),$(INCDIR),$(OBJDIR)/{debug,release},$(DEPDIR)}
	@for i in $(wildcard *.cpp) $(wildcard *.c); do mv ./$$i $(SRCDIR)/$$i; done
	@for i in $(wildcard *.hpp) $(wildcard *.h); do mv ./$$i $(INCDIR)/$$i; done
	@$(file >compile_flags.txt)
	@$(foreach i,$(CXXFLAGS),$(file >>compile_flags.txt,$(i)))

$(TARGET) : $(OBJS)
	-@echo LD $(maketype) "ALL ->" $@ && \
		$(LD) -o $@ $(OBJS) $(LDFLAGS)

CREATE = mkdir -p $(1) $(dir $(2))
COMPILE = echo CXX $(maketype) $(1) "->" $(2) && \
						$(CXX) -c $(1) -o $(2) -MF $(3) $(CXXFLAGS)

$(OBJDIR)/$(maketype)/%.cpp.o : $(SRCDIR)/%.cpp
	@$(eval CUR_DEP := $(patsubst $(SRCDIR)/%,$(DEPDIR)/%.d,$<))
	@$(call CREATE,$(@D),$(CUR_DEP))
	-@$(call COMPILE,$<,$@,$(CUR_DEP))

$(BINDIR2)/$(OBJDIR)/$(maketype)/%.cpp.o : $(BINDIR2)/$(SRCDIR)/%.cpp
	@$(eval CUR_DEP := $(patsubst $(BINDIR2)/$(SRCDIR)/%,$(BINDIR2)/$(DEPDIR)/%.d,$<))
	@$(call CREATE,$(@D),$(CUR_DEP))
	-@$(call COMPILE,$<,$@,$(CUR_DEP))

.PHONY: clean
clean : 
	-$(RM) $(OBJDIR)/$(maketype)/* $(DEPDIR)/* $(TARGET)

.PHONY: debug
debug : $(TARGET)
	@export ASAN_OPTIONS=detect_leaks=0; \
		$(DEBUGGER) $(TARGET)

-include $(DEPS)
