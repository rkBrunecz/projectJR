uniform sampler2D texture;
uniform vec4 color;
uniform vec3 lights[200];
uniform float maxOpacity;
uniform int numLights;

void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	
	if(pixel.r == color.r && pixel.g == color.g && pixel.b == color.b && pixel.a != 0.0)
	{
		pixel.a = color.a;	
		
		float alpha = 0.0;
		for(int i = 0; i < numLights; i++)
		{		
			vec2 lightPos = vec2(lights[i].x, lights[i].y);
			float steps = lights[i].z;
			
			// Extend the range of where shadows draw to be slightly further than the radius of the light
			if(distance(gl_FragCoord, lightPos) <= steps * 1.2)
				pixel.a = max((1.1 - distance(gl_FragCoord, lightPos) / steps) * maxOpacity, pixel.a);
		}
	}
		
	gl_FragColor = pixel;
}