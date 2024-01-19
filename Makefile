# Directory settings
SRCDIR := src
BUILDDIR := bin
TARGET := game

# Compiler and linker settings
# The -MMD flag ensures dependency files (.d) are created
CXX := clang++ # g++
CXXFLAGS := -std=c++20 -MMD
INCLUDES := -I${SRCDIR} \
	-Ilib/glad/include \
	-Ilib/stb_image \
	-Ilib/alure/include \
	-Ilib/objload 
LDFLAGS := -lSDL2 -lalure2 # -lSDL2main not required?

# Platform specific settings
UNAME_S := $(shell uname -s)

# MacOS
ifeq ($(UNAME_S), Darwin)
	LDFLAGS += -framework OpenGL
endif

# Precompiled headers -- also see -Winvalid-pch 
# PCH_SRC = src/common.h
# PCH_OUT = src/common.h.gch

# Source and object file settings
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -name '*.$(SRCEXT)')
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# Debug build settings
DBGDIR := $(BUILDDIR)/debug
DBGOBJ := $(DBGDIR)/obj
# DEBUG_PCH = $(DBGOBJ)/$(PCH_OUT)
DEBUG_TARGET := $(DBGDIR)/$(TARGET)
DEBUG := -O0 -g
DEBUG_OBJECTS := $(patsubst $(SRCDIR)/%, $(DBGOBJ)/%, $(SOURCES:.$(SRCEXT)=.o))

# Release build settings
RELDIR := $(BUILDDIR)/release
RELOBJ := $(RELDIR)/obj
# RELEASE_PCH = $(RELOBJ)/$(PCH_OUT)
RELEASE_TARGET := $(RELDIR)/$(TARGET)
RELEASE := -O2 -ffast-math -DNDEBUG # DNDEBUG will strip out all assert() calls
RELEASE_OBJECTS := $(patsubst $(SRCDIR)/%, $(RELOBJ)/%, $(SOURCES:.$(SRCEXT)=.o))

# Targets
all: debug
# pch: mkdir $(DEBUG_PCH) $(RELEASE_PCH)
debug: mkdir $(DEBUG_TARGET) copy_assets_debug
release: mkdir $(RELEASE_TARGET) copy_assets_release

# Checks whether dependency files have changed, see -MMD flag
-include $(RELEASE_OBJECTS:%.o=%.d)
-include $(DEBUG_OBJECTS:%.o=%.d)

# $(DEBUG_PCH): $(PCH_SRC)
# 	$(CXX) -std=c++20 $(DEBUG) $(INCLUDES) -o $@ $<
# $(RELEASE_PCH): $(PCH_SRC)
# 	$(CXX) -std=c++20 $(RELEASE) $(INCLUDES) -o $@ $<

$(DEBUG_TARGET): $(DEBUG_OBJECTS)
	$(CXX) $(DEBUG) $^ $(LDFLAGS) -o $@

$(RELEASE_TARGET): $(RELEASE_OBJECTS)
	$(CXX) $(RELEASE) $^ $(LDFLAGS) -o $@

$(DBGOBJ)/%.o: $(SRCDIR)/%.$(SRCEXT)
	mkdir -p $(dir $(subst $(SRCDIR),$(DBGOBJ),$@))
	$(CXX) $(CXXFLAGS) $(DEBUG) $(INCLUDES) -c -o $@ $<
$(RELOBJ)/%.o: $(SRCDIR)/%.$(SRCEXT)
	mkdir -p $(dir $(subst $(SRCDIR),$(RELOBJ),$@))
	$(CXX) $(CXXFLAGS) $(RELEASE) $(INCLUDES) -c -o $@ $<

clean:
	rm -rf $(RELDIR)
	rm -rf $(DBGDIR)

mkdir:
	mkdir -p $(DBGOBJ)
	mkdir -p $(RELOBJ)

copy_assets_debug:
	rsync -ru --exclude='*.blend' assets $(DBGDIR)
copy_assets_release:
	rsync -ru --exclude='*.blend' assets $(RELDIR)

.PHONY: all debug release mkdir clean copy_assets_debug copy_assets_release
