#include "cuberenderer.hpp"
#include "game.hpp"
#include "input.hpp"

#include <math.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#ifdef EMSCRIPTEN
#	include <emscripten.h>
#endif

static bool quit = false;
static CubeRenderer* pRenderer;
static Game* pGame;
static Uint32 lastTicks;
static Input input;

void run();

int main() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* pScreen = SDL_SetVideoMode(800, 600, 32, SDL_OPENGL);

	glEnable(GL_DEPTH_TEST);

	pRenderer = new CubeRenderer();
	pGame = new Game();
	input.x = 0;
	input.y = 0;
	input.pressed = 0;
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
		case SDL_MOUSEMOTION:
			input.x = (event.motion.x - 400) / 60.0f;
			input.y = -(event.motion.y - 300) / 60.0f;
			break;
		case SDL_MOUSEBUTTONDOWN:
			input.pressed = true;
			break;
		case SDL_MOUSEBUTTONUP:
			input.pressed = false;
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

	pGame->update(timeStep, input);

	pGame->render(*pRenderer);

	SDL_GL_SwapBuffers();
}
