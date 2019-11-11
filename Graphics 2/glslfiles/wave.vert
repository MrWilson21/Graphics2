#version 150

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

uniform mat4 normalMatrix;

in  vec3 in_Position;  // Position coming in
in  vec3 in_Color;     // colour coming in
out vec3 ex_Color;     // colour leaving the vertex, this will be sent to the fragment shader

/*void main(void)
{
	gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(in_Position, 1.0);
	ex_Color = in_Color;
	//ex_Color = vec3(in_Position.y / 48.0, 1.0 - in_Position.y / 48.0, 1.0);
}*/


//uniform mat4 ModelViewMatrix;
//uniform mat4 ProjectionMatrix;

out vec3 position;
out vec3 worldNormal;
out vec3 eyeNormal;

const float pi = 3.14159;
uniform float waterHeight;
uniform float time;
uniform int numWaves;
uniform float amplitude[8];
uniform float wavelength[8];
uniform float speed[8];
uniform vec2 direction[8];

//in vec3 in_Position;

float wave(int i, float x, float y) {
    float frequency = 2.0*pi/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    return amplitude[i] * sin(theta * frequency + time * phase);
}

float waveHeight(float x, float y) {
    float height = 0.0;
    for (int i = 0; i < numWaves; ++i)
        height += wave(i, x, y);
    return height;
}

float dWavedx(int i, float x, float y) {
    float frequency = 2.0*pi/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    float A = amplitude[i] * direction[i].x * frequency;
    return A * cos(theta * frequency + time * phase);
}

float dWavedy(int i, float x, float y) {
    float frequency = 2.0*pi/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    float A = amplitude[i] * direction[i].y * frequency;
    return A * cos(theta * frequency + time * phase);
}

vec3 waveNormal(float x, float y) {
    float dx = 0.0;
    float dy = 0.0;
    for (int i = 0; i < numWaves; ++i) {
        dx += dWavedx(i, x, y);
        dy += dWavedy(i, x, y);
    }
    vec3 n = vec3(-dx, 1.0, -dy);
    return normalize(n);
}

void main() {
    vec4 pos = vec4(in_Position, 1.0);
    pos.y += waveHeight(pos.x, pos.z);
    position = pos.xyz / pos.w;
    worldNormal = waveNormal(pos.x, pos.y);
    eyeNormal = mat3(normalMatrix) * worldNormal;
	if(in_Position.x < -499 || in_Position.x > 499 || in_Position.z < -499 || in_Position.z > 499)
	{
		pos.y = waterHeight;
	}
    gl_Position = ProjectionMatrix * ModelViewMatrix * pos;
}