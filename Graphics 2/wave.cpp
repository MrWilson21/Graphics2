#include "wave.h"
#include "Box.h"
#include "shaders\Shader.h"

Wave::Wave()
{
	dim = 1.0;
}

void Wave::render()
{
	//draw objects
	glBindVertexArray(m_vaoID);		// select VAO

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, numberOfTris * 3, GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Done

	glBindVertexArray(0); //unbind the vertex array object

}

void Wave::constructGeometry(Shader* myShader, float minx, float minz, float maxx, float maxz, float y)
{
	float squareSizeX = (maxx - minx) / (size - 1);
	float squareSizeZ = (maxz - minz) / (size - 1);

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			verts[i * 3 + size * j * 3] = minx + squareSizeX * i;
			verts[i * 3 + size * j * 3 + 1] = y;
			verts[i * 3 + size * j * 3 + 2] = minz + squareSizeZ * i;
		}
	}

	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < size - 1; j++)
		{
			tris[i * 2 * 3 + (size - 1) * j * 2 * 3] = i * 3 + size * j * 3;
			tris[i * 2 * 3 + (size - 1) * j * 2 * 3 + 1] = (i + 1) * 3 + size * j * 3;
			tris[i * 2 * 3 + (size - 1) * j * 2 * 3 + 2] = i * 3 + size * (j + 1) * 3;

			tris[(i + 1) * 2 * 3 + (size - 1) * j * 2 * 3] = (i + 1) * 3 + size * j * 3;
			tris[(i + 1) * 2 * 3 + (size - 1) * j * 2 * 3 + 1] = i * 3 + size * (j + 1) * 3;
			tris[(i + 1) * 2 * 3 + (size - 1) * j * 2 * 3 + 2] = (i + 1) * 3 + size * (j + 1) * 3;
		}
	}

	// VAO allocation
	glGenVertexArrays(1, &m_vaoID);

	// First VAO setup
	glBindVertexArray(m_vaoID);

	glGenBuffers(1, m_vboID);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);
	//initialises data storage of vertex buffer object
	glBufferData(GL_ARRAY_BUFFER, NumberOfVertexCoords * sizeof(GLfloat), verts, GL_STATIC_DRAW);
	GLint vertexLocation = glGetAttribLocation(myShader->handle(), "in_Position");
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexLocation);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfTris * 3 * sizeof(unsigned int), tris, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
