#ifndef INPUT_H
#define INPUT_H




#include <SDL2/SDL.h>




extern const unsigned char* isola_keyState;
extern unsigned char isola_keyRepeat[SDL_NUM_SCANCODES];


extern void isola_clear(void);
extern void isola_repeat(void);




extern char* isola_text_string;
extern unsigned char isola_textEditing;


extern void isola_textEditStart(unsigned int textLength,
		char(* textLoad)[32]);
extern void isola_textEditPush(char(* textChar)[32]);
extern void isola_textEditPop(void);
extern void isola_textEditStop(void);




#endif
