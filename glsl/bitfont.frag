#version 130




in vec2 fragTexCoord


out vec4 fragColor;


uniform sampler2D bitfontAtlas;
uniform vec3 fontColor;
uniform vec3 backColor;




void main(){



	fragColor = vec4(fontColor, 1.-sample(bitfontAtlas).x);
}




