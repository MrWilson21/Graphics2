#version 150

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

in  vec3 in_Position;  // Position coming in
in  vec3 in_Color;     // colour coming in
out vec3 ex_Color;     // colour leaving the vertex, this will be sent to the fragment shader

void main(void)
{
	gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(in_Position, 1.0);
	ex_Color = in_Color;
	if(in_Color == vec3(0, 0, 0))
	{
		float a = 1.0 - in_Position.y/300;
		vec4 col = mix(	vec4(0.2,0.4,0.65,1.0), vec4(0.05,0.4,0.9,1.0), a);
		col = vec4(0.2,0.4,0.65,1.0);
		ex_Color = vec3(col);
	}
	//ex_Color = vec3(in_Position.y / 48.0, 1.0 - in_Position.y / 48.0, 1.0);
}