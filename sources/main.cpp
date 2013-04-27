#include <SDL/SDL.h>
#include <GL/gl.h>
#ifdef EMSCRIPTEN
#	include <emscripten.h>
#endif

static bool quit = false;

void run();

int main() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* pScreen = SDL_SetVideoMode(800, 600, 32, SDL_OPENGL);

#ifdef EMSCRIPTEN
	emscripten_set_main_loop(run, 60, true);
#else
	while(!quit) {
		run();
	}
#endif

	SDL_Quit();
}

void run() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym) {
			case SDLK_ESCAPE:
				quit = true;
				break;
			}
			break;
		}
	}

	static float c = 0;
	c += 0.01f;
	if(c > 1) c = 0;
	glClearColor(0.125f, c, 0.125f, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	SDL_GL_SwapBuffers();
}
