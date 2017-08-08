#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable

// -------------LIGHTS----------------------

#define Light mat4

#define LIGHT_TYPE(light) (int(light[3][3] + 0.5))
#define LIGHT_POSITION(light) (light[0].xyz)
#define LIGHT_DIRECTION(light) (light[1].xyz)
#define LIGHT_ATTENUATION_INNER_RADIUS(light) (light[2].x)
#define LIGHT_ATTENUATION_OUTER_RADIUS(light) (light[2].y)
#define LIGHT_SPOT_COS_INNER_ANGLE(light) (light[2].z)
#define LIGHT_SPOT_COS_OUTER_ANGLE(light) (light[2].w)
#define LIGHT_COLOR(light) (light[3].xyz)

#define LIGHT_TYPE_NONE (0)
#define LIGHT_TYPE_DIRECTION (1)
#define LIGHT_TYPE_POINT (2)
#define LIGHT_TYPE_SPOT (3)
#define LIGHT_TYPE_COUNT (4)

const float g_hasDirectionLightType[LIGHT_TYPE_COUNT] = float[LIGHT_TYPE_COUNT](0.0, 1.0, 0.0, 0.0);
const float g_distanceAttenuationLightType[LIGHT_TYPE_COUNT] = float[LIGHT_TYPE_COUNT](0.0, 0.0, 1.0, 1.0);
const float g_spotAttenuationLightType[LIGHT_TYPE_COUNT] = float[LIGHT_TYPE_COUNT](0.0, 0.0, 0.0, 1.0);

vec3 toLightVector(in Light light, vec3 v) {
    return mix(LIGHT_POSITION(light) - v, -LIGHT_DIRECTION(light), g_hasDirectionLightType[LIGHT_TYPE(light)]);
}

float distanceAttenuationLighting(in Light light, in float distToLight) {
    float att = (distToLight - LIGHT_ATTENUATION_INNER_RADIUS(light)) / (LIGHT_ATTENUATION_OUTER_RADIUS(light) - LIGHT_ATTENUATION_INNER_RADIUS(light));
    att = clamp(att, 0.0, 1.0);
    att = att * att;
    att = att * g_distanceAttenuationLightType[LIGHT_TYPE(light)];
    
    return 1.0 - att;
}

float spotAttenuationLighting(in Light light, in float cosAngle) {
    float att = (cosAngle - LIGHT_SPOT_COS_OUTER_ANGLE(light)) / (LIGHT_SPOT_COS_INNER_ANGLE(light) - LIGHT_SPOT_COS_OUTER_ANGLE(light));
    att = 1.0 - clamp(att, 0.0, 1.0);
    att = att * att;
    att = att * g_spotAttenuationLightType[LIGHT_TYPE(light)];

    return 1.0 - att;
}

float diffuseLighting(in float att, in vec3 n, in vec3 l) {
    return att * max(0.0, dot(n, l));
}

float specularLighting(in float att, in vec3 n, in vec3 h, in float shininess) {
    return att * pow(max(0.0, dot(h, n)), shininess);
}

// -----------------------------------------

layout(location = 0) in vec3 pos;
layout(location = 3) in vec3 normal;
layout(location = 4) in vec2 texcoord;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform vec3 cameraPos;

uniform Light light[4];

out vec2 tc;
out vec3 n;
out vec3 l[4];
out vec3 h[4];

void main(void)
{
    tc = texcoord;
    n = normalize(normalMatrix * normal);

    vec4 v = modelMatrix * vec4(pos, 1.0);
    vec3 toCamera = normalize(cameraPos - v.xyz);

    l[0] = toLightVector(light[0], v.xyz);
    l[1] = toLightVector(light[1], v.xyz);
    l[2] = toLightVector(light[2], v.xyz);
    l[3] = toLightVector(light[3], v.xyz);
    
    h[0] = normalize(normalize(l[0]) + toCamera);
    h[1] = normalize(normalize(l[1]) + toCamera);
    h[2] = normalize(normalize(l[2]) + toCamera);
    h[3] = normalize(normalize(l[3]) + toCamera);
	
	gl_Position = viewProjectionMatrix * v;
}
