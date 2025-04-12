#ifndef TIMING_H
#define TIMING_H




extern unsigned long isola_clockFreq;




struct ISOLA_timer{
	unsigned long sps;
	unsigned long currentStep;
	unsigned long lastStep;
};


extern void isola_timerSetup(struct ISOLA_timer* timer,
		unsigned long stepsPerSecond);


extern unsigned char isola_timerStep(struct ISOLA_timer* timer);




struct ISOLA_counter{
	unsigned long sps;
	unsigned long currentStep;
	unsigned long lastStep;
	unsigned long stepDelay[256];
	unsigned char stepIndex;
};


extern void isola_counterSetup(struct ISOLA_counter* counter, 
		unsigned long stepsPerSecond);


extern unsigned char isola_counterStep(struct ISOLA_counter* timer);




#endif
