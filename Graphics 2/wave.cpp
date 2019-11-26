#include "wave.h"
#include "Box.h"
#include "shaders\Shader.h"

Wave::Wave()
{
	dim = 1.0;
}

void Wave::render(glm::mat4 viewMatrix, glm::mat4 projection, unsigned int envTex, glm::vec3 cameraPos)
{
	glUseProgram(myShader->handle());

	glUniform1f(glGetUniformLocation(myShader->handle(), "waterHeight"), App::waterHeight);
	glUniform1i(glGetUniformLocation(myShader->handle(), "numWaves"), 4);
	
	glUniform1fv(glGetUniformLocation(myShader->handle(), "amplitude"), 8, &amp[0]);
	glUniform1fv(glGetUniformLocation(myShader->handle(), "wavelength"), 8, &wl[0]);
	glUniform1fv(glGetUniformLocation(myShader->handle(), "speed"), 8, &sp[0]);
	glUniform2fv(glGetUniformLocation(myShader->handle(), "direction"), 8, &dr[0]);

	time += App::deltaTime;
	glUniform1f(glGetUniformLocation(myShader->handle(), "time"), time);

	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &viewMatrix[0][0]);
	glUniform3fv(glGetUniformLocation(myShader->handle(), "eyePos"), 1, &cameraPos[0]);
	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ProjectionMatrix"), 1, GL_FALSE, &projection[0][0]);

	glm::mat4 normalMatrix = glm::inverseTranspose(viewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "normalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

	glUniform1i(glGetUniformLocation(myShader->handle(), "envMap"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envTex);

	//draw objects
	glBindVertexArray(m_vaoID);		// select VAO

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, NumberOfTriangleIndices, GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Done

	glBindVertexArray(0); //unbind the vertex array object

}

void Wave::constructGeometry(Shader* myShader, float minx, float minz, float maxx, float maxz, float y)
{
	this->myShader = myShader;

	float squareSizeX = (maxx - minx) / (size - 1);
	float squareSizeZ = (maxz - minz) / (size - 1);

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			verts[j * 3 + size * i * 3] = minx + squareSizeX * j;
			verts[j * 3 + size * i * 3 + 1] = y;
			verts[j * 3 + size * i * 3 + 2] = minz + squareSizeZ * i;	
		}
	}

	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < size - 1; j++)
		{
			tris[j * 2 * 3 + (size - 1) * i * 2 * 3 + 0] = j * 3 + i * size * 3;
			tris[j * 2 * 3 + (size - 1) * i * 2 * 3 + 1] = (j + 1) * 3 + (i + 1) * size * 3;
			tris[j * 2 * 3 + (size - 1) * i * 2 * 3 + 2] = j * 3 + (i + 1) * size * 3;

			tris[j * 2 * 3 + (size - 1) * i * 2 * 3 + 3] = j * 3 + i * size * 3;
			tris[j * 2 * 3 + (size - 1) * i * 2 * 3 + 4] = (j + 1) * 3 + i * size * 3;
			tris[j * 2 * 3 + (size - 1) * i * 2 * 3 + 5] = (j + 1) * 3 + (i + 1) * size * 3;

			tris[j * 2 * 3 + (size - 1) * i * 2 * 3 + 0] = j + i * size;
			tris[j * 2 * 3 + (size - 1) * i * 2 * 3 + 1] = (j + 1) + (i + 1) * size;
			tris[j * 2 * 3 + (size - 1) * i * 2 * 3 + 2] = j + (i + 1) * size;

			tris[j * 2 * 3 + (size - 1) * i * 2 * 3 + 3] = j + i * size;
			tris[j * 2 * 3 + (size - 1) * i * 2 * 3 + 4] = (j + 1) + i * size;
			tris[j * 2 * 3 + (size - 1) * i * 2 * 3 + 5] = (j + 1) + (i + 1) * size;
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, NumberOfTriangleIndices * sizeof(unsigned int), tris, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);


	for (int i = 0; i < 8; i++)
	{
		amp[i] = 1.2f / (float)(i + 1);
	}

	for (int i = 0; i < 8; i++)
	{
		wl[i] = 32 * 3.14f / (i + 1);
	}
	for (int i = 0; i < 8; i++)
	{
		sp[i] = 2.5f + 5 * i;
	}

	for (int i = 0; i < 8; i++)
	{
		double n = rand() % 361;
		double v = -1.1 + n * ((1.1) - (-1.1));
		dr[i] = sin(n*3.14/180);
		dr[i + 1] = cos(n*3.14 / 180);
	}
}
