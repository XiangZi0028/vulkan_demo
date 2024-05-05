#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
void main()
{
    gl_Position = vec4(position.xy ,0.5,1.0);
   // fragColor = color;
}