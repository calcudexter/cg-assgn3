#version 330

in vec4 pos;
in vec3 norm;

out vec4 frag_color;

uniform sampler2D texture;
uniform mat4 uModelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 modelMatrix;

uniform vec3 lPos[4];
uniform int l1On[4];
uniform vec3 spotDir[2];

in vec2 tex;

float ptLight(vec3 lightPos) {
  
  // Transforming the positions to WCS
  vec3 wcsPos = vec3(modelMatrix * pos);
  vec3 wcsLightPos = lightPos;

  float diffuse = 0.3;
  float ambient = 0.1;
  float specular = 0.5;
  float spec = 0.0;

  float constant = 1.0;
  float linear = 0.0009;
  float quadratic = 0.00032;

  // Calculating the attenuation
  float dist = length(wcsLightPos - wcsPos);
  float attenuation = 1.0/(constant + linear * dist + quadratic * (dist * dist));

  // Defining Light 
  // lightDir points towards the source from the vertex
  vec3 lightDir = wcsLightPos - wcsPos;
  lightDir = normalize(lightDir);

  vec3 n = normalize(normalMatrix * normalize(norm));
  float dotProduct = dot(n, lightDir);
  float intensity =  max(dotProduct, 0.0);

  return max((intensity*(diffuse + spec)), ambient);
}

float spotLight(vec3 lightPos, vec3 spotDir, float cutOff, float outCutOff) {
  // Implementing spotlight in the WCS
  // lightPos is the position of light in the WCS already
  // spotDir is a vector pointing from the light towards its
  // flashing direction

  // Converting to WCS
  // Transforming the positions to WCS
  vec3 wcsPos = vec3(modelMatrix * pos);
  vec3 wcsLightPos = lightPos;

  float diffuse = 0.5;
  float ambient = 0.1;
  float specular = 0.5;
  float spec = 0.0;

  // Defining Light 
  // lightDir points towards the source from the vertex
  vec3 lightDir = wcsLightPos - wcsPos;
  lightDir = normalize(lightDir);

  // Here, the variable theta is the cosine of the actual angle
  float theta = dot(lightDir, normalize(-spotDir));

  if(theta > cutOff) {
    vec3 n = normalize(normalMatrix * normalize(norm));
    float dotProduct = dot(n, lightDir);
    float intensity = max(dotProduct, 0.0);

    return max((intensity*(diffuse + spec)), ambient);
  }
  else if (theta > outCutOff) {
    vec3 n = normalize(normalMatrix * normalize(norm));
    float dotProduct = dot(n, lightDir);
    float intensity = max(dotProduct, 0.0);

    float epsilon = cutOff - outCutOff;
    float fac = (theta - outCutOff)/epsilon;

    intensity *= fac;

    return max((intensity*(diffuse + spec)), ambient);
  }
  else {
    return ambient;
  }
}

void main ()
{
  frag_color = vec4(0.0);
  float fac = 0.0;

  for(int i = 0; i < 2; i++) {
    if(l1On[i] == 1) fac += ptLight(lPos[i]);
  }

  if(l1On[2] == 1) fac += spotLight(lPos[2], spotDir[0], 0.9763, 0.9063);
  if(l1On[3] == 1) fac += spotLight(lPos[3], spotDir[1], 0.9063, 0.6428);

  fac = clamp(fac, 0.0, 1.0);
  
  frag_color = fac * texture2D(texture, tex);
}
