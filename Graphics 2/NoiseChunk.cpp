#include "NoiseChunk.h"



NoiseChunk::NoiseChunk()
{
}

void NoiseChunk::render()
{
	glUseProgram(myShader->handle());
	//draw objects
	glBindVertexArray(m_vaoID);		// select VAO

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, numOfTris * 3, GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Done

	glBindVertexArray(0); //unbind the vertex array object

}

void NoiseChunk::genTerrain(Shader* myShader, glm::vec3 offset)
{
	this->myShader = myShader;

	float data[size][height][size];

	float xFactor = 1.0f / (size - 1);
	float zFactor = 1.0f / (size - 1);
	float yFactor = 1.0f / (height - 1);

	for (int col = 0; col < size; col++) {
		for (int layer = 0; layer < height; layer++) {
			for (int row = 0; row < size; row++) {
				float x = xFactor * col;
				float y = yFactor * layer;
				float z = zFactor * row;

				float val = 0;
				for (int i = 1; i <= octaves; i++)
				{
					glm::vec3 p((x + offset.x) * (chunkFreq / i), (y + offset.y) * (chunkFreq / i) * height / size, (z + offset.z) * (chunkFreq / i));
					float sum = (1.0332f + glm::perlin(p)) / 2.06778;
					val += sum / pow(2, i);
				}
				//glm::vec3 p(x * (chunkFreq), y * (chunkFreq), z * (chunkFreq));
				//val = (1.0332f + glm::perlin(p)) / 2.06778;

				
				if (createTerraces)
				{
					float a = fmod(y, terraceHeight) / terraceHeight;
					//if (a < 0.5)
					{
						if (val > minTerraceSurfaceLevel + (terraceIncrement) * a && y < ceilingLevel)
						{
							val = surfaceLevel;
						}
					}

					//val += (fmod(y, terraceHeight) - fmod(y, terraceHeight*2)) *  terraceWeight;
				}

				if (y > ceilingLevel)
				{
					val -= ((ceilingLevel - y) / (1.0 - ceilingLevel)) * ceilingWeight;
				}
				else if (y < floorLevel)
				{
					val -= ((floorLevel - y) / floorLevel) * floorWeight;
				}
				
				data[col][layer][row] = val;
			}
		}
	}

	vector<float> vertices;
	vector<float> colours;
	vector<unsigned int> triangles;
	numOfVerts = 0;
	numOfTris = 0;
	unsigned int vertIndexes[size][size][height];

	for (int x = 0; x < size - 1; x++) {
		for (int y = 0; y <  height - 1; y++) {
			for (int z = 0; z < size - 1; z++) {
				glm::vec4 cubeCorners[8] = {
					glm::vec4(x, y, z, data[x][y][z]),
					glm::vec4(x+1, y, z, data[x + 1][y][z]),
					glm::vec4(x+1, y, z+1, data[x+1][y][z+1]),
					glm::vec4(x, y, z+1, data[x][y][z+1]),
					glm::vec4(x, y+1, z, data[x][y+1][z]),
					glm::vec4(x+1, y+1, z, data[x+1][y+1][z]),
					glm::vec4(x+1, y+1, z+1, data[x+1][y+1][z+1]),
					glm::vec4(x, y+1, z+1, data[x][y+1][z+1])
				};

				int cubeIndex = 0;
				if (cubeCorners[0].w < surfaceLevel) cubeIndex |= 1;
				if (cubeCorners[1].w < surfaceLevel) cubeIndex |= 2;
				if (cubeCorners[2].w < surfaceLevel) cubeIndex |= 4;
				if (cubeCorners[3].w < surfaceLevel) cubeIndex |= 8;
				if (cubeCorners[4].w < surfaceLevel) cubeIndex |= 16;
				if (cubeCorners[5].w < surfaceLevel) cubeIndex |= 32;
				if (cubeCorners[6].w < surfaceLevel) cubeIndex |= 64;
				if (cubeCorners[7].w < surfaceLevel) cubeIndex |= 128;			

				// Create triangles for current cube configuration
				for (int i = 0; triangulation[cubeIndex][i] != -1; i += 3) {
					// Get indices of corner points A and B for each of the three edges
					// of the cube that need to be joined to form the triangle.
					int a0 = cornerIndexAFromEdge[triangulation[cubeIndex][i]];
					int b0 = cornerIndexBFromEdge[triangulation[cubeIndex][i]];

					int a1 = cornerIndexAFromEdge[triangulation[cubeIndex][i + 1]];
					int b1 = cornerIndexBFromEdge[triangulation[cubeIndex][i + 1]];

					int a2 = cornerIndexAFromEdge[triangulation[cubeIndex][i + 2]];
					int b2 = cornerIndexBFromEdge[triangulation[cubeIndex][i + 2]];

					glm::vec3 vert = interpolateVerts(cubeCorners[a0], cubeCorners[b0]);
					vertices.push_back((vert.x + (offset.x) * (size-1)) * chunkScale);
					vertices.push_back((vert.y + (offset.y) * (size - 1)) * chunkScale);
					vertices.push_back((vert.z + (offset.z) * (size - 1)) * chunkScale);
					glm::vec3 colour = colourSelector(vert.y / height);
					colours.push_back(colour.x);
					colours.push_back(colour.y);
					colours.push_back(colour.z);
					numOfVerts += 1;

					//cout << "(" << a0 << ", " << b0 << ")  " << "(" << vert.x << ", " << vert.y << ", " << vert.z << ")\t";

					vert = interpolateVerts(cubeCorners[a1], cubeCorners[b1]);
					vertices.push_back((vert.x + (offset.x) * (size - 1)) * chunkScale);
					vertices.push_back((vert.y + (offset.y) * (size - 1)) * chunkScale);
					vertices.push_back((vert.z + (offset.z) * (size - 1)) * chunkScale);
					colour = colourSelector(vert.y / height);
					colours.push_back(colour.x);
					colours.push_back(colour.y);
					colours.push_back(colour.z);
					numOfVerts += 1;

					//cout << "(" << a1 << ", " << b1 << ")  " << "(" << vert.x << ", " << vert.y << ", " << vert.z << ")\t";

					vert = interpolateVerts(cubeCorners[a2], cubeCorners[b2]);
					vertices.push_back((vert.x + (offset.x) * (size - 1)) * chunkScale);
					vertices.push_back((vert.y + (offset.y) * (size - 1)) * chunkScale);
					vertices.push_back((vert.z + (offset.z) * (size - 1)) * chunkScale);
					colour = colourSelector(vert.y / height);
					colours.push_back(colour.x);
					colours.push_back(colour.y);
					colours.push_back(colour.z);
					numOfVerts += 1;
					//cout << "(" << a2 << ", " << b2 << ")  " << "(" << vert.x << ", " << vert.y << ", " << vert.z << ")\n";

					triangles.push_back(numOfVerts - 3);
					triangles.push_back(numOfVerts - 2);
					triangles.push_back(numOfVerts - 1);
					numOfTris ++;

					//cout << x << y << z << "\n";
				}
			}
		}
	}
	verts = &vertices[0];
	cols = &colours[0];
	tInds = &triangles[0];
	
	glUseProgram(myShader->handle());

	// VAO allocation
	glGenVertexArrays(1, &m_vaoID);

	// First VAO setup
	glBindVertexArray(m_vaoID);

	glGenBuffers(2, m_vboID);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);
	//initialises data storage of vertex buffer object
	glBufferData(GL_ARRAY_BUFFER, numOfVerts * 3 * sizeof(GLfloat), verts, GL_STATIC_DRAW);
	GLint vertexLocation = glGetAttribLocation(myShader->handle(), "in_Position");
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexLocation);


	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[1]);
	glBufferData(GL_ARRAY_BUFFER, numOfVerts * 3 * sizeof(GLfloat), cols, GL_STATIC_DRAW);
	GLint colorLocation = glGetAttribLocation(myShader->handle(), "in_Color");
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorLocation);


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfTris * 3 * sizeof(unsigned int), tInds, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	//delete[] data;
}

glm::vec3 NoiseChunk::interpolateVerts(glm::vec4 v1, glm::vec4 v2) {
	float t = (surfaceLevel - v1.w) / (v2.w - v1.w);
	return glm::vec3(v1) + t * (glm::vec3(v2) - glm::vec3(v1));
	//glm::vec3 pos = glm::vec3(glm::vec3(v1) + glm::vec3(v2));
	//return pos * 0.5f;
}

glm::vec3 NoiseChunk::colourSelector(float y)
{
	float colourLevel = fmod(y, terraceHeight) / terraceHeight;
	return glm::vec3(colourLevel, 1.0 - colourLevel, 0.9f);
	//return glm::vec3(0.0, colourLevel, 0.0f);
}


