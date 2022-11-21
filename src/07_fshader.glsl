#version 130

in vec4 pos;
in vec3 norm;
in vec4 color;
out vec4 frag_color;

uniform mat4 uModelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 modelMatrix;

// Note that this position of light is in the
// WCS
uniform vec3 lPos[4];
uniform int l1On[4];
uniform vec3 spotDir[2];

vec4 ptLight(vec3 lightPos) {
  
  // Transforming the positions to WCS
  vec3 wcsPos = vec3(modelMatrix * pos);
  vec3 wcsLightPos = lightPos;

  // Defining Materials
  vec4 diffuse = vec4(0.2, 0.2, 0.2, 1.0); 
  vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
  vec4 specular = vec4(1.0, 0.5, 0.5, 1.0);
  float shininess = 0.05;
  vec4 spec = vec4(0.0); 

  float constant = 1.0;
  float linear = 0.009;
  float quadratic = 0.0032;

  // Calculating the attenuation
  float dist = length(wcsLightPos - wcsPos);
  float attenuation = 1.0/(constant + linear * dist + quadratic * (dist * dist));

  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;
  
  // Defining Light 
  // lightDir points towards the source from the vertex
  vec3 lightDir = wcsLightPos - wcsPos;
  
  vec3 n = normalize(normalMatrix * normalize(norm));
  float dotProduct = dot(n, lightDir);
  float intensity =  max(dotProduct, 0.0);

  return max((intensity * diffuse  + spec)*color, ambient);
}

vec4 spotLight(vec3 lightPos, vec3 spotDir, float cutOff, float outCutOff) {
  // Implementing spotlight in the WCS
  // lightPos is the position of light in the WCS already
  // spotDir is a vector pointing from the light towards its
  // flashing direction

  // Converting to WCS
  // Transforming the positions to WCS
  vec3 wcsPos = vec3(modelMatrix * pos);
  vec3 wcsLightPos = lightPos;

  // Defining Materials
  vec4 diffuse = vec4(0.9, 0.9, 0.9, 1.0); 
  vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
  vec4 specular = vec4(1.0, 0.5, 0.5, 1.0);
  float shininess = 0.05;
  vec4 spec = vec4(0.0); 

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

    return max((intensity * diffuse + spec)*color, ambient);
  }
  else if (theta > outCutOff) {
    vec3 n = normalize(normalMatrix * normalize(norm));
    float dotProduct = dot(n, lightDir);
    float intensity = max(dotProduct, 0.0);

    float epsilon = cutOff - outCutOff;
    float fac = (theta - outCutOff)/epsilon;

    intensity = intensity * fac;

    return max((intensity * diffuse + spec)*color, ambient);
  }
  else {
    return ambient;
  }
}

void main () 
{
  frag_color = vec4(0.0);

  for(int i = 0; i < 2; i++) {
    if(l1On[i] == 1) frag_color += ptLight(lPos[i]);
  }

  // The last two arguments are the cosines of 25 and 50 degrees
  if(l1On[2] == 1) frag_color += spotLight(lPos[2], spotDir[0], 0.9763, 0.9063);
  if(l1On[3] == 1) frag_color += spotLight(lPos[3], spotDir[1], 0.9063, 0.8192);
}