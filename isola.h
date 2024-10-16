#ifndef ISOLA_H
#define ISOLA_H




#include "isola_config.h"




#if defined(ISOLA_GAMEPAD) && (ISOLA_GAMEPAD != 0 && ISOLA_GAMEPAD != 1)
 #error ISOLA_GAMEPAD: invalid definition
#endif
#if defined(ISOLA_MSANTIALIASING) && (ISOLA_MSANTIALIASING < 0                \
			|| ISOLA_MSANTIALIASING > 16 || ISOLA_MSANTIALIASING%2)
 #error ISOLA_MSANTIALIASING: invalid definition
#endif
#if defined(ISOLA_VSYNC) && (ISOLA_VSYNC != 0 && ISOLA_VSYNC != 1)
 #error ISOLA_VSYNC: invalid definition
#endif
#if defined(ISOLA_GLSLCHARMAX) && ISOLA_GLSLCHARMAX <= 0
 #error ISOLA_GLSLCHARMAX: invalid definition
#endif


#if(ISOLA_CONFIG_DEBUG)
 #define ISOLA_DBG 1
#endif




#include <SDL2/SDL.h>
#include <GL/glew.h>




/* extended replacement for SDL_Init() */ 
extern void isolaInit(void);
/* extended replacement for SDL_Quit() */ 
extern void isolaQuit(void);




#ifdef ISOLA_DBG
 #define isolaGLDBG_(x)                                                         \
	if(isolaErrorGL()){                                                       \
		SDL_Log(" UNEXPECTED ERROR, line : %d, function : %d, file : %s",     \
				__LINE__,__FUNCTION__,__FILE__);                              \
	}                                                                         \
	x;                                                                        \
	if(isolaErrorGL()){                                                       \
		SDL_Log(" ^- in line : %d, function : %d, file : %s\n",               \
				__LINE__,__FUNCTION__,__FILE__);                              \
	}
#else
 #define isolaGLDBG_(x) x;
#endif


/* print & clear sdl errors, returns -1 if error, else 0 */
extern signed char isolaErrorSDL(int SDLfunction);
/* print & clear opengl errors, returns -1 if error, else 0 */
extern signed char isolaErrorGL(void);




extern SDL_GLContext* isolaContext;
extern SDL_Window* isolaWindow;


/* system resources and specification related information */
struct ISOLA_Context{
	int fbdefRedsize;
	int fbdefGreensize;
	int fbdefBluesize;
	int fbdefAlphasize;
	int fbdefDepthsize;
	int fbdefStencilsize;		/* GL_LINEAR or GL_SRGB */
	int fbdefColorencoding;
	int fbdefDoublebuffer;

	int maxVertices;
	int maxIndices;
	int maxAttrib;				/* at least 16 */
	int maxVertexUniforms;		/* at least 1024 */
	int maxFragmentUniforms;	/* at least 1024 */
	int maxTexCombinedUnits;	/* at least 48 */
	int maxTexUnits;			/* at least 16 */
	int maxTexSize;				/* at least 1024 */
	int max3DTexSize;			/* at least 64 */
	int maxCubeTexSize;			/* at least 1024 */
	int maxDrawBuffers;			/* at least 8 */
	int maxColorAttachments;	/* at least 8 */
	int maxRenderbufferSize;    /* at least 8 */

	int cpuCount;
	int systemRAM;
	int cacheSize;
}extern isolaInfoContext;


/* current window state. updated with isolaGetWindow() */
struct ISOLA_Window{
	int xpos, ypos;
	int width, height;
	double xratio, yratio;
	double pixelWidth, pixelHeight;
	unsigned int flags;	/* see SDL_WindowFlags for a list of flags */
	int displayIndex;
	SDL_DisplayMode displayMode;
	SDL_DisplayMode desktopDisplayMode;
	float clearColor[4];
}extern isolaInfoWindow;


/* dysplay modes for all availiable devices. retrieved with isolaGetDisplay() */
struct ISOLA_Display{
/* 	number of display modes for the id, access array with desired display id */
	int* displayModeCount;
/*	contains ALL display modes, for specific display id, its modes range from
	(sum of previous counts - 1) to (sum of previous counts + count - 1) */
	SDL_DisplayMode* displayModes;
}extern isolaInfoDisplay;


typedef enum {
	ISOLA_STATE_NONE				= 0x00000000,
	ISOLA_STATE_BLEND				= 0x00000001,
	ISOLA_STATE_COLORLOGIC			= 0x00000002,
	ISOLA_STATE_CULLFACE 			= 0x00000004,
	ISOLA_STATE_DEPTHTEST			= 0x00000008,
	ISOLA_STATE_DITHER				= 0x00000010,
	ISOLA_STATE_SCISSORTEST 		= 0x00000020,
	ISOLA_STATE_STENCILTEST 		= 0x00000040,
	ISOLA_STATE_SRGBFRAMEBUFFER 	= 0x00000080,
	ISOLA_STATE_POINTSMOOTH 		= 0x00000100,
	ISOLA_STATE_LINESMOOTH			= 0x00000200,
	ISOLA_STATE_POLYGONSMOOTH		= 0x00000400,
	ISOLA_STATE_POINTSIZEPROGRAM	= 0x00000800,
	ISOLA_STATE_MULTISAMPLE 		= 0x00001000
}ISOLA_State;
/* glEnable information. meant for quick debugging. see isolaGetState() */
extern ISOLA_State isolaInfoState;


/* update window information (isolaInfoWindow) */
extern void isolaGetWindow(void);


/* retrieve all availiable display information (isolaInfoDisplay) */
extern void isolaGetDisplay(void);


/* update currently enabled opengl state (isolaInfoState) */
extern void isolaGetState(void);


/* sets opengl state to match an ISOLA_State snapshot */
extern void isolaSetState(ISOLA_State state);




/* compile source file into returned opengl shader object id */
extern unsigned int isolaShaderCompile(const char* shaderFile,
		unsigned int shaderType);


/* compile and link source files into returned opengl shader program */
extern unsigned int isolaShaderProgram(const char* vertShaderFile,
		const char* fragShaderFile);


/* loads srcfile into heap, returns a pointer that can be issued to
   isolaShaderSrcCompare(), you must free this pointer yourself */
extern char* isolaShaderSrcLoad(const char* shaderFile);


/* compares srcfile with loaded src, if !=, src is updated and return !0 */
extern unsigned char isolaShaderSrcCompare(char* shaderSrc,
		const char* shaderFile);




#endif
