#version 410 core
layout(location = 0) in vec2 vertPos;
layout(location = 1) in vec2 vertTex;

out vec2 vertex_tex;

void main()
{
	gl_Position = vec4(vertPos, 0.0, 1.0);
	vertex_tex = vertPos;	
}
