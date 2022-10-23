CC=g++

OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include -I./

BIN1=a2-model-rider
BIN2=a2-model-bike
BIN3=a2-model-track
SRCS1=rider_file.cpp gl_framework.cpp shader_util.cpp hierarchy_node.cpp shapes.cpp rider.cpp bike.cpp track.cpp
SRCS2=bike_file.cpp gl_framework.cpp shader_util.cpp hierarchy_node.cpp shapes.cpp rider.cpp bike.cpp track.cpp
SRCS3=track_file.cpp gl_framework.cpp shader_util.cpp hierarchy_node.cpp shapes.cpp rider.cpp bike.cpp track.cpp
INCLUDES=gl_framework.hpp shader_util.hpp 07_hierarchical_modelling.hpp hierarchy_node.hpp shapes.hpp rider.hpp bike.hpp track.hpp

all: $(BIN1) $(BIN2) $(BIN3)

$(BIN1): $(SRCS) $(INCLUDES)
	g++ $(CPPFLAGS) $(SRCS1) -o $(BIN1) $(LDFLAGS) $(LIBS)

$(BIN2): $(SRCS) $(INCLUDES)
	g++ $(CPPFLAGS) $(SRCS2) -o $(BIN2) $(LDFLAGS) $(LIBS)

$(BIN3): $(SRCS) $(INCLUDES)
	g++ $(CPPFLAGS) $(SRCS3) -o $(BIN3) $(LDFLAGS) $(LIBS)

clean:
	rm -f *~ *.o $(BIN1) $(BIN2) $(BIN3)
