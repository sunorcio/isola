#include <SDL2/SDL.h>




void printm4(float m[4*4]){
	int i;
	SDL_Log("\n");
	for(i = 0;i<4;i++){
			SDL_Log("%f,%f,%f,%f,\n",m[0*4+i],m[1*4+i],m[2*4+i],m[3*4+i]);
	}
	SDL_Log("\n");
}
void printm3(float m[3*3]){
	int i;
	SDL_Log("\n");
	for(i = 0;i<3;i++){
			SDL_Log("%f,%f,%f,\n",m[0*3+i],m[1*3+i],m[2*3+i]);
	}
	SDL_Log("\n");
}


void mat_cp_3(float src[3*3],float dest[3*3]){
	dest[0*3+0] = src[0*3+0];
	dest[0*3+1] = src[0*3+1];
	dest[0*3+2] = src[0*3+2];
	dest[1*3+0] = src[1*3+0];
	dest[1*3+1] = src[1*3+1];
	dest[1*3+2] = src[1*3+2];
	dest[2*3+0] = src[2*3+0];
	dest[2*3+1] = src[2*3+1];
	dest[2*3+2] = src[2*3+2];
}
void mat_cp_3_4(float src[3*3],float dest[4*4]){
	dest[0*4+0] = src[0*3+0];
	dest[0*4+1] = src[0*3+1];
	dest[0*4+2] = src[0*3+2];
	dest[1*4+0] = src[1*3+0];
	dest[1*4+1] = src[1*3+1];
	dest[1*4+2] = src[1*3+2];
	dest[2*4+0] = src[2*3+0];
	dest[2*4+1] = src[2*3+1];
	dest[2*4+2] = src[2*3+2];
}

void mat_mul_3(float a[3*3],float b[3*3],float dest[3*3]){
	float c[3*3] = {0};
	c[0*3+0] = a[0*3+0]*b[0*3+0] + a[1*3+0]*b[0*3+1] + a[2*3+0]*b[0*3+2];
	c[0*3+1] = a[0*3+1]*b[0*3+0] + a[1*3+1]*b[0*3+1] + a[2*3+1]*b[0*3+2];
	c[0*3+2] = a[0*3+2]*b[0*3+0] + a[1*3+2]*b[0*3+1] + a[2*3+2]*b[0*3+2];
	c[1*3+0] = a[0*3+0]*b[1*3+0] + a[1*3+0]*b[1*3+1] + a[2*3+0]*b[1*3+2];
	c[1*3+1] = a[0*3+1]*b[1*3+0] + a[1*3+1]*b[1*3+1] + a[2*3+1]*b[1*3+2];
	c[1*3+2] = a[0*3+2]*b[1*3+0] + a[1*3+2]*b[1*3+1] + a[2*3+2]*b[1*3+2];
	c[2*3+0] = a[0*3+0]*b[2*3+0] + a[1*3+0]*b[2*3+1] + a[2*3+0]*b[2*3+2];
	c[2*3+1] = a[0*3+1]*b[2*3+0] + a[1*3+1]*b[2*3+1] + a[2*3+1]*b[2*3+2];
	c[2*3+2] = a[0*3+2]*b[2*3+0] + a[1*3+2]*b[2*3+1] + a[2*3+2]*b[2*3+2];
	mat_cp_3(c,dest);
}
