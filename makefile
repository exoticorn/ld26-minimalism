all: minimalism

minimalism: build/main.o build/cuberenderer.o build/levelcube.o build/game.o
	g++ $^ -lSDL -lGL -o $@

build/%.o: sources/%.cpp
	mkdir -p build
	g++ -g -c $< -MD -MF build/$*.d -o build/$*.o

minimalism.html: build-ems/main.o build-ems/cuberenderer.o build-ems/levelcube.o build-ems/game.o
	/home/dranke/tmp/emscripten/emcc -O2 -s ASM_JS=1 $^ -o $@

build-ems/%.o: sources/%.cpp
	mkdir -p build-ems
	/home/dranke/tmp/emscripten/emcc -DEMSCRIPTEN -c $< -MD -MF build-ems/$*.d -o build-ems/$*.o

clean:
	rm -rf build
	rm -rf build-ems
	rm -f minimalism
	rm -f minimalism.html

-include build/*.d

