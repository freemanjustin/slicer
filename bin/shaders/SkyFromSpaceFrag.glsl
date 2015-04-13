#version 150
//
// Atmospheric scattering fragment shader
//
// Author: Sean O'Neil
//
// Copyright (c) 2004 Sean O'Neil
//
// updated by freeman.justin@gmail.com

uniform vec3 v3LightPos;
uniform float g;
uniform float g2;


in vec3 v3Direction;


in vec3 v3Color;
in vec3 v3SecondaryColor;
out vec4 v4FragColor;


void main (void)
{
    
	float fCos = dot(v3LightPos, v3Direction) / length(v3Direction);
	float fRayleighPhase = 0.75 * (1.0 + fCos*fCos);
	float fMiePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos*fCos) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);
    //gl_FragColor = fRayleighPhase * gl_Color + fMiePhase * gl_SecondaryColor;
    v4FragColor = vec4(fRayleighPhase * v3Color + fMiePhase * v3SecondaryColor,1.0);
	//gl_FragColor.a = gl_FragColor.b;
}

