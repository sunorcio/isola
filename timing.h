#ifndef TIMING_H
#define TIMING_H




#include <SDL3/SDL.h>




extern Uint64 isola_clockFreq;




struct ISOLA_timer{
	Uint64 sps;
	Uint64 currentStep;
	Uint64 lastStep;
};


extern void isola_timerSetup(struct ISOLA_timer* timer,
		Uint64 stepsPerSecond);


extern unsigned char isola_timerStep(struct ISOLA_timer* timer);




struct ISOLA_counter{
	Uint64 sps;
	Uint64 currentStep;
	Uint64 lastStep;
	Uint64 stepDelay[256];
	unsigned char stepIndex;
};


extern void isola_counterSetup(struct ISOLA_counter* counter, 
		Uint64 stepsPerSecond);


extern unsigned char isola_counterStep(struct ISOLA_counter* timer);




#endif
