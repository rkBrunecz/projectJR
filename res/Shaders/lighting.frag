uniform sampler2D texture;
uniform vec4 lightColor[200];
uniform vec3 lights[200]; // Lights contains the light position in x and y. The variable z contains the number of "steps" the light takes away from its position.
uniform float intensities[200];
uniform int numLights;

void main()
{
    vec4 texColor = texture2D(texture, gl_TexCoord[0].xy);

	for(int i = 0; i < numLights; i++)
	{	
		float dist = distance(gl_FragCoord, lights[i].xy);
		
		if(dist < lights[i].z)
		{
			vec4 posColor = (lightColor[i] / lights[i].z) * (lights[i].z - dist);
			
			texColor = max(texColor, (texColor + posColor * intensities[i]) + (texColor * posColor * intensities[i]));
		}
	}
	
    gl_FragColor = texColor;
}