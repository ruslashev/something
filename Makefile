CXX = clang++
OBJS = $(patsubst src/%.cpp, objs/%.o, $(shell find src -type f -name "*.cpp" ))
EXECNAME = something
LIBS = -lglfw -lGL -lGLEW

all: $(EXECNAME)
	./$(EXECNAME)

objs/%.o: src/%.cpp
	$(CXX) -c -o $@ $< -Wall -g -std=c++0x

$(EXECNAME): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	-rm -f objs/*.o $(EXECNAME)

