// Fragment Shader

#version 150

in  vec3 ex_Color;  //colour arriving from the vertex
out vec4 out_Color; //colour for the pixel

void main(void)
{
	out_Color = vec4(ex_Color,1.0);
	if(ex_Color == vec3(0, 0, 0))
	{
		out_Color = vec4(0.2,0.4,0.65,1.0);
	}
}