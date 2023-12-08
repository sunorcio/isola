#include "isola.h"
#include <cglm/cglm.h>
#include <time.h>




void printm4(float m[4][4]){
	int i;
	printf("\n");
	for(i = 0;i<4;i++){
			printf("%f,%f,%f,%f,\n",m[0][i],m[1][i],m[2][i],m[3][i]);
	}
	printf("\n");
}

void msetup(void){
	float cameraPos[] = {0,0,-1};
	float cameraTgt[] = {0,0,0};
	float cameraUp[] = {0,1,0};
	mat4 view = {0};
	mat4 projection = {0};

 	glm_lookat(cameraPos, cameraTgt, cameraUp, view);
	glm_ortho((float)-32/24/* *0.816496581 */, (float)32/24/* *0.816496581 */,
			-/* 0.81649658 */1, /* 0.81649658 */1, 0.125f, 4.f, projection);

	printm4(view);
	printm4(projection);
	glm_frustum((float)-32/24/* *0.816496581 */, (float)32/24/* *0.816496581 */,
			-/* 0.81649658 */1, /* 0.81649658 */1, 0.125f, 4.f, projection);
	//glm_ortho((float)-32/24/* *0.816496581 */, (float)32/24/* *0.816496581 */,
	//		-/* 0.81649658 */1, /* 0.81649658 */1, 0.125f, 8.f, projection);
	printm4(projection);
}











/* no input without window */
/* TODO print average frametime, be generous with the sample size to compensate probable imprecision */


void loop(void){
	int isolaFPS = 144;
	int isolaSPS = 144;
	unsigned long lastFrame = SDL_GetTicks64();
	unsigned long lastStep = SDL_GetTicks64();

	int numkeys;
	const unsigned char* state = SDL_GetKeyboardState(&numkeys);

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
				}
			}
		}

		if(SDL_GetTicks64()>=lastStep+1000/isolaSPS){
			lastStep = SDL_GetTicks64();
		}

	}
}


int main(void){
	srand(time(0));
	isolaInit();


	loop();


	isolaErrorGL();
	isolaErrorSDL(-1);
	isolaQuit();
	return 0;
}




