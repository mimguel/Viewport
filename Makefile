BUILDDIR := ../build
PROJECT_NAME := Viewport
TARGET := $(BUILDDIR)/$(PROJECT_NAME)

CC := g++

CFLAGS := -I../libraries/Include/SDL2
LDFLAGS := -L../libraries/lib -lmingw32 -lSDL2main -lSDL2 -lopengl32 -luser32 -std=c++17

SRCS := $(wildcard ../src/*.cpp) $(wildcard ./Window/*.cpp) $(wildcard ./Compiler/*.cpp) $(wildcard ./Graphics/*.cpp) $(wildcard ./Events/*.cpp) $(wildcard ./Objects/*.cpp) $(wildcard ../imgui/*.cpp)

$(TARGET): $(SRCS)
	cls
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)
	../build/$(PROJECT_NAME)

clean:
	rm -f $(TARGET)

git: 
	git add .
	git commit -m "commit"
	git push -u origin main

.PHONY: clean run
