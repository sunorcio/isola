#include "SDL2/SDL.h"
#include <cglm/cglm.h>



void printm4(float m[4*4]){
	int i;

	SDL_Log("\n");
	for(i = 0;i<4;i++){
		SDL_Log("%f,%f,%f,%f,\n",m[0*4+i],m[1*4+i],m[2*4+i],m[3*4+i]);
	}
	SDL_Log("\n");
}




void msetup(void){
	float cameraPos[3] = {0,0,1};
	float cameraTgt[3] = {0,0,0};
	float cameraUp[3] = {0,1,0};
	mat4 view = {0};
	mat4 projection = {0};
	
	glm_lookat(cameraPos, cameraTgt, cameraUp, view);
	glm_ortho((float)-8/6/* *0.816496581 */, (float)8/6/* *0.816496581 */,
		-/* 0.81649658 */1, /* 0.81649658 */1, 0.125f, 4.f, projection);
/* 	glm_frustum((float)-8/6// *0.816496581 //, (float)8/6// *0.816496581 //,
			-// 0.81649658 //1, // 0.81649658 //1, 0.125f, 4.f, projection); */
	printm4(*view);
	printm4(*projection);
}




int main(void){

	msetup();
}




