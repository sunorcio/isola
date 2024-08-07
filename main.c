

#include <time.h>
#include <stdlib.h>
#include <math.h>


#include "isola/isola.h"
#include "render.h"




void loop(void){

	#define isolaFPS 60
	#define isolaSPS 60
	const unsigned long clockFreq = SDL_GetPerformanceFrequency();
	unsigned long lastFrame = SDL_GetPerformanceCounter();
	unsigned long lastStep = SDL_GetPerformanceCounter();

	int keyLength;
	const unsigned char* keyState = SDL_GetKeyboardState(&keyLength);

	static unsigned char apressed = 0;
	static unsigned char dpressed = 0;

	SDL_Event event = {0};
	unsigned char run = 1;
	unsigned char pause = 0;
	while(run){
		while (SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){run = 0;}
			if(event.type == SDL_WINDOWEVENT){
				switch(event.window.event){
					case SDL_WINDOWEVENT_SIZE_CHANGED:
					case SDL_WINDOWEVENT_DISPLAY_CHANGED:
						isolaGetWindow();
					break;
					case SDL_WINDOWEVENT_CLOSE:
						run = !run;
					break;
				}
			}
			if(event.type == SDL_KEYDOWN){
				switch (event.key.keysym.sym){
					case SDLK_ESCAPE:
					case SDLK_q:
						run = !run;
					break;
					case SDLK_SPACE:
						levelreset();
						recursetree(6);
					break;
					case SDLK_p:
						pause = !pause;
					break;
					case SDLK_e:
						integerscaling = !integerscaling;
					break;
					case SDLK_i:
						camerapos[2] -= 1;
					break;
					case SDLK_k:
						camerapos[2] += 1;
					break;
					case SDLK_j:
						camerapos[0] -= 1;
					break;
					case SDLK_l:
						camerapos[0] += 1;
					break;
					case SDLK_o:
						camerapos[1] += 1;
					break;
					case SDLK_n:
						camerapos[1] -= 1;
					break;
					case SDLK_u:
						cameraframe += 2;
					break;
					case SDLK_h:
						cameraframe += -2;
						if(cameraframe<4){
							cameraframe = 4;
						}
					break;
				}
			}
		}

		if(!pause){
			unsigned long currentStep = SDL_GetPerformanceCounter();
			if(currentStep>=lastStep+clockFreq/isolaSPS){
/* 				SDL_Log("sec %ld, diff %ld",
						clockFreq/isolaSPS,currentStep-lastStep); */
				lastStep = currentStep;

				if (keyState[SDL_SCANCODE_X]){
					cameradesrot[0] -= 2*(float)asin(0.5);
				}
				if (keyState[SDL_SCANCODE_S]){
					cameradesrot[0] -= (float)asin(0.5);
				}
				if (keyState[SDL_SCANCODE_W]){
					cameradesrot[0] += (float)(M_PI/2-asin(0.5)+0.0);
				}
				if (keyState[SDL_SCANCODE_A]){
					if (!apressed) {
						cameradesrot[1] += (float)(M_PI/2.);
						apressed=!apressed;
					}
				}else { apressed=0; }
				if (keyState[SDL_SCANCODE_D]){
					if (!dpressed) {
						cameradesrot[1] -= M_PI/2;
						dpressed=!dpressed;
					}
				}else { dpressed=0;}

				cameraUpdate();
			}
		}

		{ unsigned long currentFrame = SDL_GetPerformanceCounter();
		if(currentFrame>=lastFrame+clockFreq/isolaFPS){
			lastFrame = currentFrame;

			renderDraw();

		}else{SDL_Delay(0);} }

	}
}




int main(int argc, char **argv){

	srand(time(0));
	isolaInit();


	renderCreate();
	loop();
	renderDestroy();


	isolaQuit();
	return 0;
}


