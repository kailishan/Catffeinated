#version 330 core
out vec4 color;

in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;

uniform vec3 campos;
uniform sampler2D tex;
uniform vec4 objColor;
uniform vec3 lightPos1;
uniform vec3 lightPos2;
uniform vec3 lightPos3;
uniform vec3 lightPos4;
uniform vec3 lightPos5;
uniform vec3 lightPos6;
uniform vec3 lightPos7;
uniform vec3 lightPos8;
uniform vec3 lightDir;

void main()
{
	vec3 lights[8] = {lightPos1, lightPos2, lightPos3, lightPos4, lightPos5, lightPos6, lightPos7, lightPos8};

	// Load Texture
	// If there is no texture, read objColor
	color.a=1;
	color.rgb = texture(tex, vec2(vertex_tex.x, -vertex_tex.y)).rgb;
	if (color.rgb == vec3(0, 0, 0))
		color.rgb = objColor.rgb;

	float lightRad = cos(radians(30.0f));
	float theta;

	
	vec3 nn;
	vec3 light_pos;
	vec3 light_dir;
	vec3 eye_dir;
	vec3 reflect_dir;
	
	float spec;
	float diffuse;
	float intensity;

	// Toon Shading
	for (int i = 0; i < 8; i++)
	{
		nn = normalize(vertex_normal);
		light_pos = lights[i];
		light_dir = normalize(vertex_pos - light_pos);
		theta = dot(light_dir, normalize(lightDir));
		
		if (theta > lightRad)
		{
			eye_dir = normalize(-vertex_pos);
			reflect_dir = normalize(reflect(light_dir, nn));
	
			spec = max(dot(reflect_dir, eye_dir), 0.0);
			diffuse = max(dot(-light_dir, nn), 0.0);

			intensity += (0.6 * diffuse + 0.4 * spec);
		}
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
