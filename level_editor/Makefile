CXX = clang++
OBJS = main.o
EXECNAME = level_editor
LIBS = -lncurses

all: $(EXECNAME)
	./$(EXECNAME)

objs/%.o: %.cpp
	$(CXX) -c -o $@ $< -Wall -g -std=c++0x

$(EXECNAME): $(OBJS)
	$(CXX) -o $@ $^ $(LIBS)

clean:
	-rm -f objs/*.o $(EXECNAME)

