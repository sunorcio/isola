#ifndef MUTIL_H
#define MUTIL_H




void mut_projGLortho(float left, float right,
		float bottom, float top, float nearZ, float farZ, float dest[4*4]);


void mut_printM4(float m[4*4]);


void mut_printM3(float m[3*3]);


void mut_M3cp(float src[3*3],float dest[3*3]);


void mut_M3cpM4(float src[3*3],float dest[4*4]);


void mut_M3mul(float a[3*3],float b[3*3],float dest[3*3]);


unsigned int mut_gcd(int a, int b);




#endif
