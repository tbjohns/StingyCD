CPP_FILES = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
OBJ_FILES = $(patsubst src/%.cpp,obj/%.o,$(CPP_FILES))

FLAGS = -Wall -O3 

all: $(OBJ_FILES)
	$(CXX) $(FLAGS) -shared -o cd.so $^

obj/%.o: src/%.cpp
	$(CXX) $(FLAGS) -fPIC -c -o $@ $<

$(OBJ_FILES): | obj

obj:
	mkdir -p obj

clean:
	rm -rf obj
	rm cd.so
