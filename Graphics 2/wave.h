#pragma once

#include "App.h"

class Shader;

class Wave
{
private:

	static const int size = 500;

	static const int NumberOfVertexCoords = size * size * 3;
	static const int numberOfTris = (size - 1) * (size - 1) * 2;
	static const int NumberOfTriangleIndices = numberOfTris * 3;

	float dim;
	unsigned int m_vaoID;		    // vertex array object
	unsigned int m_vboID[2];		// two VBOs - used for colours and vertex data
	GLuint ibo;                     //identifier for the triangle indices

	float verts[NumberOfVertexCoords];
	unsigned int tris[NumberOfTriangleIndices];

	Shader* myShader;

	float time = 0;

	float dr[16];
	float sp[8];
	float wl[8];
	float amp[8];

public:
	Wave();
	void constructGeometry(Shader* myShader, float minx, float minz, float maxx, float maxz, float y);
	void render(glm::mat4 viewMatrix, glm::mat4 projection, unsigned int envTex);
};

