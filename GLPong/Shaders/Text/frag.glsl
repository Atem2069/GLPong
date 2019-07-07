R"(

#version 460 core

in vec2 texCoords;
layout(location=1)uniform vec3 colour;
layout(binding=0)uniform sampler2D text;

out vec4 fragColour;

void main()
{
	vec4 sampled = vec4(1.0,1.0,1.0,texture(text,texCoords).r);
	fragColour = vec4(colour,1.0) * sampled;
}

)"