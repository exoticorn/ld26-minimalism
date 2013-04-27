all: minimalism

minimalism: build/main.o
	g++ $^ -lSDL -lGL -o $@

build/%.o: sources/%.cpp
	mkdir -p build
	g++ -g -c $< -MD -MF build/$*.d -o build/$*.o

clean:
	rm -rf build

-include build/*.d

