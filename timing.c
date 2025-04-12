#include "timing.h"




#include <SDL2/SDL.h>




unsigned long isola_clockFreq = 0;




void isola_timerSetup(struct ISOLA_timer* timer,unsigned long stepsPerSecond){

	isola_clockFreq = SDL_GetPerformanceFrequency();


	timer->sps = stepsPerSecond;
	timer->lastStep = SDL_GetPerformanceCounter();
}


void isola_counterSetup(struct ISOLA_counter* counter,
		unsigned long stepsPerSecond){

	isola_clockFreq = SDL_GetPerformanceFrequency();


	counter->sps = stepsPerSecond;

	{unsigned int i;
	for(i = 0;i<256;i++){
		counter->stepDelay[i] = isola_clockFreq/counter->sps;
	}}

	counter->lastStep = SDL_GetPerformanceCounter();
}




unsigned char isola_timerStep(struct ISOLA_timer* timer){

	timer->currentStep = SDL_GetPerformanceCounter();
	if(timer->currentStep >= timer->lastStep + isola_clockFreq/timer->sps){

		timer->lastStep = timer->currentStep;
		return 1;
	}

	return 0;
}


unsigned char isola_counterStep(struct ISOLA_counter* counter){

	counter->currentStep = SDL_GetPerformanceCounter();
	if(counter->currentStep >= counter->lastStep
			+ isola_clockFreq/counter->sps){
		counter->stepDelay[counter->stepIndex] = counter->currentStep 
			- counter->lastStep;
		counter->stepIndex++;

		counter->lastStep = counter->currentStep;
		return 1;
	}

	return 0;
}




