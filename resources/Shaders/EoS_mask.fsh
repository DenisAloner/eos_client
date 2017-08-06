uniform sampler2D Map;
uniform sampler2D Unit;

void main(void)
{
	ivec2 size = textureSize(Map, 0);
	float TexStepX = 1.0f / size[0];
	float TexStepY = 1.0f / size[1];
	vec4 color = texture2D(Map, gl_TexCoord[0].xy);
	vec4 unitv = texture2D(Unit, gl_TexCoord[1].xy);
	if (unitv[3]>0.1f)
	{
		gl_FragColor = vec4(color[0], color[1], color[2], color[3] * 0.75);
		if (color[3]>0.0f)
		{
			vec4 t1 = texture2D(Map, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y - TexStepY));
			vec4 t2 = texture2D(Map, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y + TexStepY));
			vec4 t3 = texture2D(Map, vec2(gl_TexCoord[0].x - TexStepX, gl_TexCoord[0].y));
			vec4 t4 = texture2D(Map, vec2(gl_TexCoord[0].x + TexStepX, gl_TexCoord[0].y));
			if (t1[3] == 0.0f || t2[3] == 0.0f || t3[3] == 0.0f || t4[3] == 0.0f)
			{
				gl_FragColor = vec4(0.7f, 0.1f, 0.0f, 1.0f);
			}
		}
	}
	else
	{
		gl_FragColor = color;
	}
}