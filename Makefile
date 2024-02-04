APP_NAME := game
SOURCE_DIR := src
BUILD_DIR := bin
ASSET_DIR := assets

# Compiler flags
CXX := clang++ # g++
CXXFLAGS := -std=c++20 -MMD # The -MMD flag ensures dependency files (.d) are created
DEFINES := -D_USE_MATH_DEFINES # -DGLM_FORCE_INTRINSICS

# Dependencies / libraries
INCLUDES := -I${SOURCE_DIR} \
	-Ilib/glad/include \
	-Ilib/stb_image \
	-Ilib/alure/include \
	-Ilib/objload 
LDFLAGS := -lSDL2 -lalure2 # -lSDL2main not required?

# MacOS specific
ifeq ($(UNAME_S), Darwin)
	LDFLAGS += -framework OpenGL
endif

# Precompiled headers -- also see -Winvalid-pch 
# PCH_SRC = src/common.h
# PCH_OUT = src/common.h.gch

# Source and object file settings
SRCEXT := cpp
SOURCES := $(shell find $(SOURCE_DIR) -name '*.$(SRCEXT)')
OBJECTS := $(patsubst $(SOURCE_DIR)/%,$(BUILD_DIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# Debug build settings
DBGDIR := $(BUILD_DIR)/debug
DBGOBJ := $(DBGDIR)/obj
# DEBUG_PCH = $(DBGOBJ)/$(PCH_OUT)
DEBUG_TARGET := $(DBGDIR)/$(APP_NAME)
DEBUG := $(DEFINES) -O0 -g -DDEBUG_BUILD
DEBUG_OBJECTS := $(patsubst $(SOURCE_DIR)/%, $(DBGOBJ)/%, $(SOURCES:.$(SRCEXT)=.o))

# Release build settings
RELDIR := $(BUILD_DIR)/release
RELOBJ := $(RELDIR)/obj
# RELEASE_PCH = $(RELOBJ)/$(PCH_OUT)
RELEASE_TARGET := $(RELDIR)/$(APP_NAME)
RELEASE := $(DEFINES) -O2 -DNDEBUG # NDEBUG will strip out all assert() calls
RELEASE_OBJECTS := $(patsubst $(SOURCE_DIR)/%, $(RELOBJ)/%, $(SOURCES:.$(SRCEXT)=.o))

# Checks whether dependency files have changed, see -MMD flag
-include $(RELEASE_OBJECTS:%.o=%.d)
-include $(DEBUG_OBJECTS:%.o=%.d)

# $(DEBUG_PCH): $(PCH_SRC)
# 	$(CXX) -std=c++20 $(DEBUG) $(INCLUDES) -o $@ $<
# $(RELEASE_PCH): $(PCH_SRC)
# 	$(CXX) -std=c++20 $(RELEASE) $(INCLUDES) -o $@ $<

# Targets
all: debug # Default to the debug build if no target is specified
debug: $(DEBUG_TARGET) copy_assets_debug
release: $(RELEASE_TARGET) copy_assets_release
# pch: mkdir $(DEBUG_PCH) $(RELEASE_PCH)

$(DEBUG_TARGET): $(DEBUG_OBJECTS)
	$(CXX) $(DEBUG) $^ $(LDFLAGS) -o $@

$(RELEASE_TARGET): $(RELEASE_OBJECTS)
	$(CXX) $(RELEASE) $^ $(LDFLAGS) -o $@

$(DBGOBJ)/%.o: $(SOURCE_DIR)/%.$(SRCEXT)
	mkdir -p $(dir $(subst $(SOURCE_DIR),$(DBGOBJ),$@))
	$(CXX) $(CXXFLAGS) $(DEBUG) $(INCLUDES) -c -o $@ $<
$(RELOBJ)/%.o: $(SOURCE_DIR)/%.$(SRCEXT)
	mkdir -p $(dir $(subst $(SOURCE_DIR),$(RELOBJ),$@))
	$(CXX) $(CXXFLAGS) $(RELEASE) $(INCLUDES) -c -o $@ $<

clean: 
	rm -rf $(RELDIR)
	rm -rf $(DBGDIR)

copy_assets_debug: 
	ln -s $(CURDIR)/$(ASSET_DIR) $(CURDIR)/$(DBGDIR)/$(ASSET_DIR)

copy_assets_release: 
	ln -s $(CURDIR)/$(ASSET_DIR) $(CURDIR)/$(RELDIR)/$(ASSET_DIR)

.PHONY: debug release clean copy_assets_debug copy_assets_release
