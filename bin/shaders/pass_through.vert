#version 150

in vec3 v3Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    
    gl_Position = projection * view * model * vec4( v3Position, 1.0 );
    
}
