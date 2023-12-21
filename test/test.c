#include "isola.h"
#include <cglm/cglm.h>
#include <time.h>




void printm4(float m[4][4]){
	int i;
	SDL_Log("\n");
	for(i = 0;i<4;i++){
			SDL_Log("%f,%f,%f,%f,\n",m[0][i],m[1][i],m[2][i],m[3][i]);
	}
	SDL_Log("\n");
}

/* TODO test matrix transformation bounds and definition */
void msetup(void){
	float cameraPos[] = {0,0,-1};
	float cameraTgt[] = {0,0,0};
	float cameraUp[] = {0,1,0};
	mat4 view = {0};
	mat4 projection = {0};
	
	glm_lookat(cameraPos, cameraTgt, cameraUp, view);
	glm_ortho((float)-8/6/* *0.816496581 */, (float)8/6/* *0.816496581 */,
			-/* 0.81649658 */1, /* 0.81649658 */1, 0.125f, 4.f, projection);
/* 	glm_frustum((float)-8/6// *0.816496581 //, (float)8/6// *0.816496581 //,
			-// 0.81649658 //1, // 0.81649658 //1, 0.125f, 4.f, projection); */
	printm4(view);
	printm4(projection);
}




void loop(void){
	#define isolaFPS 144
	const unsigned long clockFreq = SDL_GetPerformanceFrequency();
	unsigned long lastSteps = SDL_GetPerformanceCounter();
	unsigned long lastStepf = SDL_GetPerformanceCounter();

	SDL_GL_SetSwapInterval(0);

	SDL_Event ev = {0};
	unsigned char run = 1;
	while(run){

		while (SDL_PollEvent(&ev)){
			if(ev.type == SDL_QUIT){run = 0;}
			if(ev.type == SDL_KEYDOWN){
				switch (ev.key.keysym.sym){
					case SDLK_ESCAPE:
						run = !run;
					break;
					case SDLK_SPACE:
					break;
				}
			}
		}

		if(SDL_GetPerformanceCounter()>=lastStepf+clockFreq/isolaFPS){
			lastStepf += clockFreq/isolaFPS;

			glClear(GL_COLOR_BUFFER_BIT);
			SDL_GL_SwapWindow(isolaWindow);
		}
	}
}


int main(void){
	srand(time(0));
	isolaInit();


	msetup();
	loop();


	isolaErrorGL();
	isolaErrorSDL(-1);
	isolaQuit();
	return 0;
}




