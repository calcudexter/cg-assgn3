#version 130

// This position is in the OCS, need to convert to WCS
in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;

out vec4 color;

out vec4 pos;
out vec3 norm;

uniform mat4 uModelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 modelMatrix;

// Note that this position of light is in the
// WCS
uniform vec3 lPos[4];
uniform int l1On[4];
uniform vec3 spotDir[2];

void main (void) 
{
  gl_Position = uModelViewMatrix * vPosition;

  color = vColor;
  pos = vPosition;
  norm = vNormal;
}