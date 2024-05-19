

#include "render.h"


#include "timer.h"
#include "mutil.h"
#include "mesh.h"




#define digitPrintNum 5
unsigned char b_DigitFPS = 1;
float digitColor[3] = {0.,0.,0.};
static char stringDigitFPS[16] = {0};
static unsigned int digitSP[1] = {0};
static unsigned int digitVAO[1] = {0};
static unsigned int digitEBO[1] = {0};
static unsigned short digitED[16]
	[sizeof(elementDigit[0])/sizeof(elementDigit[0][0])] = {0};

void updateDigitFPS(void){
	glUseProgram(digitSP[0]);

	{int locDcol;
	locDcol = glGetUniformLocation(digitSP[0],"digitColor");
	if(locDcol == -1){SDL_Log("digitColor not found in shader %d",0);}
	glUniform3fv(locDcol,1,digitColor);
	}
}
static void createDigitFPS(void){

	glGenVertexArrays(1,&digitVAO[0]);
	glGenBuffers(1,&digitEBO[0]);


	glBindVertexArray(digitVAO[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,digitEBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(digitED),
					digitED,GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,
					sizeof(digitED),digitED);


	{unsigned int vs, fs;
	digitSP[0] = glCreateProgram();
	vs = isolaShaderCompile("glsl/digit.vert",GL_VERTEX_SHADER);
	fs = isolaShaderCompile("glsl/digit.frag",GL_FRAGMENT_SHADER);
	glAttachShader(digitSP[0],vs);
	glAttachShader(digitSP[0],fs);

	glLinkProgram(digitSP[0]);
	glDetachShader(digitSP[0],vs);
	glDetachShader(digitSP[0],fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	}


	glUseProgram(digitSP[0]);

	{int locProj;
	float matProj[4*4] = {0};
	mut_proj_glortho(-isolaInfoWindow.xratio,isolaInfoWindow.xratio,
						-isolaInfoWindow.yratio,isolaInfoWindow.yratio,
						0.25,2.,matProj);
	locProj = glGetUniformLocation(digitSP[0],"matProj");
	if(locProj == -1){SDL_Log("matProj not found in shader %d",0);}
	glUniformMatrix4fv(locProj,1,GL_FALSE,matProj);
	}

	{int locDcol;
	locDcol = glGetUniformLocation(digitSP[0],"digitColor");
	if(locDcol == -1){SDL_Log("digitColor not found in shader %d",0);}
	glUniform3fv(locDcol,1,digitColor);
	}
}
static void destroyDigitFPS(void){

	glUseProgram(0);
	glBindVertexArray(0);
	glDeleteProgram(digitSP[0]);
	glDeleteBuffers(1,digitEBO);
	glDeleteVertexArrays(1,digitVAO);
}
static void drawDigitFPS(void){

	unsigned delaySum = 0;

	{unsigned int i;
	for(i = 0;i<256;i++){
		delaySum += frameDelay[i];
	}}

	{unsigned int i;
	for(i = 0;i<digitPrintNum;i++){
		stringDigitFPS[i] = 0;
	}}
	sprintf(stringDigitFPS,"%f",1./((delaySum/256.)/(double)clockFreq) );

	{unsigned int i;
	unsigned char digit;
	for(i = 0;i<digitPrintNum;i++){
		switch (stringDigitFPS[i]) {
			case 0:   digit = 0;  break;
			case ' ': digit = 0;  break;
			case '0': digit = 1;  break;
			case '1': digit = 2;  break;
			case '2': digit = 3;  break;
			case '3': digit = 4;  break;
			case '4': digit = 5;  break;
			case '5': digit = 6;  break;
			case '6': digit = 7;  break;
			case '7': digit = 8;  break;
			case '8': digit = 9;  break;
			case '9': digit = 10; break;
			case '.': digit = 11; break;
			default: digit = 11; break;
		}

		{unsigned char j;
		for(j = 0;j<sizeof(elementDigit[0])/sizeof(elementDigit[0][0]);j++){
			digitED[i][j] = elementDigit[digit][j]+15*i;
		}}
	}}

	glBindVertexArray(digitVAO[0]);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,
		( sizeof(digitED)/16 )*digitPrintNum,digitED);
	glUseProgram(digitSP[0]);

	glDrawRangeElements(GL_TRIANGLES,0,
		(sizeof(elementDigit[0])/sizeof(elementDigit[0][0]))*digitPrintNum-1,
		(sizeof(elementDigit[0])/sizeof(elementDigit[0][0]))*digitPrintNum,
		GL_UNSIGNED_SHORT,(void*)0);
}




static float fontColor[3] = {0};
static float backColor[3] = {0};
static unsigned int bitfontSP[1] = {0};
static unsigned int bitfontVAO[1] = {0};
static unsigned int bitfontVBO[1] = {0};
static float bitfontVD[8] = {0};

static void createBitmapFont(void){

	glGenVertexArrays(1,&bitfontVAO[0]);
	glGenBuffers(1,&bitfontVBO[0]);


	glBindVertexArray(bitfontVAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER,bitfontVBO[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(bitfontVD),
					bitfontVD,GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0,
					sizeof(bitfontVD),bitfontVD);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,
						  sizeof(bitfontVD[0])*2,(void*)0);


	/* load and configure texture unit */
	/* check attr/sampler bindings */
	/* test shader */


	{unsigned int vs, fs;
	bitfontSP[0] = glCreateProgram();
	vs = isolaShaderCompile("glsl/bitfont.vert",GL_VERTEX_SHADER);
	fs = isolaShaderCompile("glsl/bitfont.frag",GL_FRAGMENT_SHADER);
	glAttachShader(bitfontSP[0],vs);
	glAttachShader(bitfontSP[0],fs);

	glBindAttribLocation(bitfontSP[0],0,"fragTexCoord");

	glLinkProgram(bitfontSP[0]);
	glDetachShader(bitfontSP[0],vs);
	glDetachShader(bitfontSP[0],fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	}


	glUseProgram(bitfontSP[0]);

	{int locProj;
	float matProj[4*4] = {0};
	mut_proj_glortho(-isolaInfoWindow.xratio,isolaInfoWindow.xratio,
						-isolaInfoWindow.yratio,isolaInfoWindow.yratio,
						0.25,2.,matProj);
	locProj = glGetUniformLocation(bitfontSP[0],"matProj");
	if(locProj == -1){SDL_Log("matProj not found in shader %d",0);}
	glUniformMatrix4fv(locProj,1,GL_FALSE,matProj);
	}

	{int locFcol;
	locFcol = glGetUniformLocation(bitfontSP[0],"fontColor");
	if(locFcol == -1){SDL_Log("fontColor not found in shader %d",0);}
	glUniform3fv(locFcol,1,fontColor);
	}

	{int locBcol;
	locBcol = glGetUniformLocation(bitfontSP[0],"backColor");
	if(locBcol == -1){SDL_Log("backColor not found in shader %d",0);}
	glUniform3fv(locBcol,1,fontColor);
	}

	{int locBatl;
	locBatl = glGetUniformLocation(bitfontSP[0],"backColor");
	if(locBatl == -1){SDL_Log("backColor not found in shader %d",0);}
	glUniform3fv(locBatl,1,fontColor);
	}
}
static void destroyBitmapFont(void){
}
static void drawBitmapFont(void){
}




void renderGlobalCreate(void){

	SDL_SetWindowSize(isolaWindow,isolaInfoWindow.desktopDisplayMode.w,
						isolaInfoWindow.desktopDisplayMode.h);
	glViewport(0,0,isolaInfoWindow.desktopDisplayMode.w,
				isolaInfoWindow.desktopDisplayMode.h);
	isolaGetWindow();


	createDigitFPS();
}
void renderGlobalDestroy(void){

	destroyDigitFPS();
}
void renderGlobalDraw(void){

	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );


	if (b_DigitFPS) {drawDigitFPS();}


	SDL_GL_SwapWindow(isolaWindow);
}


