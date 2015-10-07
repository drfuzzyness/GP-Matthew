#include "CoreApp.hpp"

SDL_Window* displayWindow;

int main(int argc, char *argv[])
{
    CoreApp coreApp = CoreApp();
	while (!coreApp.done) {
        
        coreApp.UpdateAndRender();
	}

	SDL_Quit();
	return 0;
}
