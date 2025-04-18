#include "isola.h"




#include <stdlib.h>
#include <stdio.h>
#include <string.h>




static char* isola_shaderSrc;
static FILE* isola_log;
static void isola_get_context(void);
static void isola_contextPromt(void);




#ifdef ISOLA_DBG
signed char isola_error_sdl(int sdlFunction){

	if(*SDL_GetError() == 0){return 0;}

	if(sdlFunction<0){ SDL_Log("%s",SDL_GetError());}
	SDL_ClearError();
	return -1;
}

signed char isola_error_gl(void){

	GLenum error = glGetError();
	if (error == GL_NO_ERROR){ return 0; }

	do{ SDL_Log("GLerror : %s", gluErrorString(error));}
	while( (error = glGetError()) != GL_NO_ERROR );
	return -1;
}

 #if(  defined(GLEW_KHR_debug) || defined(GLEW_ARB_debug_output)  )
static void debugCallback(unsigned int source, unsigned int type,
		unsigned int id, unsigned int severity,
		int length,const char* message, const void* userParam){

	SDL_Log("%s",message);
	return;
}
 #endif
#else
signed char isola_error_sdl(int sdlFunction){return 0;}
signed char isola_error_gl(void){return 0;}
#endif




SDL_GLContext* isola_context = {0};
SDL_Window* isola_window = {0};    


struct ISOLA_context isola_info_context = {0};
struct ISOLA_window isola_info_window = {0};
struct ISOLA_display isola_info_display = {0};
ISOLA_state isola_info_state = {0};


static char* isola_shaderSrc = {0};
static FILE* isola_log = {0};




void isola_get_window(void){

	SDL_GetWindowPosition(isola_window, &isola_info_window.xPos,
			&isola_info_window.yPos);
	SDL_GetWindowSize(isola_window, &isola_info_window.width,
			&isola_info_window.height);

	if (isola_info_window.width > isola_info_window.height) {
		isola_info_window.yRatio = 1.;
		isola_info_window.xRatio = (double)isola_info_window.width
				/isola_info_window.height;
	}else {
		isola_info_window.xRatio = 1.;
		isola_info_window.yRatio = (double)isola_info_window.height 
				/isola_info_window.width;
	}
	isola_info_window.pixelWidth = (double)2./isola_info_window.width;
	isola_info_window.pixelHeight = (double)2./isola_info_window.height;

	isola_info_window.flags = SDL_GetWindowFlags(isola_window);
	isola_info_window.displayIndex = SDL_GetWindowDisplayIndex(isola_window);
	SDL_GetWindowDisplayMode(isola_window, &isola_info_window.displayMode);
	SDL_GetDesktopDisplayMode(isola_info_window.displayIndex,
			&isola_info_window.desktopDisplayMode);

	glGetFloatv(GL_COLOR_CLEAR_VALUE,isola_info_window.clearColor);
}


void isola_get_display(void){

	int i,j;
	int buffer;

	buffer = SDL_GetNumVideoDisplays();
	isola_info_display.displayModeCount = calloc(buffer+1, sizeof(int));
	for(i = 0;i<buffer;i++){
		isola_info_display.displayModeCount[i] = SDL_GetNumDisplayModes(
				isola_info_window.displayIndex);
	}

	for(i = 0;isola_info_display.displayModeCount[i]==0;i++){
		buffer += isola_info_display.displayModeCount[i];
	}
	isola_info_display.displayModes = malloc(buffer*sizeof(SDL_DisplayMode));

	buffer = 0;
	for(i = 0;isola_info_display.displayModeCount[i]==0;i++){
		for(j = 0;j<isola_info_display.displayModeCount[i];j++){
			SDL_GetDisplayMode(i,j,&isola_info_display.displayModes[buffer+j]);
		}
		buffer += isola_info_display.displayModeCount[i];
	}
}


static void isola_get_context(void){

	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
			GL_FRONT_LEFT, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE,
			&isola_info_context.fbdef_redSize);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, 
			GL_FRONT_LEFT, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE,
			&isola_info_context.fbdef_greenSize);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
			GL_FRONT_LEFT, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE,
			&isola_info_context.fbdef_blueSize);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
			GL_FRONT_LEFT, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE,
			&isola_info_context.fbdef_alphaSize);
#if ISOLA_DEPTH
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
			GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE,
			&isola_info_context.fbdef_depthSize);
#endif
#if ISOLA_STENCIL
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
			GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE,
			&isolaInfoContext.fbdefStencilsize);
#endif
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
			GL_FRONT_LEFT, GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING,
			&isola_info_context.fbdef_colorEncoding);

	glGetIntegerv(GL_DOUBLEBUFFER, &isola_info_context.fbdef_doubleBuffer);

	glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &isola_info_context.max_vertices);
	glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &isola_info_context.max_indices);
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &isola_info_context.max_attrib);
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS,
			&isola_info_context.max_vertexUniforms);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
			&isola_info_context.max_fragmentUniforms);
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
			&isola_info_context.max_texCombinedUnits);
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &isola_info_context.max_texUnits);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &isola_info_context.max_texSize);
	glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &isola_info_context.max_3dTexSize);
	glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE,
			&isola_info_context.max_cubeTexSize);
	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &isola_info_context.max_drawBuffers);
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS,
			&isola_info_context.max_colorAttachments);
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE,
			&isola_info_context.max_renderbufferSize);

	isola_info_context.cpuCount = SDL_GetCPUCount();
	isola_info_context.systemRAM = SDL_GetSystemRAM();
	isola_info_context.cacheSize = SDL_GetCPUCacheLineSize();
}


void isola_get_state(void){

	int state = {0};

	isola_info_state = 0x0000;
	glGetIntegerv(GL_BLEND, &state);
	isola_info_state = (isola_info_state | state*ISOLA_STATE_BLEND);
	glGetIntegerv(GL_COLOR_LOGIC_OP, &state);
	isola_info_state = (isola_info_state | state*ISOLA_STATE_COLORLOGIC);
	glGetIntegerv(GL_CULL_FACE, &state);
	isola_info_state = (isola_info_state | state*ISOLA_STATE_CULLFACE);
	glGetIntegerv(GL_DEPTH_TEST, &state);
	isola_info_state = (isola_info_state | state*ISOLA_STATE_DEPTHTEST);
	glGetIntegerv(GL_DITHER, &state);
	isola_info_state = (isola_info_state | state*ISOLA_STATE_DITHER);
	glGetIntegerv(GL_SCISSOR_TEST, &state);
	isola_info_state = (isola_info_state | state*ISOLA_STATE_SCISSORTEST);
	glGetIntegerv(GL_STENCIL_TEST, &state);
	isola_info_state = (isola_info_state | state*ISOLA_STATE_STENCILTEST);
	glGetIntegerv(GL_FRAMEBUFFER_SRGB, &state);
	isola_info_state = (isola_info_state | state*ISOLA_STATE_SRGBFRAMEBUFFER);
	glGetIntegerv(GL_POINT_SMOOTH, &state);
	isola_info_state = (isola_info_state | state*ISOLA_STATE_POINTSMOOTH);
	glGetIntegerv(GL_LINE_SMOOTH, &state);
	isola_info_state = (isola_info_state | state*ISOLA_STATE_LINESMOOTH);
	glGetIntegerv(GL_POLYGON_SMOOTH, &state);
	isola_info_state = (isola_info_state | state*ISOLA_STATE_POLYGONSMOOTH);
	glGetIntegerv(GL_PROGRAM_POINT_SIZE, &state);
	isola_info_state = (isola_info_state | state*ISOLA_STATE_POINTSIZEPROGRAM);
	glGetIntegerv(GL_MULTISAMPLE, &state);
	isola_info_state = (isola_info_state | state*ISOLA_STATE_MULTISAMPLE);
}


void isola_set_state(ISOLA_state state){

	if (state&ISOLA_STATE_BLEND) {glEnable(GL_BLEND);}
	else {glDisable(GL_BLEND);}
	if (state&ISOLA_STATE_COLORLOGIC) {glEnable(GL_COLOR_LOGIC_OP);}
	else {glDisable(GL_COLOR_LOGIC_OP);}
	if (state&ISOLA_STATE_CULLFACE) {glEnable(GL_CULL_FACE);}
	else {glDisable(GL_CULL_FACE);}
	if (state&ISOLA_STATE_DEPTHTEST) {glEnable(GL_DEPTH_TEST);}
	else {glDisable(GL_DEPTH_TEST);}
	if (state&ISOLA_STATE_DITHER) {glEnable(GL_DITHER);}
	else {glDisable(GL_DITHER);}
	if (state&ISOLA_STATE_SCISSORTEST) {glEnable(GL_SCISSOR_TEST);}
	else {glDisable(GL_SCISSOR_TEST);}
	if (state&ISOLA_STATE_STENCILTEST) {glEnable(GL_STENCIL_TEST);}
	else {glDisable(GL_STENCIL_TEST);}
	if (state&ISOLA_STATE_SRGBFRAMEBUFFER) {glEnable(GL_FRAMEBUFFER_SRGB);}
	else {glDisable(GL_FRAMEBUFFER_SRGB);}
	if (state&ISOLA_STATE_POINTSMOOTH) {glEnable(GL_POINT_SMOOTH);}
	else {glDisable(GL_POINT_SMOOTH);}
	if (state&ISOLA_STATE_LINESMOOTH) {glEnable(GL_LINE_SMOOTH);}
	else {glDisable(GL_LINE_SMOOTH);}
	if (state&ISOLA_STATE_POLYGONSMOOTH) {glEnable(GL_POLYGON_SMOOTH);}
	else {glDisable(GL_POLYGON_SMOOTH);}
	if (state&ISOLA_STATE_POINTSIZEPROGRAM) {glEnable(GL_PROGRAM_POINT_SIZE);}
	else {glDisable(GL_PROGRAM_POINT_SIZE);}
	if (state&ISOLA_STATE_MULTISAMPLE) {glEnable(GL_MULTISAMPLE);}
	else {glDisable(GL_MULTISAMPLE);}
}




unsigned int isola_shader_compile(const char* shaderFile,
		unsigned int shaderType){

	unsigned int shaderObject;
	FILE* f;
	int l;

	f = fopen(shaderFile, "a+");
	fseek(f, 0, SEEK_END);
	l = ftell(f);
	if(!l){
		SDL_Log("Shader file missing or empty\n");
		fclose(f);return 0;}
	if(l>ISOLA_GLSLCHARMAX){
		SDL_Log("Shader exceeds character limit (defined in isola.h)\n");
		fclose(f);return 0;
	}
	memset(isola_shaderSrc, 0, ISOLA_GLSLCHARMAX);
	fseek(f, 0, SEEK_SET);
	fread(isola_shaderSrc, 1, l, f);
	fclose(f);
		
	shaderObject = glCreateShader(shaderType);
	glShaderSource(shaderObject,1,(const char**)&isola_shaderSrc,0);
	glCompileShader(shaderObject);

#ifdef ISOLA_DBG
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &l);
	if(!l){
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &l);
		glGetShaderInfoLog(shaderObject, l, &l, isola_shaderSrc);
		SDL_Log("Compilation failed  :  %s\n\n",isola_shaderSrc);
		return 0;
	}
#endif

	return shaderObject;
}


unsigned int isola_shader_buildProgram(const char* vertShaderFile,
		const char* fragShaderFile){

	unsigned int sp;
	unsigned int vs, fs;
	int length;

	sp = glCreateProgram();
	vs = isola_shader_compile(vertShaderFile,GL_VERTEX_SHADER);
	fs = isola_shader_compile(fragShaderFile,GL_FRAGMENT_SHADER);
	glAttachShader(sp,vs);
	glAttachShader(sp,fs);
	
	glLinkProgram(sp);

#ifdef ISOLA_DBG
	glGetProgramiv(sp, GL_LINK_STATUS, &length);
	if(!length){
		glGetProgramiv(sp, GL_INFO_LOG_LENGTH, &length);
		glGetProgramInfoLog(sp, length, &length, isola_shaderSrc);
		SDL_Log("Compilation failed  :  %s\n\n",isola_shaderSrc);
		return 0;
	}
#endif

	glDetachShader(sp,vs);
	glDetachShader(sp,fs);
	glDeleteShader(vs);
	glDeleteShader(fs);

	return sp;
}


char* isola_shader_srcLoad(const char* shaderFile){

	FILE* file;
	int length;
	char* shaderSrc;

	shaderSrc = calloc(ISOLA_GLSLCHARMAX+1, sizeof(char));
	file = fopen(shaderFile, "a+");
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	if(!length){
		SDL_Log("Shader file missing or empty\n");
		fclose(file);return 0;
	}
	if(length>ISOLA_GLSLCHARMAX){
		SDL_Log("Shader exceeds character limit (defined in isola.h)\n");
		fclose(file);return 0;
	}
	memset(shaderSrc, 0, ISOLA_GLSLCHARMAX);
	fseek(file, 0, SEEK_SET);
	(void)fread(shaderSrc, 1, length, file);
	fclose(file);

	return shaderSrc;
}


unsigned char isola_shader_srcCompare(char* shaderSrc, const char* shaderFile){

	int l;
	FILE* f;

	f = fopen(shaderFile, "a+");
	fseek(f, 0, SEEK_END);
	l = ftell(f);
	if(!l){SDL_Log("Shader file missing or empty\n");fclose(f);return 0;}
	if(l>ISOLA_GLSLCHARMAX){SDL_Log("Shader exceeds character limit \
			(defined in isola.h)\n");fclose(f);return 0;}
	memset(isola_shaderSrc, 0, ISOLA_GLSLCHARMAX);
	fseek(f, 0, SEEK_SET);
	fread(isola_shaderSrc, 1, l, f);
	fclose(f);
	
	if(strcmp(shaderSrc, isola_shaderSrc)){
		memset(shaderSrc, 0, ISOLA_GLSLCHARMAX);
		memcpy(shaderSrc, isola_shaderSrc, l);
		return 1;
	}

	return 0;
}




static void isola_contextPromt(void){

	int maj, min, flags, prof;

	SDL_Log("\n");
	SDL_Log("Vendor        : %s", glGetString(GL_VENDOR));
	SDL_Log("Renderer      : %s", glGetString(GL_RENDERER));
	SDL_Log("Version       : %s", glGetString(GL_VERSION));
	SDL_Log("GLSL          : %s", glGetString(GL_SHADING_LANGUAGE_VERSION));


	SDL_Log("\n");
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &maj);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &min);
	SDL_Log("Context(SDL)  : %d.%d", maj, min);

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &prof);
	if(prof == GL_CONTEXT_CORE_PROFILE_BIT){
		SDL_Log(" core profile");
	}else if(prof == GL_CONTEXT_COMPATIBILITY_PROFILE_BIT){
		SDL_Log(" compatibility profile");
	}

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &flags);
	if(flags & SDL_GL_CONTEXT_DEBUG_FLAG){
		SDL_Log(" debug context");
	}else{
		SDL_Log(" non-debug context");
	}

	SDL_Log("\n");
	glGetIntegerv(GL_MAJOR_VERSION, &maj);
	glGetIntegerv(GL_MINOR_VERSION, &min);
	SDL_Log("Context(GL)   : %d.%d", maj, min);

	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &prof);
	if(prof == GL_CONTEXT_CORE_PROFILE_BIT){
		SDL_Log(" core profile");
	}else if(prof == GL_CONTEXT_COMPATIBILITY_PROFILE_BIT){
		SDL_Log(" compatibility profile");
	}

	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if(flags & GL_CONTEXT_FLAG_DEBUG_BIT){
		SDL_Log(" debug context");
	}else{
		SDL_Log(" non-debug context");
	}

	SDL_Log("\n\n\n");
}


void isola_init(void){

	int contextFlags = 0;

	isola_shaderSrc = calloc(ISOLA_GLSLCHARMAX+1, sizeof(char));
	if (!isola_shaderSrc) {SDL_Log("Failed allocation");exit(EXIT_FAILURE);}

#if ISOLA_CONFIG_LOG
	isolaLog = freopen("isola.log","a+",stderr);
#endif
	
	SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER |
			ISOLA_GAMEPAD*(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) );


	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, ISOLA_MAJOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, ISOLA_MINOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, ISOLA_PROFILE);

#ifdef ISOLA_DBG
	contextFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif
#ifdef __APPLE__
	contextFlags |= SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
#endif
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, contextFlags);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

	SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 0);

#if(ISOLA_ALPHA)
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);
#endif

#if(ISOLA_DEPTH)
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
#else
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
#endif

#if(ISOLA_STENCIL)
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
#else
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
#endif

#if(!ISOLA_DOUBLEBUFFER)
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);
#endif

#if(ISOLA_MSANTIALIASING)
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, ISOLA_MSANTIALIASING);
#endif

	isola_window = SDL_CreateWindow( ISOLA_WINDOWTITLE,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			ISOLA_WINDOWWIDTH, ISOLA_WINDOWHEIGHT,
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );

	if (!isola_window) {
		isola_error_sdl(-1);
		SDL_Log("window creation failed, using default sdl hints");
		SDL_Log("\n");

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 3);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 3);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 2);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 0);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 0);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 0);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_STEREO, 0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
				SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

		isola_window = SDL_CreateWindow( ISOLA_WINDOWTITLE,
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				ISOLA_WINDOWWIDTH, ISOLA_WINDOWHEIGHT,
				SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
		if (!isola_window) {
			SDL_Log("window creation failed (again)");
			SDL_Log("\n");
		}
	}

	isola_context = SDL_GL_CreateContext(isola_window);
	SDL_GL_MakeCurrent(isola_window, isola_context);


	/* glewExperimental = GL_TRUE; */
	glewInit();


#ifdef ISOLA_DBG
	isola_contextPromt();
#endif

#ifdef ISOLA_DBG
	if( GLEW_KHR_debug || GLEW_ARB_debug_output ){
		glDebugMessageCallbackARB(&debugCallback, 0);
	}
#endif


#if(ISOLA_DEPTH)
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glClearDepth(1);
#else
	glDisable(GL_DEPTH_TEST);
#endif

#if(ISOLA_STENCIL)
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0xff);
	glStencilFuncSeparate(GL_FRONT_AND_BACK,GL_ALWAYS,0xff,0xff);
	glStencilOpSeparate(GL_FRONT_AND_BACK,GL_KEEP,GL_KEEP,GL_REPLACE);
	glClearStencil(0);
#else
	glDisable(GL_STENCIL_TEST);
#endif

#if(ISOLA_ALPHA)
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	glBlendEquationSeparate(GL_FUNC_ADD,GL_FUNC_ADD);
	/* glDepthMask(GL_FALSE); */
#else
	glDisable(GL_BLEND);
#endif

#if(ISOLA_MSANTIALIASING)
	glEnable(GL_MULTISAMPLE);
#else
	glDisable(GL_MULTISAMPLE);
#endif


	glClearColor(0.5f,0.375f,0.5f,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
/* 	SDL_GL_SwapWindow(isola_window); */


#ifdef ISOLA_DBG
	isola_get_context();
	isola_get_state();
#endif
	isola_get_window();
	isola_get_display();
	SDL_GL_SetSwapInterval(ISOLA_VSYNC);
}


void isola_quit(void){

#ifdef ISOLA_DBG
	if( isola_error_gl() || isola_error_sdl(-1) ){
		SDL_Log("UNCAUGHT ERRORS LEFT");
	}
#endif


#if ISOLA_CONFIG_LOG
	fclose(isolaLog);
#endif
	free(isola_shaderSrc);
	free(isola_info_display.displayModeCount);
	free(isola_info_display.displayModes);
	SDL_GL_DeleteContext(isola_context);
	SDL_DestroyWindow(isola_window);
	SDL_Quit();
}




