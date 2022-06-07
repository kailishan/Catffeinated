#version 410 core
out vec4 color;
in vec2 vertex_tex;
uniform sampler2D tex;

uniform float     u_distortion;
uniform float     u_stripe;
uniform float     u_rgbshift;
uniform float     time;

void main()
{

        // distortion
        vec2 ndc_pos;
        ndc_pos.x = vertex_tex.x;
        ndc_pos.y = -vertex_tex.y;
        vec2 testVec = ndc_pos.xy / max(abs(ndc_pos.x), abs(ndc_pos.y));
        float len = max(1.0,length( testVec ));
        ndc_pos *= mix(1.0, mix(1.0,len,max(abs(ndc_pos.x), abs(ndc_pos.y))), u_distortion);
        vec2 texCoord = vec2(ndc_pos.s, -ndc_pos.t) * 0.5 + 0.5;

        // stripes
        float stripTile = texCoord.t * mix(10.0, 100.0, u_stripe) + time;
        float stripFac = 1.0 + 0.25 * u_stripe * (step(0.5, stripTile-float(int(stripTile))) - 0.5);
        
        // rgb shift
        float texR = texture( tex, texCoord.st-vec2(u_rgbshift) ).r;
        float texG = texture( tex, texCoord.st ).g;
        float texB = texture( tex, texCoord.st+vec2(u_rgbshift) ).b;
        
        float clip = step(0.0, texCoord.s) * step(texCoord.s, 1.0) * step(0.0, texCoord.t) * step(texCoord.t, 1.0); 
        color  = vec4( vec3(texR, texG, texB) * stripFac * clip, 1.0 );
  
    //vec3 tcol = texture(tex, vertex_tex).rgb;
	//color.rgb = tcol;
	//color.a=1;
}
