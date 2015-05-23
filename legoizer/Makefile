CC := g++
CC := i686-w64-mingw32-g++ # Compiler for Windows MinGW64
# CC := clang --analyze # and comment out the linker last line for sanity

SRC_DIR := src
BUILD_DIR := build

PROJECT_TARGET := bin/legoizer
TEST_TARGET := bin/tester

SRC_EXT := cpp
SOURCES := $(shell find $(SRC_DIR) -type f -name *.$(SRC_EXT))
OBJECTS := $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SOURCES:.$(SRC_EXT)=.o))

CFLAGS := -g -Wall -std=c++0x
LDFLAGS :=

LIB_DIR := \
	-L lib \

LIB := \
	-l glfw3 \ # glfw3 on Cygwin (and I think MinGW toolchains need )
	-l opengl32 \
	-l gdi32 \

INC := \
	-I include \

$(PROJECT_TARGET): $(OBJECTS)
	@echo "    Linking..."
	@echo "    $(CC) $^ $(LIB_DIR) -o $(PROJECT_TARGET) $(LIB)"; $(CC) $(LDFLAGS) $^ $(LIB_DIR) -o $(PROJECT_TARGET) $(LIB)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.$(SRC_EXT)
	@mkdir -p $(@D)
	@echo "    $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo "    Cleaning...";
	@echo "    $(RM) -r $(BUILD_DIR) $(PROJECT_TARGET)"; $(RM) -r $(BUILD_DIR) $(PROJECT_TARGET)

test:
	$(CC) $(CFLAGS) test/tester.cpp $(INC) $(LIB) -o $(TEST_TARGET)

run: $(PROJECT_TARGET)
	$(PROJECT_TARGET) assets/minicooper.obj

.PHONY: clean

print-%: ; @echo $*=$($*)
