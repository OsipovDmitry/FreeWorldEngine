#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable

in vec2 tc;

layout(location = 1) uniform sampler2D tex;
layout(location = 2) uniform float alpha = 0.5;

layout(location = 0) out vec4 outColor;

void main(void)
{
	outColor = texture(tex, tc);
	outColor.a = outColor.a * alpha;
}