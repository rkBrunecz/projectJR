uniform sampler2D texture;
uniform vec4 color;

void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	
	if(pixel.r == color.r && pixel.g == color.g && pixel.b == color.b && pixel.a != 0.0)
		pixel.a = color.a;
		
	gl_FragColor = pixel;
}