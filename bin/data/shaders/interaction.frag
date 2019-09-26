#version 330

uniform vec2 res;
uniform sampler2DRect kTex;

in vec2 texCoordVarying;

out vec4 fragColor;

void main()
{
	//	Flip axis
	vec2 pos = gl_FragCoord.xy;
	pos.y = res.y - pos.y;

	//	Lookup tex
	vec4 col = texture(kTex, pos);

	//	Threshold
	float light = length(col.rgb);
	if(light <= 0.0) col.r = 0.0;
	else col.r = 1.0;

	//	Frag out
	fragColor = vec4(col.r);
}