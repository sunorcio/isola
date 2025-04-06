#define INPUT_C
#include "input.h"




#include <SDL2/SDL.h>




static int isola_input_keyNum;
const unsigned char* isola_input_keyState;
unsigned char isola_input_keyRepeat[SDL_NUM_SCANCODES];


void isola_input_clear(void){

	if (isola_input_keyState == 0) {
		isola_input_keyState = SDL_GetKeyboardState(&isola_input_keyNum);
	}

	if (SDL_IsTextInputActive()) {SDL_StopTextInput();}

/* 	memset(keyRepeat,0,keyNum*sizeof(unsigned char)); */
}


void isola_input_repeat(void){

	memcpy(isola_input_keyRepeat,isola_input_keyState,
			isola_input_keyNum*sizeof(unsigned char));
}




/* move to isola config if stablished */
#define textCharSize 32
#define maxTextStringLength 32


static unsigned int isola_input_textCursor = 0;
static unsigned int isola_input_textCharLength = 0;
char(* isola_input_textChars)[textCharSize] = {0};
char* isola_input_textString = {0};
unsigned char isola_input_textEditing = 0;


static void isola_input_buildTextString(void){

	memset(isola_input_textString,0,
			isola_input_textCharLength*sizeof(char)*textCharSize);
	{unsigned int i;
	for(i = 0;i<isola_input_textCharLength;i++){
		strcat(isola_input_textString,isola_input_textChars[i]);
	}}
}


void isola_input_textEditStart(unsigned int textLength, char(* textLoad)[32]){

	if (isola_input_textEditing) {
		return;
	}
	if (!textLength) {
		SDL_Log("%s : text length must not be zero",__FUNCTION__);
		return;
	}
	if (maxTextStringLength && textLength > maxTextStringLength) {
		SDL_Log("%s : text length cannot exceed %d",
				__FUNCTION__,maxTextStringLength);
		return;
	}


	isola_input_textCharLength = textLength;
	isola_input_textChars = calloc(sizeof(char)*textCharSize,
			isola_input_textCharLength);
	isola_input_textString = calloc(sizeof(char),
			textCharSize*isola_input_textCharLength);
	isola_input_textCursor = 0;


	if (textLoad) {
		{unsigned int i;
		for(i = 0;i<isola_input_textCharLength;i++){
			{unsigned int j;
			for(j = 0;j<textCharSize;j++){
				isola_input_textChars[i][j] = textLoad[i][j];
			}}
		}}
	}


	SDL_StartTextInput();
	isola_input_textEditing = 1;
}


void isola_input_textEditPush(char(* textChar)[32]){

	if (!isola_input_textEditing) {
		return;
	}
	if (isola_input_textCursor == isola_input_textCharLength) {
		return;
	}

	{unsigned int i;
	for(i = 0;i<textCharSize;i++){
		isola_input_textChars[isola_input_textCursor][i] = textChar[0][i];
	}}
	isola_input_textCursor++;

	isola_input_buildTextString();
}


void isola_input_textEditPop(void){

	if (!isola_input_textEditing) {
		return;
	}
	if (!isola_input_textCursor) {
		return;
	}

	isola_input_textCursor--;
	{unsigned int i;
	for(i = 0;i<textCharSize;i++){
		isola_input_textChars[isola_input_textCursor][i] = 0;
	}}

	isola_input_buildTextString();
}


void isola_input_textEditStop(void){

	if (isola_input_textEditing) {
		free(isola_input_textChars);
		free(isola_input_textString);
	}else {
		return;
	}

	SDL_StopTextInput();
	isola_input_textEditing = 0;
}




