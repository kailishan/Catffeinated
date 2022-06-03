#version 330 core

out vec4 color;

in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;

uniform vec4 objColor;
uniform vec3 campos;
uniform sampler2D tex;

void main()
{
	color.a=1;
	color.rgb = texture(tex, vertex_tex*2).rgb;
	if (color.rgb == vec3(0, 0, 0))
		color.rgb = objColor.rgb;

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
	if (outline > -0.1 && outline < 0.1)
	{
		intensity = 0.0;
	}

	color.rgb = color.rgb * intensity;
}
