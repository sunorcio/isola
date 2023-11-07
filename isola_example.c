#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "isola.h"



/* TODO
fix depth, fix rotation
use and test all functions in their simplest form
print all information, compartmentalyze init 
break apart render setup
keep or replace timers, test for stable framerate
*/



#define JUSTPI		3.14159265358979323846

static float vertexCube[] = {
/* 	pos                         col */
	-0.500,-0.500, 0.500,		0.750, 0.250, 0.250,
	 0.500,-0.500, 0.500,		0.750, 0.250, 0.250,
	 0.500, 0.500, 0.500,		0.750, 0.250, 0.250,
	-0.500, 0.500, 0.500,		0.750, 0.250, 0.250,

	-0.500, 0.500, 0.500,		0.250, 0.750, 0.250,
	 0.500, 0.500, 0.500,		0.250, 0.750, 0.250,
	 0.500, 0.500,-0.500,		0.250, 0.750, 0.250,
	-0.500, 0.500,-0.500,		0.250, 0.750, 0.250,

	-0.500,-0.500, 0.500,		0.250, 0.250, 0.750,
	-0.500, 0.500, 0.500,		0.250, 0.250, 0.750,
	-0.500, 0.500,-0.500,		0.250, 0.250, 0.750,
	-0.500,-0.500,-0.500,		0.250, 0.250, 0.750,

	-0.500,-0.500,-0.500,		0.000, 0.500, 0.500,
	-0.500, 0.500,-0.500,		0.000, 0.500, 0.500,
	 0.500, 0.500,-0.500,		0.000, 0.500, 0.500,
	 0.500,-0.500,-0.500,		0.000, 0.500, 0.500,

	-0.500,-0.500, 0.500,		0.500, 0.000, 0.500,
	-0.500,-0.500,-0.500,		0.500, 0.000, 0.500,
	 0.500,-0.500,-0.500,		0.500, 0.000, 0.500,
	 0.500,-0.500, 0.500,		0.500, 0.000, 0.500,

	 0.500,-0.500, 0.500,		0.500, 0.500, 0.000,
	 0.500,-0.500,-0.500,		0.500, 0.500, 0.000,
	 0.500, 0.500,-0.500,		0.500, 0.500, 0.000,
	 0.500, 0.500, 0.500,		0.500, 0.500, 0.000,
};
static uint8_t elementCube[] = {
	0,1,2,			0,2,3,
	4,5,6,			4,6,7,
	8,9,10,			8,10,11,
	12,13,14,		12,14,15,
	16,17,18,		16,18,19,
	20,21,22,		20,22,23,
};

int resolution[2] = {480,360};
int framesize = 32;

float view[4][4];
float projection[4][4];

void printm4(float m[4][4]){
	int i;
	SDL_Log("\n");
	for(i = 0;i<4;i++){
			SDL_Log("%f,%f,%f,%f,",m[0][i],m[1][i],m[2][i],m[3][i]);
	}
	SDL_Log("\n");
}

void mset(void){
	view[0][0] = 1;
	view[1][1] = 1;
	view[2][2] = 1;
	view[3][3] = 1;
	view[3][2] = -1;
	projection[0][0] = 1;
	projection[1][1] = (float)resolution[0]/resolution[1];
	projection[2][2] = 0.25;
	projection[3][3] = 1;
}

void mrot(void){
	static float cameraRad = 1;
	static float cameraPhs = 0.;

	cameraPhs += JUSTPI/32;
	if (cameraPhs>JUSTPI*2){
		cameraPhs -= JUSTPI*2;
	}

 	view[0][0] = cos(cameraPhs);
	view[2][0] = -sin(cameraPhs);
 	view[1][1] = cos(cameraPhs); 
 	view[2][1] = sin(cameraPhs); 
	view[0][2] = sin(cameraPhs);
 	view[1][2] = -sin(cameraPhs);
	view[2][2] = cos(cameraPhs);
}

unsigned int drawBuffers[1];
unsigned int renderBuffer[1];
unsigned int frameBuffer[1];
unsigned int elementBuffer[1];
unsigned int vertexBuffer[1];
unsigned int vertexArrayObject[1];
unsigned int shaderProgram[1];
void renderCreate(void){
	int locPos = 0;
	int locCol = 1;
	int id;
	unsigned int vs;
	unsigned int fs;
	int locViw;
	int locPrj;


	glGenVertexArrays(1,&vertexArrayObject[0]);
	glGenBuffers(1,&vertexBuffer[0]);
	glGenBuffers(1,&elementBuffer[0]);


	id = 0;
	glBindVertexArray(vertexArrayObject[id]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementBuffer[id]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elementCube),
				 elementCube,GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer[id]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexCube),
				 vertexCube,GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(locPos);
	glVertexAttribPointer(locPos,3,GL_FLOAT,GL_FALSE,
						  sizeof(vertexCube[0])*6,(void*)0);
	
	glEnableVertexAttribArray(locCol);
	glVertexAttribPointer(locCol,3,GL_FLOAT,GL_FALSE,
						  sizeof(vertexCube[0])*6,
						  (void*)(sizeof(vertexCube[0])*3));


	id = 0;
	shaderProgram[id] = glCreateProgram();
	vs = isolaShaderCompile("glsl/default.vert",GL_VERTEX_SHADER);
	fs = isolaShaderCompile("glsl/default.frag",GL_FRAGMENT_SHADER);
	glAttachShader(shaderProgram[id],vs);
	glAttachShader(shaderProgram[id],fs);

	glBindAttribLocation(shaderProgram[id],locPos,"vertPosition");
	glBindAttribLocation(shaderProgram[id],locCol,"vertColor");

	glLinkProgram(shaderProgram[id]);
	glDetachShader(shaderProgram[id],vs);
	glDetachShader(shaderProgram[id],fs);
	glDeleteShader(vs);
	glDeleteShader(fs);



	id = 0;
	glUseProgram(shaderProgram[id]);

	mset();

	locViw = glGetUniformLocation(shaderProgram[id],"matView");
	if(locViw == -1){SDL_Log("matView not found in shader %d",id);}
	glUniformMatrix4fv(locViw,1,GL_FALSE,*view);

	locPrj = glGetUniformLocation(shaderProgram[id],"matProj");
	if(locPrj == -1){SDL_Log("matProj not found in shader %d",id);}
	glUniformMatrix4fv(locPrj,1,GL_FALSE,*projection);


	glGenFramebuffers(1,&frameBuffer[0]);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,frameBuffer[0]);

	glGenRenderbuffers(1,&renderBuffer[0]);
	glBindRenderbuffer(GL_RENDERBUFFER,renderBuffer[0]);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_RGB,framesize,framesize);
	glViewport(0,0,framesize,framesize);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,
							  GL_RENDERBUFFER,renderBuffer[0]);
	drawBuffers[0] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1,drawBuffers);
}
void renderDestroy(void){
	glBindRenderbuffer(GL_RENDERBUFFER,0);
	glDeleteRenderbuffers(1,&renderBuffer[0]);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
	glDeleteFramebuffers(1,&frameBuffer[0]);

	glUseProgram(0);
	glBindVertexArray(0);
	glDeleteProgram(shaderProgram[0]);
	glDeleteBuffers(1,vertexBuffer);
	glDeleteBuffers(1,elementBuffer);
	glDeleteVertexArrays(1,vertexArrayObject);
}
void draw(void){
	int id;
	int locViw;

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,frameBuffer[0]);

	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );

	id = 0;
	glBindVertexArray(vertexArrayObject[id]);
	glUseProgram(shaderProgram[id]);

	locViw = glGetUniformLocation(shaderProgram[id],"matView");
	glUniformMatrix4fv(locViw,1,GL_FALSE,*view);

	glDrawElements(GL_TRIANGLES,sizeof(elementCube),GL_UNSIGNED_BYTE,(void*)0);

	glBindFramebuffer(GL_READ_FRAMEBUFFER,frameBuffer[0]);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
	glBlitFramebuffer(0,0,framesize,framesize,0,0,resolution[0],resolution[1],
					  GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT,GL_NEAREST);

	SDL_GL_SwapWindow(isolaWindow);
}



void loop(void){
	int isolaFPS = 30;
	int isolaSPS = 30;
	SDL_TimerID lastFrame = SDL_GetTicks64();
	SDL_TimerID lastStep = SDL_GetTicks64();

	int numkeys;
	const uint8_t* state = SDL_GetKeyboardState(&numkeys);

	SDL_Event ev = {0};
	uint8_t run = 1;
	while(run){
		while (SDL_PollEvent(&ev)){
			if(ev.type == SDL_QUIT){run = 0;}
			if(ev.type == SDL_KEYDOWN){
				switch (ev.key.keysym.sym){

					case SDLK_ESCAPE:
						run = 0;
					break;
					case SDLK_q:
						run = 0;
					break;
					case SDLK_r:
					break;
					case SDLK_SPACE:
					break;
				}
			}
		}

		if(SDL_GetTicks64()>=(unsigned)lastStep+1000/isolaSPS){

			mrot();

			/* if (state[SDL_SCANCODE_A]){} */
			lastStep = SDL_GetTicks64();
		}

		if(SDL_GetTicks64()>(unsigned)lastFrame+1000/isolaFPS){


			draw();


			lastFrame = SDL_GetTicks64();
		}else{SDL_Delay(1);}
	}
}

int main(void){
	srand(time(0));
	isolaInit();


	renderCreate();
	loop();
	renderDestroy();


	isolaErrorGL();
	isolaErrorSDL(-1);
	isolaQuit();
	return 0;
}




