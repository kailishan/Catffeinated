#version 330 core

out vec4 color;

in vec4 vertex_color;
in vec3 vertex_normal;
in vec3 vertex_pos;

uniform vec3 campos;

void main()
{
	// Diffuse
	vec3 norm = normalize(vertex_normal);
	vec3 Lp = vec3(0, 0, 100);
	vec3 Ld = normalize(Lp - vertex_pos);
	float diff = clamp(dot(norm, Ld), 0, 1);

	// Specular
	vec3 Cd = normalize(campos - vertex_pos);
	vec3 H = normalize(Cd + Ld);
	float spec = pow(clamp(dot(H, norm), 0, 1), 2);

	// Ambient + Diffuse
	color = 0.4 * vertex_color + diff * vertex_color;
	//color.rgb = (diff * vertex_color.rgb) + (spec * vec3(1, 1, 1));
	color.a = vertex_color.a;
}
