#version 330 core

out vec4 color;

in vec4 vertex_color;
in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;

uniform vec3 campos;
uniform sampler2D tex;

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
	//float spec = pow(clamp(dot(H, norm), 0, 1), 2);

	// Ambient + Diffuse
	color = 0.3 * vertex_color + diff * vertex_color;
	color = vertex_color;
	//color.rgb = vec3(diff);
	//color.rgb = (diff * vertex_color.rgb) + (spec * vec3(1, 1, 1));
	color.a = vertex_color.a;

	color.a=1;
	color.rgb = texture(tex, vertex_tex*2).rgb;
	if (color.rgb == vec3(0, 0, 0))
		color.rgb = vertex_color.rgb;

	vec3 nn = normalize(vertex_normal);
	vec3 light_pos = vec3(0.0, 10.0, 0.0);
	vec3 light_dir = normalize(vertex_pos - light_pos);
	vec3 eye_dir = normalize(-vertex_pos);
	vec3 reflect_dir = normalize(reflect(light_dir, nn));
	
	float spec = max(dot(reflect_dir, eye_dir), 0.0);
	float diffuse = max(dot(-light_dir, nn), 0.0);

	float intensity = 0.6 * diffuse + 0.4 * spec;

 	if (intensity > 0.9) {
 		intensity = 1.1;
 	}
 	else if (intensity > 0.5) {
 		intensity = 0.7;
 	}
 	else {
 		intensity = 0.5;
	}

	vec3 camDir = campos - vertex_pos;
	camDir = normalize(camDir);
	float outline = dot(camDir, nn);
	if (outline > -0.2 && outline < 0.2)
	{
		intensity = 0.0;
	}

	color.rgb = color.rgb * intensity;
}
