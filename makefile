all: minimalism

minimalism: build/main.o build/cuberenderer.o build/levelcube.o build/game.o build/player.o
	g++ $^ -lSDL -lGL -o $@

build/%.o: sources/%.cpp
	mkdir -p build
	g++ -g -c $< -MD -MF build/$*.d -o build/$*.o

minimalism.html: build-ems/main.o build-ems/cuberenderer.o build-ems/levelcube.o build-ems/game.o build-ems/player.o shell.html
	/home/dranke/tmp/emscripten/emcc --shell-file shell.html -O2 -s ASM_JS=1 $^ -o $@

build-ems/%.o: sources/%.cpp
	mkdir -p build-ems
	/home/dranke/tmp/emscripten/emcc -DEMSCRIPTEN -c $< -MD -MF build-ems/tmp.d -o build-ems/$*.o
	sed -e "s/^.*\.o:/build-ems\/$*.o:/" build-ems/tmp.d > build-ems/$*.d
	rm build-ems/tmp.d

publish: minimalism.html
	cp minimalism.html gh-pages/index.html
	cd gh-pages && git commit -a -m "new build"
	cd gh-pages && git push origin gh-pages

clean:
	rm -rf build
	rm -rf build-ems
	rm -f minimalism
	rm -f minimalism.html

-include build/*.d
-include build-ems/*.d
