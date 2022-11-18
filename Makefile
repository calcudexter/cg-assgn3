CC=g++

OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include -I./src/

BIN_DIR=./bin/*
BIN3=./bin/fmx-animate

SRCS3=./src/track_file.cpp ./src/gl_framework.cpp ./src/shader_util.cpp ./src/hierarchy_node.cpp ./src/shapes.cpp ./src/rider.cpp ./src/bike.cpp ./src/track.cpp ./src/texture.cpp ./src/stbi.cpp
INCLUDES=./src/gl_framework.hpp ./src/shader_util.hpp ./src/07_hierarchical_modelling.hpp ./src/hierarchy_node.hpp ./src/shapes.hpp ./src/rider.hpp ./src/bike.hpp ./src/track.hpp ./src/texture.hpp ./src/stb_image.h 

all: $(BIN3)

$(BIN3): $(SRCS3) $(INCLUDES)
	mkdir -p bin && g++ $(CPPFLAGS) $(SRCS3) -o $(BIN3) $(LDFLAGS) $(LIBS)

clean:
	rm -r ${BIN_DIR}
