#version 330 core
out vec4 color;
in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;

uniform vec3 campos;
uniform sampler2D tex;
uniform vec3 objColor;
uniform vec3 lightPos;

void main()
{
	/*
	vec3 n = normalize(vertex_normal);
	vec3 lp=vec3(10,20,-100);
	vec3 ld = normalize(vertex_pos - lp);
	float diffuse = dot(n,ld);

	color.rgb = texture(tex, vertex_tex).rgb;

	color.rgb *= diffuse*0.7;

	vec3 cd = normalize(vertex_pos - campos);
	vec3 h = normalize(cd+ld);
	float spec = dot(n,h);
	spec = clamp(spec,0,1);
	spec = pow(spec,20);
	color.rgb += vec3(1,1,1)*spec*3;
	*/


	color.a=1;
	color.rgb = texture(tex, -vertex_tex).rgb;

	/*
	if (intensity > 0.95)
		color += vec4(0.25,0.25,0.25,0.0);
	else if (intensity > 0.5)
		color += vec4(0.0,0.0,0.0,0.0);
	else if (intensity > 0.25)
		color -= vec4(0.25,0.25,0.25,0.0);
	else
		color -= vec4(0.5,0.5,0.5,0.0);
	*/
	
	vec3 nn = normalize(vertex_normal);
	vec3 light_pos = lightPos;
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
	//if (outline > -0.2 && outline < 0.2)
	if (outline == 0)
	{
		intensity = 0.0;
	}

	color.rgb = color.rgb * intensity;
}
