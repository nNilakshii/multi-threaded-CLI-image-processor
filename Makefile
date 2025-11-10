CXX := xcrun --sdk macosx clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2 -stdlib=libc++
LDFLAGS := 

TARGET := image_processor
SRC_DIR := src
OBJ_DIR := build
INCLUDE_DIR := include
TEST_TARGET := cli_parser_tests
TEST_SRC := tests/test_cli_parser.cpp
TEST_OBJ := $(patsubst tests/%.cpp,$(OBJ_DIR)/tests/%.o,$(TEST_SRC))

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

all: $(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/tests/%.o: tests/%.cpp | $(OBJ_DIR)/tests
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(OBJ_DIR)/tests:
	@mkdir -p $(OBJ_DIR)/tests

$(TEST_TARGET): $(OBJ_DIR)/CLIParser.o $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(TEST_TARGET)

.PHONY: all clean test
