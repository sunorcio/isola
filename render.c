#include "render.h"




static float vertexCube[] = {
	-0.500,-0.500, 0.500,		0.250, 0.250, 0.750,
	 0.500,-0.500, 0.500,		0.250, 0.250, 0.750,
	 0.500, 0.500, 0.500,		0.250, 0.250, 0.750,
	-0.500, 0.500, 0.500,		0.250, 0.250, 0.750,

	-0.500, 0.500, 0.500,		0.250, 0.750, 0.250,
	 0.500, 0.500, 0.500,		0.250, 0.750, 0.250,
	 0.500, 0.500,-0.500,		0.250, 0.750, 0.250,
	-0.500, 0.500,-0.500,		0.250, 0.750, 0.250,

	 0.500,-0.500, 0.500,		0.750, 0.250, 0.250,
	 0.500,-0.500,-0.500,		0.750, 0.250, 0.250,
	 0.500, 0.500,-0.500,		0.750, 0.250, 0.250,
	 0.500, 0.500, 0.500,		0.750, 0.250, 0.250,

	-0.500,-0.500,-0.500,		0.625, 0.625, 0.000,
	-0.500, 0.500,-0.500,		0.625, 0.625, 0.000,
	 0.500, 0.500,-0.500,		0.625, 0.625, 0.000,
	 0.500,-0.500,-0.500,		0.625, 0.625, 0.000,

	-0.500,-0.500, 0.500,		0.625, 0.000, 0.625,
	-0.500,-0.500,-0.500,		0.625, 0.000, 0.625,
	 0.500,-0.500,-0.500,		0.625, 0.000, 0.625,
	 0.500,-0.500, 0.500,		0.625, 0.000, 0.625,

	-0.500,-0.500, 0.500,		0.000, 0.625, 0.625,
	-0.500, 0.500, 0.500,		0.000, 0.625, 0.625,
	-0.500, 0.500,-0.500,		0.000, 0.625, 0.625,
	-0.500,-0.500,-0.500,		0.000, 0.625, 0.625,
};
static unsigned short elementsCube[] = {
	0,1,2,			0,2,3,
	4,5,6,			4,6,7,
	8,9,10,			8,10,11,
	12,13,14,		12,14,15,
	16,17,18,		16,18,19,
	20,21,22,		20,22,23,
};




float view[4*4] = {0};
float projection[4*4] = {0};

static void orthogl(float left, float right, float bottom, float top,
				float nearZ, float farZ, float dest[4*4]){
	float rl,tb,fn;
	rl = 1./(right-left);
	tb = 1./(top-bottom);
	fn =-1./(farZ-nearZ);

	dest[0*4+0] = 2.*rl;
	dest[1*4+1] = 2.*tb;
	dest[2*4+2] = 2.*fn;
	dest[3*4+0] = -(right+left)*rl;
	dest[3*4+1] = -(top+bottom)*tb;
	dest[3*4+2] = (farZ+nearZ)*fn;
	dest[3*4+3] = 1.;
}
static void matrixsetup(void){
/* 	defaultpos[1] = +0.5/((float)framesize[0]/2); */

	view[0*4+0] = 1;
	view[1*4+1] = 1;
	view[2*4+2] = 1;

	view[3*4+0] = 0;
	view[3*4+1] = 0;
	view[3*4+2] = -2;
	view[3*4+3] = 1;

	orthogl(-1.,1.,-1.,1., 0.125f, 4.f, projection);
}




static unsigned int drawBuffers[1];
static unsigned int renderBuffer[2];
static unsigned int frameBuffer[1];
void frameCreate(void){

	glGenFramebuffers(1,&frameBuffer[0]);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,frameBuffer[0]);

	glGenRenderbuffers(2,&renderBuffer[0]);
	glBindRenderbuffer(GL_RENDERBUFFER,renderBuffer[0]);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_RGB,
							isolaInfoWindow.width,isolaInfoWindow.height);
	glBindRenderbuffer(GL_RENDERBUFFER,renderBuffer[1]);

	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,
							isolaInfoWindow.width,isolaInfoWindow.height);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,
								  GL_RENDERBUFFER,renderBuffer[0]);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,
							  GL_RENDERBUFFER,renderBuffer[1]);


	drawBuffers[0] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1,drawBuffers);
	glViewport(0,0,isolaInfoWindow.width,isolaInfoWindow.height);
}
void frameDestroy(void){

	glBindRenderbuffer(GL_RENDERBUFFER,0);
	glDeleteRenderbuffers(2,&renderBuffer[0]);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
	glDeleteFramebuffers(1,&frameBuffer[0]);
}
static unsigned int elementBuffer[1];
static unsigned int vertexBuffer[1];
static unsigned int vertexArrayObject[1];
static unsigned int shaderProgram[1];
void renderCreate(void){
	int locPos = 0;
	int locCol = 1;
	int id;
	unsigned int vs, fs;
	int locViw, locPrj, locDis;



	SDL_GL_SetSwapInterval(0);


	glGenVertexArrays(1,&vertexArrayObject[0]);
	glGenBuffers(1,&vertexBuffer[0]);
	glGenBuffers(1,&elementBuffer[0]);


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

	matrixsetup();

	locViw = glGetUniformLocation(shaderProgram[id],"matView");
	if(locViw == -1){SDL_Log("matView not found in shader %d",id);}
	glUniformMatrix4fv(locViw,1,GL_FALSE,view);

	locPrj = glGetUniformLocation(shaderProgram[id],"matProj");
	if(locPrj == -1){SDL_Log("matProj not found in shader %d",id);}
	glUniformMatrix4fv(locPrj,1,GL_FALSE,projection);



	frameCreate();

}
void renderDestroy(void){

	frameDestroy();

	glUseProgram(0);
	glBindVertexArray(0);
	glDeleteProgram(shaderProgram[0]);
	glDeleteBuffers(1,vertexBuffer);
	glDeleteBuffers(1,elementBuffer);
	glDeleteVertexArrays(1,vertexArrayObject);
}
void renderDraw(void){
	int id;
	int locViw;

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,frameBuffer[0]);
	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );


	id = 0;
	glBindVertexArray(vertexArrayObject[id]);
	glUseProgram(shaderProgram[id]);
	locViw = glGetUniformLocation(shaderProgram[id],"matView");
	glUniformMatrix4fv(locViw,1,GL_FALSE,view);

	glDrawElements(GL_TRIANGLES,sizeof(elementsCube),
					GL_UNSIGNED_SHORT,(void*)0);



	glBindFramebuffer(GL_READ_FRAMEBUFFER,frameBuffer[0]);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
	glBlitFramebuffer(0,0,isolaInfoWindow.width,isolaInfoWindow.height,0,0,
						isolaInfoWindow.width,isolaInfoWindow.height,
						GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT,GL_NEAREST);

	SDL_GL_SwapWindow(isolaWindow);
}



