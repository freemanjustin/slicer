#version 150

in vec3 v3Position;
in vec3 v3Color;


out vec3 firstColor;
out vec4 position;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



void main() {
    
    position = view * model * vec4(v3Position, 1.0);
    
    firstColor = v3Color;
    
    gl_Position = projection * view * model * vec4(v3Position, 1.0);
    
}

