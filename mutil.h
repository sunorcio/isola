#ifndef MUTIL_H
#define MUTIL_H




void isola_mut_glproj_ortho(float left, float right,
		float bottom, float top, float nearZ, float farZ, float dest[4*4]);


void isola_mut_print_m4(float m[4*4]);


void isola_mut_print_m3(float m[3*3]);


void isola_mut_cp_m3(float src[3*3],float dest[3*3]);


void isola_mut_cp_m3m4(float src[3*3],float dest[4*4]);


void isola_mut_mul_m3(float a[3*3],float b[3*3],float dest[3*3]);


unsigned int isola_mut_gcd(int a, int b);




#endif
