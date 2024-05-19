#version 130




uniform mat4 matProj;




void main(){

	float scale = 0.0625;


	gl_Position = matProj * vec4( position,-1.,1.);
}




