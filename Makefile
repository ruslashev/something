CXX = clang++
OBJS = src/main.o src/framebufferer.o src/camera.o src/graphics.o src/window.o
EXECNAME = something
LIBS = -lglfw -lGL -lGLEW

all: $(EXECNAME)
	./$(EXECNAME)

objs/%.o: src/%.cpp
	$(CXX) -c -o $@ $< -Wall -g -std=c++0x

$(EXECNAME): $(OBJS)
	$(CXX) -o $@ $^ $(LIBS)

clean:
	-rm -f objs/*.o $(EXECNAME)

