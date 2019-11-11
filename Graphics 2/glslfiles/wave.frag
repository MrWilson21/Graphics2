#version 330
//#extension GL_NV_shadow_samplers_cube : enable

out vec4 out_Color; //colour for the pixel

in vec3 position;
in vec3 worldNormal;
in vec3 eyeNormal;
uniform float waterHeight;
uniform vec3 eyePos;
uniform samplerCube envMap;

void main() {
	float disFactor = 0;
	float depthFactor = 0;

	float xDiff = pow(eyePos.x - position.x, 2);
	float yDiff = pow(eyePos.y - position.y, 2);
	float zDiff = pow(eyePos.z - position.z, 2);
	disFactor = sqrt(xDiff + yDiff + zDiff);
			
	depthFactor = pow(abs(waterHeight - eyePos.y) / waterHeight, 1.0f);
	if(depthFactor > 0)
	{
		depthFactor = clamp(depthFactor, 0.4, 1.0);
	}
	depthFactor = 0.5;
	float fogStart = -0.0;
	float fogEnd = -100.0;
	float fogFactor = (fogEnd - eyePos.z)/(fogEnd-fogStart);
	fogFactor = (fogEnd + disFactor * depthFactor)/(fogEnd-fogStart);

	fogFactor = clamp(fogFactor,0.0,1.0);
	vec4 fogColor = vec4(0.2,0.4,0.65,1.0);

    vec3 eye = normalize(position - eyePos);
    vec3 r = reflect(eye, worldNormal);
    //vec4 color = mix(textureCube(envMap, eye), vec4(0.2,0.4,0.65,1.0), 0.3);
	vec4 color = mix(texture(envMap, r), fogColor, 0.0);
	if(eyePos.y < position.y)
	{
		color = mix(texture(envMap, -r), vec4(0.2,0.4,0.65,1.0), 0.0 - min(-0.4, -(1- fogFactor)));
	}
    color.a = 0.5;
    out_Color = color;
	//out_Color = vec4(position.y - 127.5, 0.0, 0.0, 1.0);
	//out_Color = vec4(r.y, 0.0, 0.0, 1.0);
}