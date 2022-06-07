#version 330 core
//layout(location = 0) in vec4 vertPos;
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;

out vec3 fragNor;
out vec3 fragPos;

out vec3 vertex_normal;
out vec3 vertex_pos;
out vec2 vertex_tex;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;


void main()
{
	vertex_normal = vec4(M * vec4(vertNor,0.0)).xyz;
	//vec4 tpos =  M * vec4(vec3(vertPos), 1.0);
	//vec4 tpos =  M * vertPos;
	vec4 tpos =  M * vec4(vertPos, 1.0);
	vertex_pos = tpos.xyz;
	gl_Position = P * V * tpos;
	vertex_tex = vertTex;


	fragNor = (V * M * vec4(vertNor, 0.0)).xyz;
	//fragNor = vertex_normal;
	//fragNor = vertNor;

	//fragPos = vec3(V * tpos);
	fragPos = vec3(V * M * vec4(vertPos, 1.0));
	//fragPos = vec3(V * M * vertPos);
	//fragPos = vertex_pos;
}
