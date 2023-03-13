# Directory settings
SRCDIR := src
BUILDDIR := bin
TARGET := game

# Compiler and linker settings
# -MMD ensures dependency files (.d) are created
# -Wl,-rpath='$$ORIGIN' makes sure dynamic libs (.so) will be found in the app directory.
# 		Check with: 
# 		objdump -x bin/debug/game | grep RUNPATH
CXX := g++
CXXFLAGS := -std=c++17 -MMD
INCLUDES := -I${SRCDIR} -Ilib/glad/include -Ilib/stb_image -Ilib/irrKlang-1.6.0/include
LDFLAGS := -lglfw -lIrrKlang

# Platform specific settings
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	# LDFLAGS += -rpath @executable_path
	LDFLAGS += -framework OpenGL -Llib/irrKlang-1.6.0/bin/macosx-gcc
else
	LDFLAGS += -Wl,-rpath='$$ORIGIN'
	LDFLAGS += -Llib/irrKlang-1.6.0/bin/linux-gcc-64
endif

# Source and object file settings
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -name '*.$(SRCEXT)')
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# Debug build settings
DBGDIR := $(BUILDDIR)/debug
DBGOBJ := $(DBGDIR)/obj
DEBUG_TARGET := $(DBGDIR)/$(TARGET)
DEBUG := -O0 -g
DEBUG_OBJECTS := $(patsubst $(SRCDIR)/%, $(DBGOBJ)/%, $(SOURCES:.$(SRCEXT)=.o))

# Release build settings -- DNDEBUG will strip out all assert() calls
RELDIR := $(BUILDDIR)/release
RELOBJ := $(RELDIR)/obj
RELEASE_TARGET := $(RELDIR)/$(TARGET)
RELEASE := -O2 -ffast-math -DNDEBUG
RELEASE_OBJECTS := $(patsubst $(SRCDIR)/%, $(RELOBJ)/%, $(SOURCES:.$(SRCEXT)=.o))

# Targets
all: debug
debug: $(DEBUG_TARGET) copy_shaders_debug
release: $(RELEASE_TARGET) copy_shaders_release

# Checks whether dependency files have changed, see -MMD flag
-include $(RELEASE_OBJECTS:%.o=%.d)
-include $(DEBUG_OBJECTS:%.o=%.d)

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
	rm -rf $(RELOBJ)
	rm -rf $(DBGOBJ)

copy_shaders_debug:
	rsync -rup assets $(DBGDIR)
copy_shaders_release:
	rsync -rup assets $(RELDIR)

.PHONY: all debug release clean copy_shaders_debug copy_shaders_release
