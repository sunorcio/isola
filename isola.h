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




#include <SDL3/SDL.h>
/* #define SDL_MAIN_USE_CALLBACKS */
/* #include <SDL3/SDL_main.h> */
#include <GL/glew.h>




/* extended replacement for SDL_Init() */ 
extern void isola_init(void);
/* extended replacement for SDL_Quit() */ 
extern void isola_quit(void);




#ifdef ISOLA_DBG
 #define ISOLA_GLDBG_(x)                                                      \
	if(isola_error_gl()){                                                       \
		SDL_Log(" UNEXPECTED ERROR,  line: %d,    function: %s,    file: %s",     \
				__LINE__,__FUNCTION__,__FILE__);                                      \
	}                                                                           \
	x;                                                                          \
	if(isola_error_gl()){                                                       \
		SDL_Log(" ^- in  line: %d,    function: %s,    file: %s\n arguments:"     \
				ISOLA_LIT_STR_(x)"\n\n\n", __LINE__, __FUNCTION__, __FILE__);         \
	}
#else
 #define ISOLA_GLDBG_(x) x;
#endif


/* print first SDL error & clear errors, returns -1 if error, else 0 */
extern signed char isola_error_sdl(int SDLfunction);
/* print first openGL error & clear errors, returns -1 if error, else 0 */
extern signed char isola_error_gl(void);




extern SDL_GLContext isola_context;
extern SDL_Window* isola_window;


/* system resources and specification related information */
struct ISOLA_context{
	int fbdef_redSize;
	int fbdef_greenSize;
	int fbdef_blueSize;
	int fbdef_alphaSize;
	int fbdef_depthSize;
	int fbdef_stencilSize; /* GL_LINEAR or GL_SRGB */
	int fbdef_colorEncoding;
	int fbdef_doubleBuffer;

	int max_vertices;
	int max_indices;
	int max_attrib; /* at least 16 */
	int max_vertexUniforms; /* at least 1024 */
	int max_fragmentUniforms; /* at least 1024 */
	int max_texCombinedUnits; /* at least 48 */
	int max_texUnits; /* at least 16 */
	int max_texSize; /* at least 1024 */
	int max_3dTexSize; /* at least 64 */
	int max_cubeTexSize; /* at least 1024 */
	int max_drawBuffers; /* at least 8 */
	int max_colorAttachments; /* at least 8 */
	int max_renderbufferSize; /* at least 8 */

	int cpuCount;
	int systemRAM;
	int cacheSize;
}extern isola_info_context;


/* current window state. updated with isola_get_window() */
struct ISOLA_window{
	int xPos, yPos;
	int width, height;
	double xRatio, yRatio;
	double pixelWidth, pixelHeight;
	unsigned int flags; /* see SDL_WindowFlags for a list of flags */
	SDL_DisplayID displayIndex;
	const SDL_DisplayMode* displayMode;
	const SDL_DisplayMode* desktopDisplayMode;
	float clearColor[4];
}extern isola_info_window;


/* initial display info
 * TODO all displays info, update according to display events*/
struct ISOLA_display{
	int numDisplaymodes;
	SDL_DisplayMode** displaymodeList;
}extern isola_info_display;


/* glEnable information. meant for quick debugging. see isola_get_state() */
enum ISOLA_state{
	ISOLA_STATE_NONE = 0x00000000,
	ISOLA_STATE_BLEND = 0x00000001,
	ISOLA_STATE_COLORLOGIC = 0x00000002,
	ISOLA_STATE_CULLFACE = 0x00000004,
	ISOLA_STATE_DEPTHTEST = 0x00000008,
	ISOLA_STATE_DITHER = 0x00000010,
	ISOLA_STATE_SCISSORTEST = 0x00000020,
	ISOLA_STATE_STENCILTEST = 0x00000040,
	ISOLA_STATE_SRGBFRAMEBUFFER = 0x00000080,
	ISOLA_STATE_POINTSMOOTH = 0x00000100,
	ISOLA_STATE_LINESMOOTH = 0x00000200,
	ISOLA_STATE_POLYGONSMOOTH = 0x00000400,
	ISOLA_STATE_POINTSIZEPROGRAM = 0x00000800,
	ISOLA_STATE_MULTISAMPLE = 0x00001000
}extern isola_info_state;


/* update window information (isola_info_window) */
extern void isola_get_window(void);


/* retrieve all availiable display information (isola_info_display) */
extern void isola_get_displays(void);


/* update currently enabled opengl state (isola_info_state) */
extern void isola_get_state(void);


/* sets opengl state to match an ISOLA_state snapshot */
extern void isola_set_state(enum ISOLA_state state);




/* compile source file into returned opengl shader object id */
extern unsigned int isola_shader_compile(const char* shaderFile,
		unsigned int shaderType);


/* compile and link source files into returned opengl shader program */
extern unsigned int isola_shader_buildProgram(const char* vertShaderFile,
		const char* fragShaderFile);


/* loads srcfile into heap, returns a pointer that can be issued to
 * isolaShaderSrcCompare(), you must free this pointer yourself */
extern char* isola_shader_srcLoad(const char* shaderFile);


/* compares srcfile with loaded src, if !=, src is updated and return !0 */
extern unsigned char isola_shader_srcCompare(char* shaderSrc,
		const char* shaderFile);




#endif
