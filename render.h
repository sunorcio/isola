#ifndef RENDER_H
#define RENDER_H




#include "isola.h"




extern float view[4*4];
extern float projection[4*4];

extern void renderCreate(void);
extern void renderDestroy(void);
extern void renderDraw(void);




#endif
