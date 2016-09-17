uniform sampler2D texture;
uniform float alpha;

void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	
	if(pixel.r == 0.0 && pixel.g == 0.0 && pixel.b == 0.0 && pixel.a != 0.0)
		pixel.a = alpha;
		
	gl_FragColor = pixel;
}