CC = gcc
AR = ar
ARFLAGS = rcs

CFLAGS = -Wall -Wextra -g -Iinclude
CFLAGS_DEBUG = $(CFLAGS) -DDEBUG
CFLAGS_SHARED = $(CFLAGS) -fPIC
CFLAGS_SHARED_DEBUG = $(CFLAGS_DEBUG) -fPIC

LDFLAGS =
LDFLAGS_SHARED = -shared

SRC_DIR = src
OBJ_DIR = obj
OBJ_DIR_DEBUG = obj_debug
OBJ_DIR_SHARED = obj_shared
OBJ_DIR_SHARED_DEBUG = obj_shared_debug
BIN_DIR = bin
LIB_DIR = lib

SRCS := $(shell find $(SRC_DIR) -type f -name '*.c')

OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
OBJS_DEBUG := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR_DEBUG)/%.o,$(SRCS))
OBJS_SHARED := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR_SHARED)/%.o,$(SRCS))
OBJS_SHARED_DEBUG := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR_SHARED_DEBUG)/%.o,$(SRCS))

TARGET = $(BIN_DIR)/resig
TARGET_DEBUG = $(BIN_DIR)/resig_debug

STATIC_LIB = $(LIB_DIR)/libresig.a
STATIC_LIB_DEBUG = $(LIB_DIR)/libresig_debug.a
SHARED_LIB = $(LIB_DIR)/libresig.so
SHARED_LIB_DEBUG = $(LIB_DIR)/libresig_debug.so

all: $(TARGET) $(STATIC_LIB) $(SHARED_LIB)

debug: $(TARGET_DEBUG) $(STATIC_LIB_DEBUG) $(SHARED_LIB_DEBUG)

# Executables

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

$(TARGET_DEBUG): $(OBJS_DEBUG)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

# Static libraries

$(STATIC_LIB): $(OBJS)
	@mkdir -p $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

$(STATIC_LIB_DEBUG): $(OBJS_DEBUG)
	@mkdir -p $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

# Shared libraries

$(SHARED_LIB): $(OBJS_SHARED)
	@mkdir -p $(LIB_DIR)
	$(CC) $(LDFLAGS_SHARED) -o $@ $^

$(SHARED_LIB_DEBUG): $(OBJS_SHARED_DEBUG)
	@mkdir -p $(LIB_DIR)
	$(CC) $(LDFLAGS_SHARED) -o $@ $^

# Object rules

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

$(OBJ_DIR_DEBUG)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_DEBUG) -MMD -MP -c -o $@ $<

$(OBJ_DIR_SHARED)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_SHARED) -MMD -MP -c -o $@ $<

$(OBJ_DIR_SHARED_DEBUG)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_SHARED_DEBUG) -MMD -MP -c -o $@ $<

# Utilities

clean:
	rm -rf $(OBJ_DIR) $(OBJ_DIR_DEBUG) \
	       $(OBJ_DIR_SHARED) $(OBJ_DIR_SHARED_DEBUG) \
	       $(BIN_DIR) $(LIB_DIR)

rebuild: clean all

run: $(TARGET)
	./$(TARGET)

run-debug: $(TARGET_DEBUG)
	./$(TARGET_DEBUG)

-include $(OBJS:.o=.d)
-include $(OBJS_DEBUG:.o=.d)
-include $(OBJS_SHARED:.o=.d)
-include $(OBJS_SHARED_DEBUG:.o=.d)

.PHONY: all clean rebuild run debug run-debug
