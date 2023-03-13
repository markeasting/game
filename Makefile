# CC=gcc
# CCX=g++
# SRCDIR=src
# IDIR=lib

# # GLAD was hard to install / compile, so I just added the C file as CPP file in the src dir.
# # It works and I don't care!
# # $(CC) $(CSOURCES) $(CFLAGS) -o $(BUILD_DIR_RELEASE)/src/glad.o
# GLAD_INC = $(IDIR)/glad/include
# GLM_INC = $(IDIR)/glm-0.9.9.8/glm
# CFLAGS = -Wall -c -I$(GLAD_INC)

# CPPFLAGS_RELEASE=-I$(SRCDIR) -I$(IDIR) -I$(GLAD_INC) -I$(GLM_INC) -O2 -ffast-math -std=c++17
# CPPFLAGS_DEBUG=-I$(SRCDIR) -I$(IDIR) -I$(GLAD_INC) -I$(GLM_INC) -O0 -g -std=c++17

# UNAME_S := $(shell uname -s)
# ifeq ($(UNAME_S), Darwin)
# 	# LDFLAGS=-framework OpenGL -lm -lglfw -lglew
# 	LDFLAGS=-framework OpenGL -lglfw
# else
# 	# LDFLAGS=-lm -lglfw -lGLEW -lGL
# 	LDFLAGS=-lglfw
# endif

# # Final binary
# BIN = game
# # Put all auto generated stuff to this build dir.
# BUILD_DIR=./bin
# BUILD_DIR_DEBUG = $(BUILD_DIR)/debug
# BUILD_DIR_RELEASE = $(BUILD_DIR)/release

# CSOURCES = lib/glad/src/glad.c

# # List of all .cpp source files.
# CPP = $(wildcard src/*.cpp) \
# 	$(wildcard src/app/*.cpp) \
# 	$(wildcard src/core/*.cpp) \
# 	$(wildcard src/geom/*.cpp) \
# 	$(wildcard src/camera/*.cpp) \
# 	$(wildcard src/gfx/*.cpp) \
# 	$(wildcard src/input/*.cpp) \
# 	$(wildcard src/math/*.cpp) \
# 	$(wildcard src/event/*.cpp) \
# 	$(wildcard src/physics/*.cpp) \
# 	$(wildcard src/scene/*.cpp) \
# 	$(wildcard src/util/*.cpp)

# # All .o files go to build dir.
# RELEASE_OBJ = $(CPP:%.cpp=$(BUILD_DIR_RELEASE)/%.o)
# DEBUG_OBJ = $(CPP:%.cpp=$(BUILD_DIR_DEBUG)/%.o)
# # Gcc/Clang will create these .d files containing dependencies.
# RELEASE_DEP = $(RELEASE_OBJ:%.o=%.d)
# DEBUG_DEP = $(DEBUG_OBJ:%.o=%.d)

# # Default target named after the binary.
# release : $(BUILD_DIR_RELEASE)/$(BIN)
# debug : $(BUILD_DIR_DEBUG)/$(BIN)

# # Actual target of the binary - depends on all .o files.
# $(BUILD_DIR_RELEASE)/$(BIN) : $(RELEASE_OBJ)
# 	# Create build directories - same structure as sources.
# 	mkdir -p $(@D)
# 	# Just link all the object files.
# 	$(CCX) $(CPPFLAGS_RELEASE) $^ $(LDFLAGS) -o $@

# $(BUILD_DIR_DEBUG)/$(BIN) : $(DEBUG_OBJ)
# 	# Create build directories - same structure as sources.
# 	mkdir -p $(@D)
# 	# Just link all the object files.
# 	$(CCX) $(CPPFLAGS_DEBUG) $^ $(LDFLAGS) -o $@

# # Include all .d files
# -include $(RELEASE_DEP)
# -include $(DEBUG_DEP)

# # Build target for every single object file.
# # The potential dependency on header files is covered
# # by calling `-include $(DEP)`.
# # The -MMD flags additionaly creates a .d file with
# # the same name as the .o file.
# $(BUILD_DIR_RELEASE)/%.o : %.cpp
# 	mkdir -p $(@D)
# 	$(CCX) $(CPPFLAGS_RELEASE) -MMD -c $< -o $@

# $(BUILD_DIR_DEBUG)/%.o : %.cpp
# 	mkdir -p $(@D)
# 	$(CCX) $(CPPFLAGS_DEBUG) -MMD -c $< -o $@

# .PHONY : clean
# clean :
# 	# This should remove all generated files.
# 	-rm -rf $(BUILD_DIR_RELEASE)/src
# 	-rm -rf $(BUILD_DIR_DEBUG)/src

# Directory settings
SRCDIR := src
BUILDDIR := bin
TARGET := game

# Compiler and linker settings
# The -MMD flag ensures dependency files (.d) are created
CXX := g++
CXXFLAGS := -std=c++17 -MMD
INCLUDES := -I${SRCDIR} -Ilib/glad/include -Ilib/stb_image 
LDFLAGS := -lglfw

# Platform specific settings
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	LDFLAGS += -framework OpenGL
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
