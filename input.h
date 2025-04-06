#ifndef INPUT_H
#define INPUT_H




#include <SDL2/SDL.h>




extern const unsigned char* isola_input_keyState;
extern unsigned char isola_input_keyRepeat[SDL_NUM_SCANCODES];


extern void isola_input_clear(void);
extern void isola_input_repeat(void);




extern char* isola_input_text_string;
extern unsigned char isola_input_textEditing;


extern void isola_input_textEditStart(unsigned int textLength,
		char(* textLoad)[32]);
extern void isola_input_textEditPush(char(* textChar)[32]);
extern void isola_input_textEditPop(void);
extern void isola_input_textEditStop(void);




#endif
#ifdef INPUT_C




static int isola_input_keyNum;
static unsigned int isola_input_textCursor;
static unsigned int isola_input_textCharLength;


static void isola_input_buildTextString(void);




#endif
