#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable

uniform vec4 color = vec4(1.0, 0.0, 0.0, 1.0);

layout(location = 0) out vec4 outColor;

void main(void)
{
	outColor = color;
}