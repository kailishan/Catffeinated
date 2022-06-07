#version 330 core

in vec2 texCoord;
out vec4 color;

//uniform sampler2D texBuf;
uniform sampler2D gBuf;
uniform sampler2D norBuf;
uniform sampler2D colorBuf;

//ignored for now
uniform vec3 Ldir;

/* just pass through the texture color we will add to this next lab */
void main()
{
   vec3 tNormal = texture( norBuf, texCoord ).rgb;
   vec3 tColor = texture( colorBuf, texCoord ).rgb;
   vec3 lightD = vec3(1, 1, 0);
   //color = vec4(tColor, 1.0);
   float dC = max(dot(normalize(tNormal), normalize(lightD)), 0);
   vec3 amb = 0.1 * tColor;
   color = vec4(tColor * dC + amb, 1.0);
   
   //if (abs(tColor.r) > 0.01 || abs(tColor.g) > 0.01)
   //	color = vec4(0.9, 0.9, 0.9, 1.0);
}
