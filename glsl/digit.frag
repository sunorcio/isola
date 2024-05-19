#version 130




out vec4 fragColor;


uniform vec3 digitColor;




void main(){

	fragColor = vec4(digitColor, 0.5);
}




