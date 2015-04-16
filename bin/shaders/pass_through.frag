#version 150

in vec3 firstColor;
out vec4 finalColor;

vec4 getJetColor(vec4 value) {
    
    if( (value.r == 0.0) || (value.r == 1.0)){
        return value;
    }
    else{
        float fourValue = 4.0 * value.r;
        float red   = min(fourValue - 1.5, -fourValue + 4.5);
        float green = min(fourValue - 0.5, -fourValue + 3.5);
        float blue  = min(fourValue + 0.5, -fourValue + 2.5);
        
        return clamp( vec4(red, green, blue,value.a), 0.0, 1.0 );
    }
}


void main() {
    
    vec4 value = vec4(firstColor,1.0);
    vec4 jet = getJetColor(value);
    finalColor = jet;

    //finalColor = vec4(0.2f, 0.88f, 0.45f, 1.0f);
    //finalColor = vec4(firstColor,1.0);
}
