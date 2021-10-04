#version 330 core
#pragma optionNV(fastmath on)
#pragma optionNV(fastprecision on)
#pragma optionNV(ifcvt none)
#pragma optionNV(inline all)
#pragma optionNV(strict on)
#pragma optionNV(unroll all)

const int c_snakeMode = 0;
const int c_foodMode = 1;
const int c_gridMode = 2;
/// @brief flag to indicate if model has unit normals if not normalize
uniform bool Normalize;
/// @brief the current fragment normal for the vert being processed
out vec3 fragmentNormal;
// the eye position of the camera
uniform vec3 viewerPos;

uniform int meshType;
/// @brief the vertex passed in
layout (location =0)in vec3 inVert;
/// @brief the normal passed in
layout (location =2)in vec3 inNormal;
/// @brief the in uv
layout (location =1)in vec2 inUV;

uniform samplerBuffer TBO;

out vec3 eyeDirection;
struct Materials
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};


struct Lights
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float constantAttenuation;
	float spotCosCutoff;
	float quadraticAttenuation;
	float linearAttenuation;
};
// our material
uniform Materials material;
#define numLights 8
// array of lights
uniform Lights light[numLights];
// direction of the lights used for shading
out vec3 lightDir[numLights];
// direction of the lights used for shading
out vec3 halfVector[numLights];

uniform mat4 M;
uniform mat4 foodPosM;
uniform mat4 gridPosM;
uniform mat4 MV;
uniform mat4 MVP;
uniform mat3 normalMatrix;

void main()
{
    mat4 tx;
    if(meshType == c_snakeMode)
    {
        tx =mat4(texelFetch(TBO,gl_InstanceID*4+0),
                     texelFetch(TBO,gl_InstanceID*4+1),
                     texelFetch(TBO,gl_InstanceID*4+2),
                     texelFetch(TBO,gl_InstanceID*4+3));
    }
    else if(meshType == c_foodMode)
    {
        tx = foodPosM;
    }
    else if(meshType == c_gridMode)
    {
        tx = gridPosM;
    }
  //calculate the fragments surface normal
 fragmentNormal = (normalMatrix*inNormal);

 if (Normalize == true)
 {
	fragmentNormal = normalize(fragmentNormal);
 }

 // calculate the vertex position
 gl_Position = MVP*tx*vec4(inVert,1.0);
 // Transform the vertex to eye co-ordinates for frag shader
 /// @brief the vertex in eye co-ordinates  homogeneous
vec4 eyeCord=MV*tx*vec4(inVert,1);

vec4 worldPosition = M * tx*vec4(inVert, 1.0);
eyeDirection = normalize(viewerPos - worldPosition.xyz);
float dist;

 for(int i=0; i<numLights; ++i)
 {
	 lightDir[i]=vec3(light[i].position.xyz-eyeCord.xyz);
	 dist = length(lightDir[i]);
	 lightDir[i]/= dist;
	 halfVector[i] = normalize(eyeDirection + lightDir[i]);
 }

}
