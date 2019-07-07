R"(#version 460 core

layout(location=0)in vec2 position;

layout(location=0)uniform mat4 ortho; 
layout(location=1)uniform mat4 model;

void main()
{
	gl_Position = ortho * model * vec4(position,0.0f,1.0f);
}
)"