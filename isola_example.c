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
static unsigned char elementCube[] = {
	0,1,2,			0,2,3,
	4,5,6,			4,6,7,
	8,9,10,			8,10,11,
	12,13,14,		12,14,15,
	16,17,18,		16,18,19,
	20,21,22,		20,22,23,
};

int resolution[] = {480,360};
int framesize[] = {32,24};


float view[4][4] = {0};
float projection[4][4] = {0};

void printm4(float m[4][4]){
	int i;
	SDL_Log("\n");
	for(i = 0;i<4;i++){
			SDL_Log("%f,%f,%f,%f,\n",m[0][i],m[1][i],m[2][i],m[3][i]);
	}
	SDL_Log("\n");
}

void mprojection(unsigned char pers,float left, float right, float bottom, float top, float near, float far, float dest[4][4]){
	projection[0][0] = (float)resolution[1]/resolution[0];
	projection[1][1] = 1;
	projection[2][2] = -0.125;
	projection[3][3] = 1;
	projection[2][3] = -1*pers;
}

void mset(void){

	view[0][0] = 1;
	view[1][1] = 1;
	view[2][2] = 1;
	view[3][3] = 1;
	view[3][0] = 0;
	view[3][1] = 0;
	view[3][2] = -1;
	mprojection(0,(float)-32/24/* *0.816496581 */, (float)32/24/* *0.816496581 */,
			-/* 0.81649658 */1, /* 0.81649658 */1, 0.125f, 4.f, projection);
}

void mrot(void){
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

	glGenRenderbuffers(2,&renderBuffer[0]);
	glBindRenderbuffer(GL_RENDERBUFFER,renderBuffer[0]);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_RGB,framesize[0],framesize[1]);
	glBindRenderbuffer(GL_RENDERBUFFER,renderBuffer[1]);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,framesize[0],framesize[1]);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,
							  GL_RENDERBUFFER,renderBuffer[0]);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,
							  GL_RENDERBUFFER,renderBuffer[1]);

	drawBuffers[0] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1,drawBuffers);
	glViewport(0,0,framesize[0],framesize[1]);
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
	glBlitFramebuffer(0,0,framesize[0],framesize[1],0,0,resolution[0],resolution[1],
					  GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT,GL_NEAREST);

	SDL_GL_SwapWindow(isolaWindow);
}



#define TICK_INTERVAL	30

/* static Uint32 next_time;
Uint32 time_left(void)
{
	Uint32 now;

	now = SDL_GetTicks();
	if(next_time <= now)
		return 0;
	else
		return next_time - now;
}

	next_time = SDL_GetTicks() + TICK_INTERVAL;
	while ( game_running ) {
		update_game_state();
		SDL_Delay(time_left());
		next_time += TICK_INTERVAL;
	} */


void loop(void){
	int isolaFPS = 144;
	int isolaSPS = 144;
	unsigned long lastFrame = SDL_GetTicks64();
	unsigned long lastStep = SDL_GetTicks64();

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
					break;
				}
			}
		}

		if(!pause){
			if(SDL_GetTicks64()>=lastStep+1000/isolaSPS){

				mrot();

				/* if (state[SDL_SCANCODE_A]){} */
				lastStep = SDL_GetTicks64();
			}
		}else {SDL_Delay(1);}

		if(SDL_GetTicks64()>lastFrame+1000/isolaFPS){


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




