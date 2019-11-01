// Fragment Shader

#version 150

in  vec3 ex_Color;  //colour arriving from the vertex
out vec4 out_Color; //colour for the pixel

in vec3 ex_PositionEye;
in vec3 ex_PositionVert;

void main(void)
{
	float xDiff = pow(ex_PositionEye.x - ex_PositionVert.x, 2);
	float yDiff = pow(ex_PositionEye.y - ex_PositionVert.y, 2);
	float zDiff = pow(ex_PositionEye.y - ex_PositionVert.y, 2);
	float disFactor = sqrt(xDiff + yDiff + zDiff);

	float waterHeight = 128;

	float depthFactor = pow(abs(waterHeight - ex_PositionVert.y) / waterHeight, 1.0f); 
	//depthFactor = abs(waterHeight - ex_PositionVert.y) / waterHeight;
	if(ex_PositionVert.y > waterHeight)
	{
		if(ex_PositionEye.y < waterHeight)
		{
			disFactor *= (ex_PositionVert.y - waterHeight) / (ex_PositionVert.y - ex_PositionEye.y);
			depthFactor = pow(abs(waterHeight - ex_PositionEye.y) / waterHeight, 1.0f);
		}
		else
		{
			depthFactor = 0;
		}
	}

	if(ex_PositionEye.y > waterHeight && ex_PositionVert.y < waterHeight)
	{
		//disFactor *= ((abs(ex_PositionEye.y - ex_PositionVert.y)) - (ex_PositionEye.y - waterHeight)) / abs(ex_PositionEye.y - ex_PositionVert.y);
	}

	float fogStart = -0.0;
	float fogEnd = -100.0;
	float fogFactor = (fogEnd - ex_PositionEye.z)/(fogEnd-fogStart);	//fogFactor = depthFactor / 50;
	//fogFactor = (fogEnd + ex_PositionVert.y)/(fogEnd-fogStart);
	fogFactor = (fogEnd + disFactor * depthFactor)/(fogEnd-fogStart);

	fogFactor = clamp(fogFactor,0.0,1.0);
	vec4 fogColor = vec4(0.2,0.4,0.65,1.0);

	out_Color = vec4(ex_Color,1.0);
	out_Color = mix(fogColor,vec4(ex_Color, 1.0), fogFactor);

	//out_Color = vec4(ex_Color,1.0);
}