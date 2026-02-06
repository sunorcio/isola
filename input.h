#ifndef INPUT_H
#define INPUT_H




#include <SDL3/SDL.h>




extern const unsigned char* isola_keyState;
extern unsigned char isola_keyRepeat[SDL_SCANCODE_COUNT];
extern unsigned char isola_mouseButtonState[5];
extern unsigned char isola_mouseButtonRepeat[5];
extern float isola_mousePos[2];


extern void isola_inputClear(SDL_Window* isola_window);
extern void isola_inputRepeat(void);




extern char* isola_text_string;
extern unsigned char isola_textEditing;


extern void isola_textEditStart(unsigned int textLength,
		char(* textLoad)[32], SDL_Window* isola_window);
extern void isola_textEditPush(char(* textChar)[32]);
extern void isola_textEditPop(void);
extern void isola_textEditStop(SDL_Window* isola_window);




#endif
