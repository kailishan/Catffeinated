#version 330 core

out vec4 color;

in vec4 vertex_color;
in vec3 vertex_normal;
in vec3 vertex_pos;
in float intensity;

uniform vec3 campos;

void main()
{
	// Diffuse
	vec3 norm = normalize(vertex_normal);
	vec3 Lp = vec3(20, 20, 20);
	vec3 Ld = normalize(Lp - vertex_pos);
	float diff = clamp(dot(norm, Ld), 0, 1);

	// Specular
	vec3 Cd = normalize(campos - vertex_pos);
	vec3 H = normalize(Cd + Ld);
	float spec = pow(clamp(dot(H, norm), 0, 1), 2);

	// Ambient + Diffuse
	color = 0.3 * vertex_color + diff * vertex_color;
	color = vertex_color;
	//color.rgb = vec3(diff);
	//color.rgb = (diff * vertex_color.rgb) + (spec * vec3(1, 1, 1));
	color.a = vertex_color.a;

	if (intensity > 0.95)
		color += vec4(0.25,0.25,0.25,0.0);
	else if (intensity > 0.5)
		color += vec4(0.0,0.0,0.0,0.0);
	else if (intensity > 0.25)
		color -= vec4(0.25,0.25,0.25,0.0);
	else
		color -= vec4(0.5,0.5,0.5,0.0);
}
