// There are some bugs in the lighting calculation
// I am fixing those now, will take a bit of time

// Just to state:
// Gouraud shading here is being done in the
// WCS, the WCS does not involve anisotropic scaling
// so it won't be a problem

// Note that specular lighting has been left out for now

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

// vec4 ptLight(vec3 lightPos) {
  
//   // Transforming the positions to WCS
//   vec3 wcsPos = vec3(modelMatrix * vPosition);
//   vec3 wcsLightPos = lightPos;

//   // Defining Materials
//   vec4 diffuse = vec4(0.2, 0.2, 0.2, 1.0); 
//   vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
//   vec4 specular = vec4(1.0, 0.5, 0.5, 1.0);
//   float shininess = 0.05;
//   vec4 spec = vec4(0.0); 

//   float constant = 1.0;
//   float linear = 0.009;
//   float quadratic = 0.0032;

//   // Calculating the attenuation
//   float dist = length(wcsLightPos - wcsPos);
//   float attenuation = 1.0/(constant + linear * dist + quadratic * (dist * dist));

//   ambient *= attenuation;
//   diffuse *= attenuation;
//   specular *= attenuation;
  
//   // Defining Light 
//   // lightDir points towards the source from the vertex
//   vec3 lightDir = wcsLightPos - wcsPos;
  
//   vec3 n = normalize(normalMatrix * normalize(vNormal));
//   float dotProduct = dot(n, lightDir);
//   float intensity =  max(dotProduct, 0.0);

//   // Compute specular component only if light falls on vertex
//   // Specular being left for now
//   // if(intensity > 0.0)
//   // {
//   //   vec3 eye = normalize( vec3(-gl_Position));
//   //   vec3 h = normalize(lightDir + eye );
//   //   float intSpec = max(dot(h,n), 0.0);	
//   //   spec = specular * pow(intSpec, shininess);
//   // }

//   return max((intensity * diffuse  + spec)*vColor, ambient);
// }

// vec4 spotLight(vec3 lightPos, vec3 spotDir, float cutOff, float outCutOff) {
//   // Implementing spotlight in the WCS
//   // lightPos is the position of light in the WCS already
//   // spotDir is a vector pointing from the light towards its
//   // flashing direction

//   // Converting to WCS
//   // Transforming the positions to WCS
//   vec3 wcsPos = vec3(modelMatrix * vPosition);
//   vec3 wcsLightPos = lightPos;

//   // Defining Materials
//   vec4 diffuse = vec4(0.9, 0.9, 0.9, 1.0); 
//   vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
//   vec4 specular = vec4(1.0, 0.5, 0.5, 1.0);
//   float shininess = 0.05;
//   vec4 spec = vec4(0.0); 

//   // Defining Light 
//   // lightDir points towards the source from the vertex
//   vec3 lightDir = wcsLightPos - wcsPos;
//   lightDir = normalize(lightDir);

//   // Here, the variable theta is the cosine of the actual angle
//   float theta = dot(lightDir, normalize(-spotDir));

//   if(theta > cutOff) {
//     vec3 n = normalize(normalMatrix * normalize(vNormal));
//     float dotProduct = dot(n, lightDir);
//     float intensity = max(dotProduct, 0.0);

//     return max((intensity * diffuse + spec)*vColor, ambient);
//   }
//   else if (theta < outCutOff) {
//     vec3 n = normalize(normalMatrix * normalize(vNormal));
//     float dotProduct = dot(n, lightDir);
//     float intensity = max(dotProduct, 0.0);

//     float epsilon = cutOff - outCutOff;
//     float fac = (theta - outCutOff)/epsilon;

//     intensity = intensity * fac;

//     return max((intensity * diffuse + spec)*vColor, ambient);
//   }
//   else {
//     return ambient;
//   }
// }

void main (void) 
{
  gl_Position = uModelViewMatrix * vPosition;
  // color = vColor;
  // gl_Position = uModelViewMatrix * vPosition;
  // color = vec4(0.0);

  // for(int i = 0; i < 2; i++) {
  //   if(l1On[i] == 1) color += ptLight(lPos[i]);
  // }

  // // The last two arguments are the cosines of 25 and 50 degrees
  // if(l1On[2] == 1) color += spotLight(lPos[2], spotDir[0], 0.9063, 0.6428);
  // if(l1On[3] == 1) color += spotLight(lPos[3], spotDir[1], 0.9063, 0.8192);

  color = vColor;
  pos = vPosition;
  norm = vNormal;
}