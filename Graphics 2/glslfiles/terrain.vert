#version 150

uniform mat4 ViewingMatrix;
uniform mat4 ProjectionMatrix;
uniform float terraceHeight;
uniform vec3 cameraPos;

in  vec3 in_Position;  // Position coming in
in  vec3 in_Color;     // colour coming in
in	vec3 in_Normal;

out vec3 ex_Color;     // colour leaving the vertex, this will be sent to the fragment shader
out vec3 ex_PositionEye; 
out vec3 ex_PositionVert;

void main(void)
{
	gl_Position = ProjectionMatrix * ViewingMatrix * vec4(in_Position, 1.0);

	vec3 brightness = vec3(dot(in_Normal, vec3(0,1,0)));

	//ex_Color = vec3(1, 1, 0) * in_Normal;
	//ex_Color = vec3(dot(in_Normal, vec3(0,1,0)));
	ex_Color = in_Color;// * (0.2 + 0.8 * brightness);

	ex_PositionEye = cameraPos;
	ex_PositionVert = in_Position;
}