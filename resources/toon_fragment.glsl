#version 330 core
out vec4 color;

in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;

uniform vec3 campos;
uniform vec3 lightPosList[7];
uniform sampler2D tex;
uniform vec4 objColor;
uniform vec3 lightPos;

void main()
{
	// Load Texture
	// If there is no texture, read objColor
	color.a=1;
	color.rgb = texture(tex, vec2(vertex_tex.x, -vertex_tex.y)).rgb;
	if (color.rgb == vec3(0, 0, 0))
		color.rgb = objColor.rgb;

	
	vec3 nn;
	vec3 light_pos;
	vec3 light_dir;
	vec3 eye_dir;
	vec3 reflect_dir;
	
	float spec;
	float diffuse;
	float intensity;

	// Toon Shading
	for (int i = 0; i < 7; i++)
	{
		nn = normalize(vertex_normal);
		light_pos = lightPosList[i];
		light_dir = normalize(vertex_pos - light_pos);
		eye_dir = normalize(-vertex_pos);
		reflect_dir = normalize(reflect(light_dir, nn));
	
		spec = max(dot(reflect_dir, eye_dir), 0.0);
		diffuse = max(dot(-light_dir, nn), 0.0);

		intensity += (0.6 * diffuse + 0.4 * spec)/7;
	}

	/*
	vec3 nn = normalize(vertex_normal);
	vec3 light_pos = lightPosList[0];
	vec3 light_dir = normalize(vertex_pos - light_pos);
	vec3 eye_dir = normalize(-vertex_pos);
	vec3 reflect_dir = normalize(reflect(light_dir, nn));
	
	float spec = max(dot(reflect_dir, eye_dir), 0.0);
	float diffuse = max(dot(-light_dir, nn), 0.0);

	float intensity = 0.6 * diffuse + 0.4 * spec;
	*/

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
	if (outline > -0.15 && outline < 0.15)
	{
		intensity = 0.0;
	}

	color.rgb = color.rgb * intensity;
}
