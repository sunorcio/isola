#include "isola.h"




static char* isola_shaderSrc;
static void isola_get_context(void);
static void isola_contextPromt(void);




#ifdef ISOLA_DBG
signed char isola_error_sdl(int sdlFunction){

	if(*SDL_GetError() == 0){ return 0; }

	if(!sdlFunction){ SDL_Log("%s",SDL_GetError()); }
	SDL_ClearError();
	return -1;
}

signed char isola_error_gl(void){

	GLenum error = glGetError();
	if (error == GL_NO_ERROR){ return 0; }

	do{ SDL_Log("GLerror : %s",gluErrorString(error)); }
	while( (error = glGetError()) != GL_NO_ERROR );
	return -1;
}

 #if(  defined(GLEW_KHR_debug) || defined(GLEW_ARB_debug_output)  )
static void debugCallback(unsigned int source, unsigned int type,
		unsigned int id, unsigned int severity, int length,
		const char* message, const void* userParam){

	SDL_Log("GL callback: %s\n\n\n",message);
	return;
}
 #endif
#else
signed char isola_error_sdl(int sdlFunction){return 0;}
signed char isola_error_gl(void){return 0;}
#endif




SDL_GLContext isola_context = {0};
SDL_Window* isola_window = {0};


struct ISOLA_context isola_info_context = {0};
struct ISOLA_window isola_info_window = {0};
struct ISOLA_display isola_info_display = {0};
enum ISOLA_state isola_info_state = {0};


static char* isola_shaderSrc = {0};




void isola_get_window(void){

	SDL_GetWindowPosition(isola_window, &isola_info_window.xPos,
			&isola_info_window.yPos);
	SDL_GetWindowSize(isola_window, &isola_info_window.width,
			&isola_info_window.height);

	if (isola_info_window.width > isola_info_window.height) {
		isola_info_window.yRatio = 1.f;
		isola_info_window.xRatio = (float)isola_info_window.width
				/(float)isola_info_window.height;
	}else {
		isola_info_window.xRatio = 1.f;
		isola_info_window.yRatio = (float)isola_info_window.height 
				/(float)isola_info_window.width;
	}
	isola_info_window.pixelWidth = 2.f/(float)isola_info_window.width;
	isola_info_window.pixelHeight = 2.f/(float)isola_info_window.height;

	isola_info_window.flags = SDL_GetWindowFlags(isola_window);
	isola_info_window.displayIndex = SDL_GetDisplayForWindow(isola_window);
	isola_info_window.displayMode = SDL_GetWindowFullscreenMode(isola_window);
	isola_info_window.desktopDisplayMode = SDL_GetDesktopDisplayMode(isola_info_window.displayIndex);

	glGetFloatv(GL_COLOR_CLEAR_VALUE,isola_info_window.clearColor);
}


void isola_get_displays(void){

/* 	int numDisplays,numDisplaymodes;
	SDL_DisplayID* displayList;
	SDL_DisplayMode** displaymodeList; */

/* 	displayList = SDL_GetDisplays(&numDisplays); */
	isola_info_display.displaymodeList = SDL_GetFullscreenDisplayModes(
			isola_info_window.displayIndex, &isola_info_display.numDisplaymodes);
}


static void isola_get_context(void){

	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
			GL_FRONT_LEFT,GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE,
			&isola_info_context.fbdef_redSize);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
			GL_FRONT_LEFT,GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE,
			&isola_info_context.fbdef_greenSize);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
			GL_FRONT_LEFT,GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE,
			&isola_info_context.fbdef_blueSize);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
			GL_FRONT_LEFT,GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE,
			&isola_info_context.fbdef_alphaSize);
#if ISOLA_DEPTH
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
			GL_DEPTH,GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE,
			&isola_info_context.fbdef_depthSize);
#endif
#if ISOLA_STENCIL
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
			GL_STENCIL,GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE,
			&isolaInfoContext.fbdefStencilsize);
#endif
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
			GL_FRONT_LEFT,GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING,
			&isola_info_context.fbdef_colorEncoding);

	glGetIntegerv(GL_DOUBLEBUFFER,&isola_info_context.fbdef_doubleBuffer);

	glGetIntegerv(GL_MAX_ELEMENTS_VERTICES,&isola_info_context.max_vertices);
	glGetIntegerv(GL_MAX_ELEMENTS_INDICES,&isola_info_context.max_indices);
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,&isola_info_context.max_attrib);
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS,
			&isola_info_context.max_vertexUniforms);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
			&isola_info_context.max_fragmentUniforms);
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
			&isola_info_context.max_texCombinedUnits);
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&isola_info_context.max_texUnits);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE,&isola_info_context.max_texSize);
	glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE,&isola_info_context.max_3dTexSize);
	glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE,
			&isola_info_context.max_cubeTexSize);
	glGetIntegerv(GL_MAX_DRAW_BUFFERS,&isola_info_context.max_drawBuffers);
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS,
			&isola_info_context.max_colorAttachments);
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE,
			&isola_info_context.max_renderbufferSize);

	isola_info_context.cpuCount = SDL_GetNumLogicalCPUCores();
	isola_info_context.systemRAM = SDL_GetSystemRAM();
	isola_info_context.cacheSize = SDL_GetCPUCacheLineSize();
}


void isola_get_state(void){

	int state = {0};


	isola_info_state = 0x00000000;
	glGetIntegerv(GL_BLEND,&state);
	isola_info_state = (isola_info_state |
			(state!=0)*ISOLA_STATE_BLEND);
	glGetIntegerv(GL_COLOR_LOGIC_OP,&state);
	isola_info_state = (isola_info_state |
			(state!=0)*ISOLA_STATE_COLORLOGIC);
	glGetIntegerv(GL_CULL_FACE,&state);
	isola_info_state = (isola_info_state |
			(state!=0)*ISOLA_STATE_CULLFACE);
	glGetIntegerv(GL_DEPTH_TEST,&state);
	isola_info_state = (isola_info_state |
			(state!=0)*ISOLA_STATE_DEPTHTEST);
	glGetIntegerv(GL_DITHER,&state);
	isola_info_state = (isola_info_state |
			(state!=0)*ISOLA_STATE_DITHER);
	glGetIntegerv(GL_SCISSOR_TEST,&state);
	isola_info_state = (isola_info_state |
			(state!=0)*ISOLA_STATE_SCISSORTEST);
	glGetIntegerv(GL_STENCIL_TEST,&state);
	isola_info_state = (isola_info_state |
			(state!=0)*ISOLA_STATE_STENCILTEST);
	glGetIntegerv(GL_FRAMEBUFFER_SRGB,&state);
	isola_info_state = (isola_info_state |
			(state!=0)*ISOLA_STATE_SRGBFRAMEBUFFER);
/* 	glGetIntegerv(GL_POINT_SMOOTH,&state);
	isola_info_state = (isola_info_state |
			(state!=0)*ISOLA_STATE_POINTSMOOTH); */
	glGetIntegerv(GL_LINE_SMOOTH,&state);
	isola_info_state = (isola_info_state |
			(state!=0)*ISOLA_STATE_LINESMOOTH);
	glGetIntegerv(GL_POLYGON_SMOOTH,&state);
	isola_info_state = (isola_info_state |
			(state!=0)*ISOLA_STATE_POLYGONSMOOTH);
	glGetIntegerv(GL_PROGRAM_POINT_SIZE, &state);
	isola_info_state = (isola_info_state |
			(state!=0)*ISOLA_STATE_POINTSIZEPROGRAM);
	glGetIntegerv(GL_MULTISAMPLE,&state);
	isola_info_state = (isola_info_state |
			(state!=0)*ISOLA_STATE_MULTISAMPLE);
}


void isola_set_state(enum ISOLA_state state){

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
/* 	if (state&ISOLA_STATE_POINTSMOOTH) {glEnable(GL_POINT_SMOOTH);}
	else {glDisable(GL_POINT_SMOOTH);} */
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
	GLint temp;
	char* data;
	size_t size;


	data = SDL_LoadFile(shaderFile,&size);
	if(!data){
		SDL_Log("isola_shader_compile: failed loading file '%s'",shaderFile);
		isola_error_sdl(0);
		SDL_Log("\n\n");
		return 0;
	}else if(size>ISOLA_GLSLCHARMAX){
		SDL_Log("isola_shader_compile: shader exceeds character limit "
				"(defined as ISOLA_GLSLCHARMAX in isola_config.h)\n"
				"current limit: %d\n""requested file: %"SDL_PRIu64"\n\n\n",
				ISOLA_GLSLCHARMAX,size);
		SDL_free(data);
		return 0;
	}

	SDL_memset(isola_shaderSrc,0,ISOLA_GLSLCHARMAX);
	SDL_memcpy(isola_shaderSrc,data,size);
	SDL_free(data);


	shaderObject = glCreateShader(shaderType);
	glShaderSource(shaderObject,1,(const char*const*)&isola_shaderSrc,0);
	glCompileShader(shaderObject);

#ifdef ISOLA_DBG
	glGetShaderiv(shaderObject,GL_COMPILE_STATUS,&temp);
	if(!temp){
		glGetShaderiv(shaderObject,GL_INFO_LOG_LENGTH,&temp);
		glGetShaderInfoLog(shaderObject,temp,&temp,isola_shaderSrc);
		isola_error_gl();
		SDL_Log("isola_shader_compile: failed '%s' compilation\n"
				"log: %s\n\n\n",shaderFile,isola_shaderSrc);
		return 0;
	}
	glGetShaderiv(shaderObject,GL_INFO_LOG_LENGTH,&temp);
	if(temp){
		glGetShaderInfoLog(shaderObject,temp,&temp,isola_shaderSrc);
		SDL_Log("isola_shader_compile: completed '%s' compilation\n"
				"log: %s\n\n\n",shaderFile,isola_shaderSrc);
	}
#endif

	return shaderObject;
}


unsigned int isola_shader_buildProgram(const char* vertShaderFile,
		const char* fragShaderFile){

	unsigned int sp;
	unsigned int vs, fs;
	GLint temp;


	sp = glCreateProgram();
	vs = isola_shader_compile(vertShaderFile,GL_VERTEX_SHADER);
	fs = isola_shader_compile(fragShaderFile,GL_FRAGMENT_SHADER);
	if (!vs || !fs){
		return 0;
	}

	glAttachShader(sp,vs);
	glAttachShader(sp,fs);
	
	glLinkProgram(sp);

#ifdef ISOLA_DBG
	glGetProgramiv(sp, GL_LINK_STATUS, &temp);
	if(!temp){
		glGetProgramiv(sp, GL_INFO_LOG_LENGTH, &temp);
		glGetProgramInfoLog(sp,temp,&temp,isola_shaderSrc);
		isola_error_gl();
		SDL_Log("isola_shader_buildProgram: failed to link '%s', '%s'\n"
				"log: %s\n\n\n",vertShaderFile,fragShaderFile,isola_shaderSrc);
		return 0;
	}
	glGetProgramiv(sp,GL_INFO_LOG_LENGTH,&temp);
	if(temp){
		glGetProgramInfoLog(sp,temp,&temp,isola_shaderSrc);
		SDL_Log("isola_shader_buildProgram: finished linking '%s', '%s'\n"
				"log: %s\n\n\n",vertShaderFile,fragShaderFile,isola_shaderSrc);
	}
#endif

	glDetachShader(sp,vs);
	glDetachShader(sp,fs);
	glDeleteShader(vs);
	glDeleteShader(fs);

	return sp;
}


char* isola_shader_srcLoad(const char* shaderFile){

	char* shaderSrc;
	size_t size;


	shaderSrc = SDL_LoadFile(shaderFile,&size);

	if(!shaderSrc){
		SDL_Log("isola_shader_srcLoad: failed loading file '%s'",shaderFile);
		isola_error_sdl(0);
		SDL_Log("\n\n");
		return 0;
	}else if(size>ISOLA_GLSLCHARMAX){
		SDL_Log("isola_shader_srcLoad: shader exceeds character limit "
				"(defined as ISOLA_GLSLCHARMAX in isola_config.h)\n"
				"current limit: %d\n""requested file: %"SDL_PRIu64"\n\n\n",
				ISOLA_GLSLCHARMAX,size);
		SDL_free(shaderSrc);
		return 0;
	}

	return shaderSrc;
}


unsigned char isola_shader_srcCompare(char* shaderSrc, const char* shaderFile){

	char* data;
	size_t size;


	data = SDL_LoadFile(shaderFile,&size);
	if(!data){
		SDL_Log("isola_shader_srcCompare: failed loading file '%s'",shaderFile);
		isola_error_sdl(0);
		SDL_Log("\n\n");
		return 0;
	}else if(size>ISOLA_GLSLCHARMAX){
		SDL_Log("isola_shader_srcCompare: shader exceeds character limit "
				"(defined as ISOLA_GLSLCHARMAX in isola_config.h)\n"
				"current limit: %d\n""requested file: %"SDL_PRIu64"\n\n\n",
				ISOLA_GLSLCHARMAX,size);
		SDL_free(data);
		return 0;
	}

	SDL_memset(isola_shaderSrc,0,ISOLA_GLSLCHARMAX);
	SDL_memcpy(isola_shaderSrc,data,size);
	SDL_free(data);
	

	if(SDL_strcmp(shaderSrc,isola_shaderSrc)){
		SDL_memset(shaderSrc,0,ISOLA_GLSLCHARMAX);
		SDL_memcpy(shaderSrc,isola_shaderSrc,size);
		return 1;
	}

	return 0;
}




static void isola_contextPromt(void){

	int vers, maj, min, prof, flags;


	SDL_Log("Vendor          : %s",glGetString(GL_VENDOR));
	SDL_Log("Renderer        : %s",glGetString(GL_RENDERER));
	SDL_Log("GL Version      : %s",glGetString(GL_VERSION));
	SDL_Log("GLSL            : %s",glGetString(GL_SHADING_LANGUAGE_VERSION));
	SDL_Log("\n");

	vers = SDL_GetVersion();
	SDL_Log("Platform        : %s",SDL_GetPlatform());
	SDL_Log("SDL (revision)  : %s",SDL_GetRevision());
	SDL_Log("SDL (header)    : %d.%d.%d",SDL_VERSIONNUM_MAJOR(SDL_VERSION),
			SDL_VERSIONNUM_MINOR(SDL_VERSION),SDL_VERSIONNUM_MICRO(SDL_VERSION));
	SDL_Log("SDL (shared)    : %d.%d.%d",SDL_VERSIONNUM_MAJOR(vers),
			SDL_VERSIONNUM_MINOR(vers),SDL_VERSIONNUM_MICRO(vers));
	SDL_Log("\n");


	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,&maj);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,&min);
	SDL_Log("GL Context(SDL) : %d.%d", maj, min);

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,&prof);
	if(prof == GL_CONTEXT_CORE_PROFILE_BIT){
		SDL_Log(" core profile");
	}else if(prof == GL_CONTEXT_COMPATIBILITY_PROFILE_BIT){
		SDL_Log(" compatibility profile");
	}

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS,&flags);
	if(flags & SDL_GL_CONTEXT_DEBUG_FLAG){
		SDL_Log(" debug context");
	}else{
		SDL_Log(" non-debug context");
	}

	SDL_Log("\n");
	glGetIntegerv(GL_MAJOR_VERSION,&maj);
	glGetIntegerv(GL_MINOR_VERSION,&min);
	SDL_Log("GL Context(GL)  : %d.%d", maj,min);

	glGetIntegerv(GL_CONTEXT_PROFILE_MASK,&prof);
	if(prof == GL_CONTEXT_CORE_PROFILE_BIT){
		SDL_Log(" core profile");
	}else if(prof == GL_CONTEXT_COMPATIBILITY_PROFILE_BIT){
		SDL_Log(" compatibility profile");
	}

	glGetIntegerv(GL_CONTEXT_FLAGS,&flags);
	if(flags & GL_CONTEXT_FLAG_DEBUG_BIT){
		SDL_Log(" debug context");
	}else{
		SDL_Log(" non-debug context");
	}

	SDL_Log("\n\n\n\n");
}


unsigned char isola_init(void){

	int contextFlags = 0;


	SDL_Log("\n\n\n\n");
	isola_shaderSrc = SDL_calloc(ISOLA_GLSLCHARMAX+1,sizeof(char));
	if(!isola_shaderSrc) {
		SDL_Log("isola_init: failed shader buffer allocation");
		return 0;
	}
	
	if(!SDL_Init( SDL_INIT_VIDEO | ISOLA_GAMEPAD*(SDL_INIT_GAMEPAD
			| SDL_INIT_HAPTIC) )){
		SDL_Log("isola_init: failed SDL initialization");
		isola_error_sdl(0);
		SDL_Log("\n\n");
		return 0;
	}


	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,ISOLA_MAJOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,ISOLA_MINOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,ISOLA_PROFILE);

#ifdef ISOLA_DBG
	contextFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif
#ifdef __APPLE__
	contextFlags |= SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
#endif
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,contextFlags);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,5);

	SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,0);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,0);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,0);

#if(ISOLA_ALPHA)
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,5);
#endif

#if(ISOLA_DEPTH)
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
#else
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,0);
#endif

#if(ISOLA_STENCIL)
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);
#else
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,0);
#endif

#if(!ISOLA_DOUBLEBUFFER)
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,0);
#endif

#if(ISOLA_MSANTIALIASING)
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,ISOLA_MSANTIALIASING);
#endif

	isola_window = SDL_CreateWindow( ISOLA_WINDOWTITLE,
			ISOLA_WINDOWWIDTH,ISOLA_WINDOWHEIGHT,
			SDL_WINDOW_OPENGL );

	if (!isola_window) {
		SDL_Log("isola_init: failed window creation. "
				"trying again using default sdl hints");
		isola_error_sdl(0);
		SDL_Log("\n\n");

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE,3);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,3);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,2);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,0);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,0);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,0);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,0);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,0);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
		SDL_GL_SetAttribute(SDL_GL_STEREO,0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
				SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

		isola_window = SDL_CreateWindow( ISOLA_WINDOWTITLE,
				ISOLA_WINDOWWIDTH, ISOLA_WINDOWHEIGHT,
				SDL_WINDOW_OPENGL );
		if (!isola_window) {
			SDL_Log("isola_init: failed window creation a second time");
			isola_error_sdl(0);
			SDL_Log("\n\n");
			return 0;
		}
	}

	isola_context = SDL_GL_CreateContext(isola_window);
	if (!isola_context) {
		SDL_Log("isola_init: failed GL context creation. "
				"trying again using default sdl hints");
		isola_error_sdl(0);
		SDL_Log("\n\n");

		SDL_DestroyWindow(isola_window);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,contextFlags);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE,3);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,3);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,2);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,0);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,0);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,0);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,0);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,0);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
		SDL_GL_SetAttribute(SDL_GL_STEREO,0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
				SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

		isola_window = SDL_CreateWindow( ISOLA_WINDOWTITLE,
				ISOLA_WINDOWWIDTH, ISOLA_WINDOWHEIGHT,
				SDL_WINDOW_OPENGL );

		isola_context = SDL_GL_CreateContext(isola_window);
		if (!isola_window) {
			SDL_Log("isola_init: failed GL context creation a second time");
			isola_error_sdl(0);
			SDL_Log("\n\n");
			return 0;
		}
	}
	SDL_GL_MakeCurrent(isola_window, isola_context);


	/* glewExperimental = GL_TRUE; */
	glewInit();


#ifdef ISOLA_DBG
	isola_contextPromt();
#endif

#ifdef ISOLA_DBG
	if( GLEW_KHR_debug || GLEW_ARB_debug_output ){
		glDebugMessageCallbackARB(&debugCallback,0);
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
	glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ZERO);
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
	isola_get_displays();
	SDL_GL_SetSwapInterval(ISOLA_VSYNC);


#ifdef ISOLA_DBG
	if( isola_error_gl() || isola_error_sdl(0) ){
		SDL_Log("UNCAUGHT ISOLA INITIALIZATION ERRORS");
	}
#endif

	return 1;
}


void isola_quit(void){

#ifdef ISOLA_DBG
	if( isola_error_gl() || isola_error_sdl(0) ){
		SDL_Log("UNCAUGHT APPLICATION ERRORS");
	}
#endif


	SDL_free(isola_shaderSrc);
	SDL_free(isola_info_display.displaymodeList);
	SDL_GL_DestroyContext(isola_context);
	SDL_DestroyWindow(isola_window);
	SDL_Quit();
}




