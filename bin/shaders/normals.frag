#version 330 core
out vec4 color;

in Data
{
    vec4 color;
} gdata;


void main()
{
    color = gdata.color;//vec4(1.0f, 1.0f, 0.0f, 1.0f);
}