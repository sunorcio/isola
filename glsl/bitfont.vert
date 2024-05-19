#version 130




uniform mat4 matProj;


#define scale 24./768.




void main(){



	gl_Position = matProj * vec4( position*scale,-1.,1.);
}




