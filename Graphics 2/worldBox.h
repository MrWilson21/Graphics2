#ifndef _WBOX_H
#define _WBOX_H

#include "gl\glew.h"

class Shader;

class worldBox
{
private:
	float dim;
	unsigned int m_vaoID;		    // vertex array object
	unsigned int m_vboID[2];		// two VBOs - used for colours and vertex data
	GLuint ibo;                     //identifier for the triangle indices

	static int numOfVerts;
	static int numOfTris;

	float verts[24];
	float cols[24];
	unsigned int tris[36];
public:
	worldBox();
	void constructGeometry(Shader* myShader, float minx, float miny, float minz, float maxx, float maxy, float maxz);
	void render();
};

#endif _WBOX_H