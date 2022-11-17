#version 330

in vec4 vPosition;
in vec2 texCoord;
in vec3 vNormal;

out vec2 tex;

out vec4 pos;
out vec3 norm;

uniform mat4 uModelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 modelMatrix;

uniform vec3 lPos[4];
uniform int l1On[4];
uniform vec3 spotDir[2];

void main (void) 
{
  gl_Position = uModelViewMatrix * vPosition;
  tex = texCoord;

  pos = vPosition;
  norm = vNormal;
}
