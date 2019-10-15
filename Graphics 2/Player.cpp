#include "Player.h"

Player::Player()
{
}

void Player::display(Shader* myShader, Shader* myBasicShader, glm::mat4* viewingMatrix, glm::mat4* ProjectionMatrix)
{
	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_ambient"), 1, Material_Ambient);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_diffuse"), 1, Material_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_specular"), 1, Material_Specular);
	glUniform1f(glGetUniformLocation(myShader->handle(), "material_shininess"), Material_Shininess);


	//DRAW THE MODEL
	ModelViewMatrix = *viewingMatrix;
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(5, 5, 5));
	ModelViewMatrix = ModelViewMatrix * objectRotation;

	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);


	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

	model.drawElementsUsingVBO(myShader);

	glUseProgram(myBasicShader->handle());  // use the shader
	glUniformMatrix4fv(glGetUniformLocation(myBasicShader->handle(), "ProjectionMatrix"), 1, GL_FALSE, &(*ProjectionMatrix)[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(myBasicShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

	//model.drawBoundingBox(myBasicShader);
	//model.drawOctreeLeaves(myBasicShader);
}

void Player::init(OBJLoader* objLoader, Shader* myShader)
{
	cout << " loading model " << endl;
	if (objLoader->loadModel("TestModels/axes.obj", model))//returns true if the model is loaded, puts the model in the model parameter
	{
		cout << " model loaded " << endl;

		//if you want to translate the object to the origin of the screen,
		//first calculate the centre of the object, then move all the vertices
		//back so that the centre is on the origin.
		//model.calcCentrePoint();
		//model.centreOnZero();


		model.calcVertNormalsUsingOctree();  //the method will construct the octree if it hasn't already been created.


		//turn on VBO by setting useVBO to true in threeDmodel.cpp default constructor - only permitted on 8 series cards and higher
		model.initDrawElements();
		model.initVBO(myShader);
		model.deleteVertexFaceData();

	}
	else
	{
		cout << " model failed to load " << endl;
	}
}

void Player::move()
{
	float xinc = 0, yinc = 0, zinc = 0;
	if (App::keys[VK_UP])
	{
		xinc = 0.01f;
	}
	if (App::keys[VK_DOWN])
	{
		xinc = -0.01f;
	}
	if (App::keys[VK_LEFT])
	{
		yinc = 0.01f;
	}
	if (App::keys[VK_RIGHT])
	{
		yinc = -0.01f;
	}
	if (App::keys[VK_SPACE])
	{
		zinc = 0.01f;
	}
	if (App::keys[VK_SHIFT])
	{
		zinc = -0.01f;
	}


	glm::mat4 matrixX, matrixXY;

	//rotation about the local x axis
	q = glm::angleAxis(xinc, glm::vec3(objectRotation[0][0], objectRotation[0][1], objectRotation[0][2]));
	matrixX = glm::mat4_cast(q) * objectRotation;

	//EXAMPLE FOR ACCESSING USING A 1D array
	const float *pSource = (const float*)glm::value_ptr(matrixX);
	//rotation about the local y axis
	q = glm::angleAxis(yinc, glm::vec3(pSource[4], pSource[5], pSource[6]));
	matrixXY = glm::mat4_cast(q) * matrixX;

	//EXAMPLE ACCESSING WITH 2D GLM structure.
	//rotation about the local z axis
	q = glm::angleAxis(zinc, glm::vec3(matrixXY[2][0], matrixXY[2][1], matrixXY[2][2]));
	objectRotation = glm::mat4_cast(q) * matrixXY;
}

void Player::update()
{
	move();
}
