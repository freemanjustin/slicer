#version 150
//
// Atmospheric scattering fragment shader
//
// Author: Sean O'Neil
//
// Copyright (c) 2004 Sean O'Neil
//
// updated by freeman.justin@gmail.com

//uniform sampler2D s2Tex1;
//uniform sampler2D s2Tex2;

in vec3 v3Color;
in vec3 v3SecondaryColor;
out vec4 v4FragColor;

void main (void)
{
	//gl_FragColor = gl_Color  -  0.1 * gl_SecondaryColor;
    v4FragColor = vec4(v3Color - 0.1 * v3SecondaryColor,1.0);
    
    
	//gl_FragColor = gl_Color + texture2D(s2Tex1, gl_TexCoord[0].st) * texture2D(s2Tex2, gl_TexCoord[1].st) * gl_SecondaryColor;
	
	
	// j's single texture code
	//gl_FragColor = gl_Color + texture2D(s2Tex1, gl_TexCoord[0].st) * gl_SecondaryColor;
}
