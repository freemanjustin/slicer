#version 150

in vec3 v3Position;
in vec2 v2TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 fragTexCoord;

void main(){
	
	//vec3 normal, lightDir;
	//vec4 diffuse, ambient, globalAmbient;
	//float NdotL;
	
	// Copy the stanard OpenGL texture coordiante to the output.
	//TexCoord0 = gl_MultiTexCoord0.xy;
	
	//normal = normalize(gl_NormalMatrix * gl_Normal);
	//lightDir = normalize(vec3(gl_LightSource[0].position));
	//NdotL = max(dot(normal, lightDir), 0.0);
	//diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
		
	//// Compute the ambient and globalAmbient terms
	//ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	//globalAmbient = gl_LightModel.ambient * gl_FrontMaterial.ambient;
		
	//gl_FrontColor =  NdotL * diffuse + globalAmbient + ambient;
	
	//gl_Position = ftransform();
    
    fragTexCoord = v2TexCoord;
    gl_Position = projection * view * model * vec4( v3Position, 1.0 );
}
