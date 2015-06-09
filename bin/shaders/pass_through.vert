#version 150

in vec4 v4Position;
in vec3 v3Color;
in vec3 v3Normal;


out vec3 firstColor;
out vec3 varyingNormalDirection;
out vec4 position;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 m_3x3_inv_transp;
uniform mat4 v_inv;
uniform vec3 v3LightPos;


void main() {
    
    position = view * model * v4Position;
    varyingNormalDirection = normalize(m_3x3_inv_transp * v3Normal);
    
    firstColor = v3Color;
    
    gl_Position = projection * view * model * v4Position;
    
}

