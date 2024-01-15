#ifndef RENDER_H
#define RENDER_H




#include "isola.h"
#include "mutil.h"




extern float cameradest[3];
extern void cameraUpdate(void);
extern void cameraZoom(short increment);

extern void renderCreate(void);
extern void renderDestroy(void);
extern void renderDraw(void);




#endif
