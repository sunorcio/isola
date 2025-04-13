#ifndef MISC_H
#define MISC_H




#define ISOLA_LIT_STR_(x) #x
#define ISOLA_DEF_STR_(X) ISOLA_LIT_STR_(X)


#define ISOLA_CONCAT_STR_(x,y) x##y
#define ISOLA_CONCAT_DEF_(X,Y) ISOLA_CONCAT_STR_(X,Y)


#define ISOLA_ARRAYCOUNT_(x) (sizeof(x)/sizeof(x[0]))


/* linux only */
#ifdef SDL_h_
 #define ISOLA_CLEARTERMINAL_() SDL_Log("\033[2J");;
#else
 #define ISOLA_CLEARTERMINAL_() printf("\033[2J");;
#endif
	



#endif
