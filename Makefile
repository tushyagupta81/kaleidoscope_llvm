CC=g++

LLVM_CXXFLAGS := $(shell llvm-config --cxxflags)
LLVM_LDFLAGS  := $(shell llvm-config --ldflags --system-libs --libs core)

CFLAGS=-Wall -Wextra -Werror -I$(INCLUDE_DIR) -I$(UNITY_DIR) -MMD -MP $(LLVM_CXXFLAGS) -Wno-unused-parameter

OUTPUT_NAME=output

LDFLAGS = $(LLVM_LDFLAGS)

BUILD_DIR=build
SRC_DIR=src
INCLUDE_DIR=include
TEST_DIR=tests
UNITY_DIR=unity

SRCS=$(wildcard $(SRC_DIR)/*.cpp)
OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS=$(OBJS:.o=.d)
TARGET=$(BUILD_DIR)/$(OUTPUT_NAME)

TEST_SRCS=$(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS=$(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%.test.o,$(TEST_SRCS))
TEST_DEPS=$(TEST_OBJS:.o=.d)
TEST_TARGET=$(BUILD_DIR)/tests

UNITY_SRC = $(UNITY_DIR)/unity.cpp
UNITY_OBJ = $(BUILD_DIR)/unity.o

LIB_OBJS=$(filter-out $(BUILD_DIR)/main.o,$(OBJS))

default: all

all: $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.test.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(TEST_TARGET): $(LIB_OBJS) $(TEST_OBJS) $(UNITY_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -r ./build

run: all
	@./$(TARGET)

debug: CFLAGS += -g
debug: clean all

# test: $(TEST_TARGET)
# 	@./$(TEST_TARGET)

$(UNITY_OBJ): $(UNITY_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(UNITY_SRC) -o $(UNITY_OBJ)

-include $(DEPS)
