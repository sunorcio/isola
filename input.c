#include "input.h"




#include <stdlib.h>


#include <SDL3/SDL.h>




/* move to isola config if stablished */
#define TEXT_CHAR_SIZE 32
#define MAX_TEXT_STRING_LENGTH 32




static int isola_keyNum;
static unsigned int isola_textCursor;
static unsigned int isola_textCharLength;
static void isola_buildTextString(void);




static int isola_keyNum;
const unsigned char* isola_keyState;
unsigned char isola_keyRepeat[SDL_SCANCODE_COUNT];


void isola_inputClear(SDL_Window* isola_window){

	if (isola_keyState == 0) {
		isola_keyState = (unsigned char*)SDL_GetKeyboardState(&isola_keyNum);
	}

	if (SDL_IsTextInputActive()) {SDL_StopTextInput(isola_window);}

/* 	memset(keyRepeat,0,keyNum*sizeof(unsigned char)); */
}


void isola_inputRepeat(void){

	memcpy(isola_keyRepeat,isola_keyState,
			isola_keyNum*sizeof(unsigned char));
}




static unsigned int isola_textCursor = 0;
static unsigned int isola_textCharLength = 0;
char(* isola_textChars)[TEXT_CHAR_SIZE] = {0};
char* isola_textString = {0};
unsigned char isola_textEditing = 0;


static void isola_buildTextString(void){

	memset(isola_textString,0,
			isola_textCharLength*sizeof(char)*TEXT_CHAR_SIZE);
	{unsigned int i;
	for(i = 0;i<isola_textCharLength;i++){
		strcat(isola_textString,isola_textChars[i]);
	}}
}


void isola_textEditStart(unsigned int textLength, char(* textLoad)[32],
		SDL_Window* isola_window){

	if (isola_textEditing) {
		return;
	}
	if (!textLength) {
		SDL_Log("%s : text length must not be zero",__FUNCTION__);
		return;
	}
	if (MAX_TEXT_STRING_LENGTH && textLength > MAX_TEXT_STRING_LENGTH) {
		SDL_Log("%s : text length cannot exceed %d",
				__FUNCTION__,MAX_TEXT_STRING_LENGTH);
		return;
	}


	isola_textCharLength = textLength;
	isola_textChars = calloc(sizeof(char)*TEXT_CHAR_SIZE,
			isola_textCharLength);
	isola_textString = calloc(sizeof(char),
			TEXT_CHAR_SIZE*isola_textCharLength);
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
		free(isola_textChars);
		free(isola_textString);
	}else {
		return;
	}

	SDL_StopTextInput(isola_window);
	isola_textEditing = 0;
}




