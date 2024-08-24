
UNAME := $(shell uname)

CC := clang

TARGET := main

SRC_DIR := ./src
INCLUDE_DIR := ./include

SOURCES := $(shell find $(SRC_DIR)/* -name '*.c')
OBJECTS := $(patsubst $(SRC_DIR)/%,$(SRC_DIR)/%,$(SOURCES:.c=.o))

INCLUDE_FLAGS := -I$(INCLUDE_DIR)

LINK_FLAGS := -lSDL2
LINK_FLAGS += -lSDL2_ttf
LINK_FLAGS += -lSDL2_image
LINK_FLAGS += -lSDL2_mixer
LINK_FLAGS += -lSDL2_gfx

CFLAGS := -Wall
CFLAGS += $(shell sdl2-config --cflags)

ifeq ("$(UNAME)", Darwin)
	CPATH := $(shell brew --prefix)/include
endif

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(LINK_FLAGS) -o $(TARGET) $^
	@echo "Done!"

clean:
	rm -rf $(OBJECTS) $(TARGET)

