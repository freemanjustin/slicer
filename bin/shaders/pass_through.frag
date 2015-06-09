#version 150

in vec3 firstColor;
in vec3 varyingNormalDirection;
in vec4 position;

out vec4 finalColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 v_inv;
uniform vec3 v3LightPos;


struct lightSource
{
    vec4 position;
    vec4 diffuse;
    vec4 specular;
    float constantAttenuation, linearAttenuation, quadraticAttenuation;
    float spotCutoff, spotExponent;
    vec3 spotDirection;
};
const int numberOfLights = 1;

lightSource lights[numberOfLights];


 lightSource light0 = lightSource(
 vec4(v3LightPos,1.0),
 vec4(1.0,  1.0,  1.0, 1.0),
 vec4(1.0,  1.0,  1.0, 1.0),
 0.0, 1.0, 0.0,
 180.0, 0.0,
 vec3(0.0, 0.0, 0.0)
 );

vec4 scene_ambient = vec4(0.2, 0.2, 0.2, 1.0);

struct material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

material frontMaterial = material(
                                  vec4(0.2, 0.2, 0.2, 1.0),
                                  vec4(1.0, 1.0, 1.0, 1.0),
                                  vec4(1.0, 1.0, 1.0, 1.0),
                                  50.0
                                  );



vec3 getJetColor(vec3 value) {
    
    if( (value.r == 0.0) || (value.r == 1.0)){
        return value;
    }
    else{
        float fourValue = 4.0 * value.r;
        float red   = min(fourValue - 1.5, -fourValue + 4.5);
        float green = min(fourValue - 0.5, -fourValue + 3.5);
        float blue  = min(fourValue + 0.5, -fourValue + 2.5);
        
        return clamp( vec3(red, green, blue), 0.0, 1.0 );
    }
}

vec3 getJetColorII(vec3 value) {
    
    if( (value.r == 0.0) || (value.r == 1.0)){
        return value;
    }
    else{
        float fourValue = 4.0 * value.r;
        float red   = min(fourValue - 2.5, -fourValue + 3.5);
        float green = min(fourValue - 1.5, -fourValue + 2.5);
        float blue  = min(fourValue + 1.5, -fourValue + 1.5);
        
        return clamp( vec3(red, green, blue), 0.0, 1.0 );
    }
}

// parula
vec3 getJetColorIII(vec3 value) {
    
    //if( (value.r == 0.0) || (value.r == 1.0)){
    //    return value;
    //}
    //else{
        float red = 106.5*pow(value.r,6.0) - 315.71*pow(value.r,4.0)-169.81*pow(value.r,3.0) + 40.86*pow(value.r,2.0) - 4.485*value.r+0.2055;
        float green = 3.4634*pow(value.g,4.0) - 4.777*pow(value.g,3.0) + 0.5384*pow(value.g,2.0)+1.5892*value.g+0.1703;
        float blue = -5.7193*pow(value.b,4.0)+14.724*pow(value.b,3.0)-13.342*pow(value.b,2.0)+3.8436*value.b+0.5413;
        return clamp( vec3(red, green, blue), 0.0, 1.0 );
    //}
}



void main() {
    
    
    
    lights[0] = light0;
    //lights[1] = light1;
    
    // j addition
    //lights[0].position = vec4(lightPos, 1.0);
    
    
    //vec4 value = vec4(firstColor,1.0f);
    vec3 jet = getJetColorIII(firstColor);
    //vec3 jet = firstColor;
    
    
    
    vec3 normalDirection = normalize(varyingNormalDirection);
    vec3 viewDirection = normalize(vec3(v_inv * vec4(0.0, 0.0, 0.0, 1.0) - position));
    vec3 lightDirection;
    float attenuation;
    
    // initialize total lighting with ambient lighting
    vec3 totalLighting = vec3(scene_ambient) * vec3(frontMaterial.ambient);
    
    int index = 0;
    //for (int index = 0; index < numberOfLights; index++) // for all light sources
    //{
        
        if (0.0 == lights[index].position.w) // directional light?
        {
            attenuation = 1.0; // no attenuation
            lightDirection = normalize(vec3(lights[index].position));
        }
        else // point light or spotlight (or other kind of light)
        {
            vec3 positionToLightSource = vec3(lights[index].position - position);
            float distance = length(positionToLightSource);
            lightDirection = normalize(positionToLightSource);
            attenuation = 1.0 / (lights[index].constantAttenuation
                                 + lights[index].linearAttenuation * distance
                                 + lights[index].quadraticAttenuation * distance * distance);
            
            if (lights[index].spotCutoff <= 90.0) // spotlight?
            {
                float clampedCosine = max(0.0, dot(-lightDirection, normalize(lights[index].spotDirection)));
                if (clampedCosine < cos(radians(lights[index].spotCutoff))) // outside of spotlight cone?
                {
                    attenuation = 0.0;
                }
                else
                {
                    attenuation = attenuation * pow(clampedCosine, lights[index].spotExponent);
                }
            }
        }
        
        vec3 diffuseReflection = attenuation
        * vec3(lights[index].diffuse) * vec3(frontMaterial.diffuse)
        * max(0.0, dot(normalDirection, lightDirection));
        
        vec3 specularReflection;
        if (dot(normalDirection, lightDirection) < 0.0) // light source on the wrong side?
        {
            specularReflection = vec3(0.0, 0.0, 0.0); // no specular reflection
        }
        else // light source on the right side
        {
            specularReflection = attenuation * vec3(lights[index].specular) * vec3(frontMaterial.specular)
            * pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), frontMaterial.shininess);
        }
        //original
        //totalLighting = (totalLighting + diffuseReflection + specularReflection);
        
        totalLighting = (totalLighting + (jet*diffuseReflection) + specularReflection);
        
        //totalLighting = jet*specularReflection;
    //}
    
    finalColor = vec4(totalLighting,1.0);
    
    //finalColor = vec4(jet,1.0);
    
}

