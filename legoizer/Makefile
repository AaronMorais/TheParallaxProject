print-%: ; @echo $*=$($*)

CC := g++
CC := i686-w64-mingw32-g++ # Compiler for Windows MinGW64
# CC := clang --analyze # and comment out the linker last line for sanity

ASSETS_DIR := assets
BIN_DIR := bin
SRC_DIR := src
BUILD_DIR := build

PROJECT_TARGET := $(BIN_DIR)/legoizer
TEST_TARGET := $(BIN_DIR)/tester

PROJECT_OBJECT := $(BUILD_DIR)/main.o
TEST_OBJECT := $(BUILD_DIR)/tester.o

SRC_EXT := cpp
SOURCES := $(shell find $(SRC_DIR) -type f -name *.$(SRC_EXT))
OBJECTS := $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SOURCES:.$(SRC_EXT)=.o))

CFLAGS := -g -Wall -std=c++0x
LDFLAGS := # --verbose

LIB_DIR := \
	-L lib \
	-L /lib/w32api \

# glfw3 on Cygwin (and I think MinGW toolchains need )
LIB := \
	-l glew32 \
	-l glfw3 \
	-l opengl32 \
	-l gdi32 \
	-l glu32 \

INC := \
	-I include \
	-L /usr/include/w32api \

all: $(PROJECT_TARGET) $(TEST_TARGET)

$(PROJECT_TARGET): $(filter-out $(TEST_OBJECT), $(OBJECTS))
	@echo "    Linking..."
	@echo "    $(CC) $^ $(LIB_DIR) -o $(PROJECT_TARGET) $(LIB)"; $(CC) $(LDFLAGS) $^ $(LIB_DIR) -o $(PROJECT_TARGET) $(LIB)

$(TEST_TARGET): $(filter-out $(PROJECT_OBJECT), $(OBJECTS))
	@echo "    Linking..."
	@echo "    $(CC) $^ $(LIB_DIR) -o $(PROJECT_TARGET) $(LIB)"; $(CC) $(LDFLAGS) $^ $(LIB_DIR) -o $(TEST_TARGET) $(LIB)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.$(SRC_EXT)
	@mkdir -p $(@D)
	@echo "    $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo "    Cleaning...";
	@echo "    $(RM) -r $(BUILD_DIR) $(PROJECT_TARGET) $(TEST_TARGET)"; $(RM) -r $(BUILD_DIR) $(PROJECT_TARGET) $(TEST_TARGET)

test: $(TEST_TARGET)

run: $(PROJECT_TARGET)
	$(PROJECT_TARGET) $(ASSETS_DIR)/teapot.obj

run-tests: $(TEST_TARGET)
	$(TEST_TARGET) $(ASSETS_DIR)/teapot.obj

.PHONY: all clean test run run-tests
