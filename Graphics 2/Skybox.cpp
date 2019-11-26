#include "Skybox.h"
#include <iostream>
Skybox::Skybox()
{

}

void Skybox::render(glm::mat4 viewMatrix, glm::mat4 projection)
{
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	glUseProgram(myShader->handle());
	glm::mat4 view = glm::mat4(glm::mat3(viewMatrix)); // remove translation from the view matrix
	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "projection"), 1, GL_FALSE, &projection[0][0]);

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);

	glUniform1i(glGetUniformLocation(myShader->handle(), "skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);;

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default

}

void Skybox::constructGeometry(Shader* myShader)
{
	this->myShader = myShader;

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(GLfloat), &verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	vector<std::string> faces
	{
		"Images/right.png",
		"Images/left.png",
		"Images/top.png",
		"Images/bottom.png",
		"Images/back.png",
		"Images/front.png"
	};
	cubemapTexture = loadCubemap(faces);
}

unsigned int Skybox::loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		nv::Image img;
		if (img.loadImageFromFile(faces[i].c_str()))
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, img.getWidth(), img.getHeight(), 0, img.getFormat(), img.getType(), img.getLevel(0));
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
