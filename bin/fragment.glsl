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

in vec2 tc;
in vec3 n;
in vec3 l[4];
in vec3 h[4];

uniform sampler2D tex;
uniform Light light[4];

layout(location = 0) out vec4 outColor;

void main(void)
{
    vec3 normal = normalize(n);

    vec3 toLight[4];
    toLight[0] = normalize(l[0]);
    toLight[1] = normalize(l[1]);
    toLight[2] = normalize(l[2]);
    toLight[3] = normalize(l[3]);

    float att[4];
    att[0] = distanceAttenuationLighting(light[0], length(l[0])) * spotAttenuationLighting(light[0], dot(-toLight[0], LIGHT_DIRECTION(light[0])));
    att[1] = distanceAttenuationLighting(light[1], length(l[1])) * spotAttenuationLighting(light[1], dot(-toLight[1], LIGHT_DIRECTION(light[1])));
    att[2] = distanceAttenuationLighting(light[2], length(l[2])) * spotAttenuationLighting(light[2], dot(-toLight[2], LIGHT_DIRECTION(light[2])));
    att[3] = distanceAttenuationLighting(light[3], length(l[3])) * spotAttenuationLighting(light[3], dot(-toLight[3], LIGHT_DIRECTION(light[3])));

    vec3 diff = vec3(0.0, 0.0, 0.0);
    diff += LIGHT_COLOR(light[0]) * diffuseLighting(att[0], normal, toLight[0]);
    diff += LIGHT_COLOR(light[1]) * diffuseLighting(att[1], normal, toLight[1]);
    diff += LIGHT_COLOR(light[2]) * diffuseLighting(att[2], normal, toLight[2]);
    diff += LIGHT_COLOR(light[3]) * diffuseLighting(att[3], normal, toLight[3]);

    float spec = 0.0;
    spec += specularLighting(att[0], normal, normalize(h[0]), 20.0);
    spec += specularLighting(att[1], normal, normalize(h[1]), 20.0);
    spec += specularLighting(att[2], normal, normalize(h[2]), 20.0);
    spec += specularLighting(att[3], normal, normalize(h[3]), 20.0);

	
    outColor.rgb = texture(tex, tc).rgb * diff + vec3(0.2) * spec;
    outColor.a = 1.0;
}
