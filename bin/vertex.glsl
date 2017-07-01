#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec3 pos;
layout(location = 4) in vec2 texcoord;

layout(location = 0) uniform mat4 modelViewProjMatrix;

out vec2 tc;

void main(void)
{
	tc = texcoord;
	gl_Position = modelViewProjMatrix * vec4(pos, 1.0);
}