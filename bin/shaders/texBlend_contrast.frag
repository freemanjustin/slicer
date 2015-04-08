#version 150


uniform sampler2D Texture0;
uniform sampler2D Texture1;

in vec2 fragTexCoord;

out vec4 finalColor;


// Brightness
//uniform float brightness;

// Contrast
//uniform float contrast;

void main()
{  
	vec4 texel0, texel1, resultColor;

	texel0 = texture(Texture0, fragTexCoord);
	texel1 = texture(Texture1, fragTexCoord);
	
    // blend the two texture maps
    finalColor = mix(texel0, texel1, (1.0-texel0.a));
		
	//resultColor += brightness;
	//resultColor = (resultColor-0.5)*contrast + 0.5;
		
	//finalColor = gl_Color * resultColor;
	
}

