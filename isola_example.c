#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "isola.h"




#define JUSTPI		3.14159265358979323846

static float vertexCube[] = {
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
static unsigned char elementsCube[] = {
	0,1,2,			0,2,3,
	4,5,6,			4,6,7,
	8,9,10,			8,10,11,
	12,13,14,		12,14,15,
	16,17,18,		16,18,19,
	20,21,22,		20,22,23,
};

static float vertexAxis[] = {
	 0.625+0.000, 0.000, 0.000,		0.000, 0.000, 0.000,
	 0.625+0.250, 0.000, 0.000,		1.000, 0.000, 0.000,
	 0.625+0.000, 0.250, 0.000,		0.000, 1.000, 0.000,
	 0.625+0.000, 0.100, 0.250,		0.000, 0.000, 1.000,
};
static unsigned char elementsAxis[] = {
	0,1,
	0,2,
	0,3,
};

unsigned int resolution[2] = {480,360};
unsigned int framesize[2] = {32,24};


float view[4*4] = {0};
float projection[4*4] = {0};

void printm4(float m[4*4]){
	int i;
	SDL_Log("\n");
	for(i = 0;i<4;i++){
			SDL_Log("%f,%f,%f,%f,\n",m[0*4+i],m[1*4+i],m[2*4+i],m[3*4+i]);
	}
	SDL_Log("\n");
}

void mset(void){

	view[0*4+0] = 1;
	view[1*4+1] = 1;
	view[2*4+2] = 1;

	view[3*4+0] = 0;
	view[3*4+1] = 0;
	view[3*4+2] = -1;
	view[3*4+3] = 1;

	projection[0*4+0] = 0.75;
	projection[1*4+1] = 1;
	projection[2*4+2] = -0.516129;

	projection[3*4+0] = 0;
	projection[3*4+1] = 0;
	projection[3*4+2] = -1.064516;
	projection[3*4+3] = 1;

	printm4(view);
}
void manim(void){
	static float cameraRad = 1;
	static float cameraPhs = 0.;

	cameraPhs += JUSTPI/32;
	if (cameraPhs>JUSTPI*2){
		cameraPhs -= JUSTPI*2;
	}
	
}


unsigned int drawBuffers[1];
unsigned int renderBuffer[2];
unsigned int frameBuffer[1];
unsigned int elementBuffer[2];
unsigned int vertexBuffer[2];
unsigned int vertexArrayObject[2];
unsigned int shaderProgram[1];
void renderCreate(void){
	int locPos = 0;
	int locCol = 1;
	int id;
	unsigned int vs;
	unsigned int fs;
	int locViw;
	int locPrj;


	glGenVertexArrays(2,&vertexArrayObject[0]);
	glGenBuffers(2,&vertexBuffer[0]);
	glGenBuffers(2,&elementBuffer[0]);


	id = 0;
	glBindVertexArray(vertexArrayObject[id]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementBuffer[id]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elementsCube),
				 elementsCube,GL_DYNAMIC_DRAW);

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

	id = 1;
	glBindVertexArray(vertexArrayObject[id]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementBuffer[id]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elementsAxis),
				 elementsAxis,GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer[id]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexAxis),
				 vertexAxis,GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(locPos);
	glVertexAttribPointer(locPos,3,GL_FLOAT,GL_FALSE,
						  sizeof(vertexAxis[0])*6,(void*)0);
	
	glEnableVertexAttribArray(locCol);
	glVertexAttribPointer(locCol,3,GL_FLOAT,GL_FALSE,
						  sizeof(vertexAxis[0])*6,
						  (void*)(sizeof(vertexAxis[0])*3));



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
	glUniformMatrix4fv(locViw,1,GL_FALSE,view);

	locPrj = glGetUniformLocation(shaderProgram[id],"matProj");
	if(locPrj == -1){SDL_Log("matProj not found in shader %d",id);}
	glUniformMatrix4fv(locPrj,1,GL_FALSE,projection);



	glGenFramebuffers(1,&frameBuffer[0]);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,frameBuffer[0]);

	glGenRenderbuffers(2,&renderBuffer[0]);
	glBindRenderbuffer(GL_RENDERBUFFER,renderBuffer[0]);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_RGB,framesize[0],framesize[1]);
	glBindRenderbuffer(GL_RENDERBUFFER,renderBuffer[1]);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,
							framesize[0],framesize[1]);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,
								  GL_RENDERBUFFER,renderBuffer[0]);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,
							  GL_RENDERBUFFER,renderBuffer[1]);

	drawBuffers[0] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1,drawBuffers);
/* 	glViewport(0,0,framesize[0],framesize[1]); */
	glViewport(0,0,resolution[0],resolution[1]);

	glLineWidth(5);

	SDL_GL_SetSwapInterval(0);
}
void renderDestroy(void){
	glBindRenderbuffer(GL_RENDERBUFFER,0);
	glDeleteRenderbuffers(2,&renderBuffer[0]);
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

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0/* frameBuffer[0] */);

	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );

	id = 0;
	glBindVertexArray(vertexArrayObject[id]);
	glUseProgram(shaderProgram[id]);

	locViw = glGetUniformLocation(shaderProgram[id],"matView");
	glUniformMatrix4fv(locViw,1,GL_FALSE,view);

	glDrawElements(GL_TRIANGLES,sizeof(elementsCube),GL_UNSIGNED_BYTE,(void*)0);

	id = 1;
	glBindVertexArray(vertexArrayObject[id]);
	glDrawElements(GL_LINES,sizeof(elementsAxis),GL_UNSIGNED_BYTE,(void*)0);

/* 	glBindFramebuffer(GL_READ_FRAMEBUFFER,frameBuffer[0]);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
	glBlitFramebuffer(0,0,framesize[0],framesize[1],0,0,
						resolution[0],resolution[1],
					  GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT,GL_NEAREST); */

	SDL_GL_SwapWindow(isolaWindow);
}




void loop(void){
	#define isolaFPS 144
	#define isolaSPS 144
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
			if(event.type == SDL_KEYDOWN){
				switch (event.key.keysym.sym){

					case SDLK_ESCAPE:
						run = !run;
					break;
					case SDLK_q:
						run = !run;
					break;
					case SDLK_p:
						pause = !pause;
					break;
					case SDLK_SPACE:
						printm4(view);
						printm4(projection);
					break;
				}
			}
		}

		if(!pause){
			if(SDL_GetPerformanceCounter()>=lastStep+clockFreq/isolaSPS){
				lastStep += clockFreq/isolaSPS;
				if (keyState[SDL_SCANCODE_A]){
				}
				if (keyState[SDL_SCANCODE_D]){
				}
				if (keyState[SDL_SCANCODE_S]){
				}
				if (keyState[SDL_SCANCODE_W]){
				}

				manim();
			}
		}

		if(SDL_GetPerformanceCounter()>=lastFrame+clockFreq/isolaFPS){
			lastFrame += clockFreq/isolaFPS;

			draw();

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




