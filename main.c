
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>


#include "isola.h"


#include "render.h"




void loop(void){
	#define isolaFPS 60
	#define isolaSPS 60
	const unsigned long clockFreq = SDL_GetPerformanceFrequency();
	unsigned long lastFrame = SDL_GetPerformanceCounter();
	unsigned long lastStep = SDL_GetPerformanceCounter();

	int keyLength;
	const unsigned char* keyState = SDL_GetKeyboardState(&keyLength);

	SDL_Event event = {0};
	unsigned char run = 1;
	unsigned char pause = 0;
	while(run){
		while (SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){run = 0;}
			if(event.type == SDL_WINDOWEVENT){
				switch(event.window.event){
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						isolaGetWindow();
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
					case SDLK_a:
						cameradesrot[1] += M_PI/2;
					break;
					case SDLK_d:
						cameradesrot[1] -= M_PI/2;
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
			if(SDL_GetPerformanceCounter()>=lastStep+clockFreq/isolaSPS){
				lastStep = SDL_GetPerformanceCounter();

				if (keyState[SDL_SCANCODE_X]){
					cameradesrot[0] -= 2*asin(0.5);
				}
				if (keyState[SDL_SCANCODE_S]){
					cameradesrot[0] -= asin(0.5);
				}
				if (keyState[SDL_SCANCODE_W]){
					cameradesrot[0] += M_PI/2-asin(0.5)+0.0;
				}
/* 				if (keyState[SDL_SCANCODE_A]){
					cameradesrot[1] += M_PI/2;
				}
				if (keyState[SDL_SCANCODE_D]){
					cameradesrot[1] -= M_PI/2;
				} */

				cameraUpdate();
			}
		}

		if(SDL_GetPerformanceCounter()>=lastFrame+clockFreq/isolaFPS){
			lastFrame = SDL_GetPerformanceCounter();

			renderDraw();

		}else{SDL_Delay(1);}

	}
}




int main(void){

	srand(time(0));
	isolaInit();
	SDL_GL_SetSwapInterval(0);


	renderCreate();
	loop();
	renderDestroy();


	isolaErrorGL();
	isolaErrorSDL(-1);
	isolaQuit();
	return 0;
}




