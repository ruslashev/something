CXX = clang++
OBJS = objs/main.o objs/framebufferer.o objs/camera.o objs/graphics.o objs/window.o
EXECNAME = something
LIBS = -lglfw -lGL -lGLEW

all: $(EXECNAME)
	./$(EXECNAME)

$(EXECNAME): $(OBJS)
	$(CXX) -o $@ $^ $(LIBS)

objs/%.o: src/%.cpp
	$(CXX) -c -o $@ $< -Wall -g -std=c++0x

clean:
	-rm -f objs/*.o $(EXECNAME)

