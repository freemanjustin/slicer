#version 150

in vec3 v3Position;
in vec2 v2TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 fragTexCoord;

void main() {
    // Pass the tex coord straight through to the fragment shader
    fragTexCoord = v2TexCoord;
    
    gl_Position = projection * view * model * vec4( v3Position, 1.0 );
}
