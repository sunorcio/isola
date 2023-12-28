#include "isola.h"
#include <time.h>


#include <cglm/cglm.h>
void lookat(float eye[3],float center[3],float up[3],float dest[4][4]){
	vec3 f, u, s;
	float norm;

/* sub */
	f[0] = center[0] - eye[0];
	f[1] = center[1] - eye[1];
	f[2] = center[2] - eye[2];
/* norm */
	norm = sqrtf(f[0] * f[0] + f[1] * f[1] + f[2] * f[2]);
	if (norm == 0.0f) {
		f[0] = f[1] = f[2] = 0.0f;
	}else {
		f[0] = f[0] * 1.0f / norm;
		f[1] = f[1] * 1.0f / norm;
		f[2] = f[2] * 1.0f / norm;
	}
/* crossnorm */
	s[0] = f[1] * up[2] - f[2] * up[1];
	s[1] = f[2] * up[0] - f[0] * up[2];
	s[2] = f[0] * up[1] - f[1] * up[0];
	norm = sqrtf(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);
	if (norm == 0.0f) {
		s[0] = s[1] = s[2] = 0.0f;
	}else{
		s[0] = s[0] * 1.0f / norm;
		s[1] = s[1] * 1.0f / norm;
		s[2] = s[2] * 1.0f / norm;
	}
/* cross */
	u[0] = s[1] * f[2] - s[2] * f[1];
	u[1] = s[2] * f[0] - s[0] * f[2];
	u[2] = s[0] * f[1] - s[1] * f[0];
/* dots */
	dest[0][0] = s[0];
	dest[0][1] = u[0];
	dest[0][2] =-f[0];
	dest[1][0] = s[1];
	dest[1][1] = u[1];
	dest[1][2] =-f[1];
	dest[2][0] = s[2];
	dest[2][1] = u[2];
	dest[2][2] =-f[2];
	dest[3][0] =-s[0] * eye[0] + s[1] * eye[1] + s[2] * eye[2];
	dest[3][1] =-u[0] * eye[0] + u[1] * eye[1] + u[2] * eye[2];
	dest[3][2] = f[0] * eye[0] + f[1] * eye[1] + f[2] * eye[2];;
	dest[0][3] = dest[1][3] = dest[2][3] = 0.0f;
	dest[3][3] = 1.0f;
}



void printm4(float m[4*4]){
	int i;
	SDL_Log("\n");
	for(i = 0;i<4;i++){
			SDL_Log("%f,%f,%f,%f,\n",m[0*4+i],m[1*4+i],m[2*4+i],m[3*4+i]);
	}
	SDL_Log("\n");
}

/* TODO test matrix transformation bounds and definition */
void msetup(void){
	float cameraPos[] = {0,0,1};
	float cameraTgt[] = {0,0,0};
	float cameraUp[] = {0,1,0};
	mat4 view = {0};
	mat4 projection = {0};
	
	lookat(cameraPos, cameraTgt, cameraUp, view);
	glm_ortho((float)-8/6/* *0.816496581 */, (float)8/6/* *0.816496581 */,
		-/* 0.81649658 */1, /* 0.81649658 */1, 0.125f, 4.f, projection);
/* 	glm_frustum((float)-8/6// *0.816496581 //, (float)8/6// *0.816496581 //,
			-// 0.81649658 //1, // 0.81649658 //1, 0.125f, 4.f, projection); */
	printm4(*view);
/* 	printm4(*projection); */
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




