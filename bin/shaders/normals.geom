#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

out Data
{
    vec4 color;
} gdata;


const float MAGNITUDE = 0.01f;
const vec4 green = vec4(0.0f, 1.0f, 0.0f, 1.0f);
const vec4 blue = vec4(0.0f, 0.0f, 1.0f, 1.0f);

const vec4 white = vec4(1.0f, 1.0f, 1.0f, 1.0f);
const vec4 black = vec4(0.0f, 0.0f, 0.0f, 1.0f);

void GenerateLine(int index)
{
    
    
    gl_Position = gl_in[index].gl_Position;
    //gdata.color = green;
    gdata.color = black;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0f) * MAGNITUDE;
    //gdata.color = blue;
    gdata.color = white;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // First vertex normal
    GenerateLine(1); // Second vertex normal
    GenerateLine(2); // Third vertex normal
}