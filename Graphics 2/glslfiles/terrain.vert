#version 150

uniform mat4 ViewingMatrix;
uniform mat4 ProjectionMatrix;

in  vec3 in_Position;  // Position coming in
in  vec3 in_Color;     // colour coming in
in	vec3 in_Normal;
out vec3 ex_Color;     // colour leaving the vertex, this will be sent to the fragment shader

void main(void)
{
	gl_Position = ProjectionMatrix * ViewingMatrix * vec4(in_Position, 1.0);
	ex_Color = in_Color;
	//ex_Color = vec3(in_Position.y / 48.0, 1.0 - in_Position.y / 48.0, 1.0);
	//ex_Color = in_Normal;
}