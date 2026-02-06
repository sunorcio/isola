#include "input.h"




#include <SDL3/SDL.h>




#define TEXT_CHAR_SIZE 32
#define MAX_TEXT_STRING_LENGTH 32




static int isola_keyNum;
static unsigned int isola_buttonFlags;
static unsigned int isola_textCursor;
static unsigned int isola_textCharLength;
static void isola_buildTextString(void);




const unsigned char* isola_keyState;
unsigned char isola_keyRepeat[SDL_SCANCODE_COUNT];
unsigned char isola_mouseButtonState[6];
unsigned char isola_mouseButtonRepeat[6];
float isola_mousePos[2];


void isola_inputClear(SDL_Window* isola_window){

	if (isola_keyState == 0) {
		isola_keyState =
				(const unsigned char*)SDL_GetKeyboardState(&isola_keyNum);
	}

	if (SDL_TextInputActive(isola_window)) {SDL_StopTextInput(isola_window);}

/* 	SDL_memset(keyRepeat,0,keyNum*sizeof(unsigned char)); */
}


void isola_inputRepeat(void){

	SDL_memcpy(isola_mouseButtonRepeat,isola_mouseButtonState,
			5*sizeof(unsigned char));

	isola_buttonFlags = SDL_GetMouseState(&isola_mousePos[0],&isola_mousePos[1]);
	isola_mouseButtonState[1] = (isola_buttonFlags&SDL_BUTTON_LMASK) != 0;
	isola_mouseButtonState[2] = (isola_buttonFlags&SDL_BUTTON_MMASK) != 0;
	isola_mouseButtonState[3] = (isola_buttonFlags&SDL_BUTTON_RMASK) != 0;
	isola_mouseButtonState[4] = (isola_buttonFlags&SDL_BUTTON_X1MASK) != 0;
	isola_mouseButtonState[5] = (isola_buttonFlags&SDL_BUTTON_X2MASK) != 0;

	SDL_memcpy(isola_keyRepeat,isola_keyState,
			(Uint64)isola_keyNum*sizeof(unsigned char));
}




static unsigned int isola_textCursor = 0;
static unsigned int isola_textCharLength = 0;
static char(* isola_textChars)[TEXT_CHAR_SIZE] = {0};
static char* isola_textString = {0};
unsigned char isola_textEditing = 0;


static void isola_buildTextString(void){

	SDL_memset(isola_textString,0,
			isola_textCharLength*sizeof(char)*TEXT_CHAR_SIZE);
	{unsigned int i;
	for(i = 0;i<isola_textCharLength;i++){
		SDL_strlcat(isola_textString,isola_textChars[i],MAX_TEXT_STRING_LENGTH);
	}}
}


void isola_textEditStart(unsigned int textLength, char(* textLoad)[32],
		SDL_Window* isola_window){

	if (isola_textEditing || !textLength) {
		return;
	}
	if (MAX_TEXT_STRING_LENGTH && textLength > MAX_TEXT_STRING_LENGTH) {
		SDL_Log("isola_textEditStart: textLength cannot exceed %d, currently %u",
				MAX_TEXT_STRING_LENGTH,textLength);
		return;
	}


	isola_textCharLength = textLength;
	isola_textChars = SDL_calloc(isola_textCharLength,
			TEXT_CHAR_SIZE*sizeof(char));
	isola_textString = SDL_calloc(isola_textCharLength,
			TEXT_CHAR_SIZE*sizeof(char));
	isola_textCursor = 0;


	if (textLoad) {
		{unsigned int i;
		for(i = 0;i<isola_textCharLength;i++){
			{unsigned int j;
			for(j = 0;j<TEXT_CHAR_SIZE;j++){
				isola_textChars[i][j] = textLoad[i][j];
			}}
		}}
	}


	SDL_StartTextInput(isola_window);
	isola_textEditing = 1;
}


void isola_textEditPush(char(* textChar)[32]){

	if (!isola_textEditing) {
		return;
	}
	if (isola_textCursor == isola_textCharLength) {
		return;
	}

	{unsigned int i;
	for(i = 0;i<TEXT_CHAR_SIZE;i++){
		isola_textChars[isola_textCursor][i] = textChar[0][i];
	}}
	isola_textCursor++;

	isola_buildTextString();
}


void isola_textEditPop(void){

	if (!isola_textEditing) {
		return;
	}
	if (!isola_textCursor) {
		return;
	}

	isola_textCursor--;
	{unsigned int i;
	for(i = 0;i<TEXT_CHAR_SIZE;i++){
		isola_textChars[isola_textCursor][i] = 0;
	}}

	isola_buildTextString();
}


void isola_textEditStop(SDL_Window* isola_window){

	if (isola_textEditing) {
		SDL_free(isola_textChars);
		SDL_free(isola_textString);
	}else {
		return;
	}

	SDL_StopTextInput(isola_window);
	isola_textEditing = 0;
}




