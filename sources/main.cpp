#include <SDL/SDL.h>
#include <GL/gl.h>

int main() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* pScreen = SDL_SetVideoMode(800, 600, 32, SDL_OPENGL);

	bool quit = false;
	while(!quit) {
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

		glClearColor(0.125f, 0.25f, 0.125f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapBuffers();
	}

	SDL_Quit();
}
