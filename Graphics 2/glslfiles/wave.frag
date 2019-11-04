#version 150
#extension GL_NV_shadow_samplers_cube : enable

out vec4 out_Color; //colour for the pixel

in vec3 position;
in vec3 worldNormal;
in vec3 eyeNormal;
uniform vec3 eyePos;
uniform samplerCube envMap;

void main() {
     vec3 eye = normalize(eyePos - position);
     vec3 r = reflect(eye, worldNormal);
     vec4 color = mix(textureCube(envMap, r), vec4(0.2,0.4,0.65,1.0), 0.3);
     color.a = 0.5;
     out_Color = color;
	 //out_Color = vec4(position.y - 127.5, 0.0, 0.0, 1.0);
}

/*void main(void)
{
	out_Color = vec4(ex_Color,1.0);
}*/