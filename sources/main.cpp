#include "cuberenderer.hpp"
#include "game.hpp"

#include <math.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#ifdef EMSCRIPTEN
#	include <emscripten.h>
#endif

static bool quit = false;
static CubeRenderer* pRenderer;
static Game* pLevel;
static Uint32 lastTicks;

void run();

int main() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* pScreen = SDL_SetVideoMode(800, 600, 32, SDL_OPENGL);

	glEnable(GL_DEPTH_TEST);

	pRenderer = new CubeRenderer();
	pLevel = new Game();
	lastTicks = SDL_GetTicks();

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

	Uint32 now = SDL_GetTicks();
	float timeStep = (now - lastTicks) / 1000.0f;
	lastTicks = now;

	pLevel->update(timeStep);

	glClearColor(0.125f, 0.25f, 0.125f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pRenderer->render(2, -1+0.2f, 0.2f, 1, 0.4f, 0.4f);
//	pRenderer->render(2, -2, 1, 1, 1, 1);
//	static float phase = 0;
//	phase += timeStep;
//	pRenderer->render(-1 + sinf(phase) * 2, 2, 1.5f, 0.8f, 0.8f, 1);

	pLevel->render(*pRenderer);

	SDL_GL_SwapBuffers();
}
