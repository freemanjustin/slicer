#version 150
 
//uniform mat4 viewMatrix, projMatrix;

uniform mat4 mvp;
in vec3 position;
 
void main()
{
    //gl_Position = projMatrix * viewMatrix * position ;
    gl_Position = mvp * vec4(position,1.0f);
}
