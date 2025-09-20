CC=gcc
CFLAGS=-Wall -Wextra -Werror -I$(INCLUDE_DIR) -I$(UNITY_DIR) -MMD -MP

OUTPUT_NAME=output

BUILD_DIR=build
SRC_DIR=src
INCLUDE_DIR=include
TEST_DIR=tests
UNITY_DIR=unity

SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS=$(OBJS:.o=.d)
TARGET=$(BUILD_DIR)/$(OUTPUT_NAME)

TEST_SRCS=$(wildcard $(TEST_DIR)/*.c)
TEST_OBJS=$(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%.test.o,$(TEST_SRCS))
TEST_DEPS=$(TEST_OBJS:.o=.d)
TEST_TARGET=$(BUILD_DIR)/tests

UNITY_SRC = $(UNITY_DIR)/unity.c
UNITY_OBJ = $(BUILD_DIR)/unity.o

LIB_OBJS=$(filter-out $(BUILD_DIR)/main.o,$(OBJS))

default: all

all: $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.test.o: $(TEST_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(TEST_TARGET): $(LIB_OBJS) $(TEST_OBJS) $(UNITY_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -r ./build

run: all
	@./$(TARGET)

test: $(TEST_TARGET)
	@./$(TEST_TARGET)

$(UNITY_OBJ): $(UNITY_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(UNITY_SRC) -o $(UNITY_OBJ)

-include $(DEPS)
