OBJ = obj/framebufferer.o obj/graphics.o obj/interaction.o obj/main.o \
	  obj/voxelworld.o obj/window.o
EXECNAME = something
LDFLAGS = -lglfw -lGL -lGLEW
CXXFLAGS = -Wall -Wextra -g -std=c++0x `freetype-config --cflags`

default: objdir $(EXECNAME)
	./$(EXECNAME)

$(EXECNAME): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.cc
	$(CXX) -c -o $@ $< $(CXXFLAGS)

objdir:
	mkdir -p obj

.PHONY: clean
clean:
	-rm -f $(OBJ) $(EXECNAME)

