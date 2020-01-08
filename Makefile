CPP_FILES = $(wildcard stingycd/src/*.cpp) $(wildcard stingycd/src/*/*.cpp)
OBJ_FILES = $(patsubst stingycd/src/%.cpp,stingycd/obj/%.o,$(CPP_FILES))

FLAGS = -Wall -O3

all: $(OBJ_FILES)
	$(CXX) $(FLAGS) -shared -o stingycd/cd.so $^

stingycd/obj/%.o: stingycd/src/%.cpp
	$(CXX) $(FLAGS) -fPIC -c -o $@ $<

$(OBJ_FILES): | stingycd/obj

stingycd/obj:
	mkdir -p stingycd/obj

clean:
	rm -rf stingycd/obj
	rm stingycd/cd.so
