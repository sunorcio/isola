#ifndef MISC_H
#define MISC_H




#define isolaLIT_TO_STR_(x) #x
#define isolaDEF_TO_STR_(X) isolaLIT_TO_STR_(X)


#define isolaCONCAT_STR_(x,y) x##y
#define isolaCONCAT_DEF_(X,Y) isolaCONCAT_STR_(X,Y)


#define isolaARRAY_COUNT_(x) (sizeof(x)/sizeof(x[0]))


/* linux only */
#define isolaCLEARTERMINAL_() SDL_Log("\033[2J");;
	



#endif
