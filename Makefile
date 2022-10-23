CC=g++

OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include -I./src/

BIN_DIR=./bin/*
BIN1=./bin/a2-model-rider
BIN2=./bin/a2-model-bike
BIN3=./bin/a2-model-track

SRCS1=./src/rider_file.cpp ./src/gl_framework.cpp ./src/shader_util.cpp ./src/hierarchy_node.cpp ./src/shapes.cpp ./src/rider.cpp ./src/bike.cpp ./src/track.cpp
SRCS2=./src/bike_file.cpp ./src/gl_framework.cpp ./src/shader_util.cpp ./src/hierarchy_node.cpp ./src/shapes.cpp ./src/rider.cpp ./src/bike.cpp ./src/track.cpp
SRCS3=./src/track_file.cpp ./src/gl_framework.cpp ./src/shader_util.cpp ./src/hierarchy_node.cpp ./src/shapes.cpp ./src/rider.cpp ./src/bike.cpp ./src/track.cpp
INCLUDES=./src/gl_framework.hpp ./src/shader_util.hpp ./src/07_hierarchical_modelling.hpp ./src/hierarchy_node.hpp ./src/shapes.hpp ./src/rider.hpp ./src/bike.hpp ./src/track.hpp

all: $(BIN1) $(BIN2) $(BIN3)

$(BIN1): $(SRCS1) $(INCLUDES)
	mkdir -p bin && g++ $(CPPFLAGS) $(SRCS1) -o $(BIN1) $(LDFLAGS) $(LIBS)

$(BIN2): $(SRCS1) $(INCLUDES)
	mkdir -p bin && g++ $(CPPFLAGS) $(SRCS2) -o $(BIN2) $(LDFLAGS) $(LIBS)

$(BIN3): $(SRCS1) $(INCLUDES)
	mkdir -p bin && g++ $(CPPFLAGS) $(SRCS3) -o $(BIN3) $(LDFLAGS) $(LIBS)

clean:
	rm -r ${BIN_DIR}
