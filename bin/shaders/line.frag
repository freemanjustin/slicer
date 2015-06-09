#version 150

in vec3 firstColor;
in vec4 position;

out vec4 finalColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main() {
    
    finalColor = vec4(firstColor,1.0);
    
}

