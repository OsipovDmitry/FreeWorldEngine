#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec3 pos;

uniform mat4 modelViewProjMatrix;

void main(void)
{
	gl_Position = modelViewProjMatrix * vec4(pos, 1.0);
}