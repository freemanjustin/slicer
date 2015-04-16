#version 150

in vec3 firstColor;
out vec4 finalColor;

void main() {
    
    //finalColor = vec4(0.2f, 0.88f, 0.45f, 1.0f);
    finalColor = vec4(firstColor,1.0);
}
