#version 420 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormalVector;

uniform mat4 modelMatrix;
uniform mat4 projectionViewMatrix;

out vec3 pos;
out vec3 normalVector;

void main()
{
	pos = (modelMatrix * vec4(inPos, 1)).xyz;
	gl_Position = projectionViewMatrix * vec4(pos, 1);
	normalVector = normalize(modelMatrix * vec4(inNormalVector, 0)).xyz;
}
