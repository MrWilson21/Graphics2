#include "Player.h"

Player::Player()
{
	localUp = glm::vec3(0, 1, 0);
	localForward = glm::vec3(0, 0, 1);
	localRight = glm::vec3(1, 0, 0);
}

void Player::display(Shader* myShader, Shader* myBasicShader, glm::mat4* viewingMatrix, glm::mat4* ProjectionMatrix)
{
	//cout << position.x << ", " << position.y << ", " << position.z << "\n";
	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_ambient"), 1, Material_Ambient);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_diffuse"), 1, Material_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_specular"), 1, Material_Specular);
	glUniform1f(glGetUniformLocation(myShader->handle(), "material_shininess"), Material_Shininess);


	//DRAW THE MODEL
	ModelViewMatrix = App::CloneMatrix(*viewingMatrix);
	//ModelViewMatrix = glm::mat4(1.0);
	//ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(3, 3, 3));
	glm::mat4 ModelViewMatrixR = App::CloneMatrix(*viewingMatrix);

	ModelViewMatrix = glm::translate(ModelViewMatrix, position);
	ModelViewMatrix = ModelViewMatrix * objectRotation;

	ModelViewMatrixR = glm::translate(ModelViewMatrixR, position);
	ModelViewMatrixR = ModelViewMatrixR * RotorRotation;

	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);


	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

	if (App::keys[VK_INSERT])
	{
		modelCollider.drawElementsUsingVBO(myShader);
	}
	else
	{
		model1.drawElementsUsingVBO(myShader);
		normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrixR));
		glUniformMatrix3fv(glGetUniformLocation(myShader->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrixR[0][0]);
		rotor.drawElementsUsingVBO(myShader);
	}

	glUseProgram(myBasicShader->handle());  // use the shader
	glUniformMatrix4fv(glGetUniformLocation(myBasicShader->handle(), "ProjectionMatrix"), 1, GL_FALSE, &(*ProjectionMatrix)[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(myBasicShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

	//model.drawBoundingBox(myBasicShader);
	//modelCollider.drawOctreeLeaves(myBasicShader);

}

void Player::init(OBJLoader* objLoader, Shader* myShader)
{
	cout << " loading model " << endl;
	if (objLoader->loadModel("TestModels/axes.obj", model1))//returns true if the model is loaded, puts the model in the model parameter
	{
		cout << " model loaded " << endl;

		//if you want to translate the object to the origin of the screen,
		//first calculate the centre of the object, then move all the vertices
		//back so that the centre is on the origin.
		//model.calcCentrePoint();
		//model.centreOnZero();


		model1.calcVertNormalsUsingOctree();  //the method will construct the octree if it hasn't already been created.


		//turn on VBO by setting useVBO to true in threeDmodel.cpp default constructor - only permitted on 8 series cards and higher
		model1.initDrawElements();
		model1.initVBO(myShader);
		//model.deleteVertexFaceData();

	}
	else
	{
		cout << " model failed to load " << endl;
	}
	cout << " loading model " << endl;
	if (objLoader->loadModel("TestModels/rotor.obj", rotor))//returns true if the model is loaded, puts the model in the model parameter
	{
		cout << " model loaded " << endl;

		//if you want to translate the object to the origin of the screen,
		//first calculate the centre of the object, then move all the vertices
		//back so that the centre is on the origin.
		//model.calcCentrePoint();
		//model.centreOnZero();

		rotor.calcVertNormalsUsingOctree();  //the method will construct the octree if it hasn't already been created.


		//turn on VBO by setting useVBO to true in threeDmodel.cpp default constructor - only permitted on 8 series cards and higher
		rotor.initDrawElements();
		rotor.initVBO(myShader);
		//model.deleteVertexFaceData();

	}
	else
	{
		cout << " model failed to load " << endl;
	}
	cout << " loading model " << endl;
	if (objLoader->loadModel("TestModels/subCollider.obj", modelCollider))//returns true if the model is loaded, puts the model in the model parameter
	{
		cout << " model loaded " << endl;

		//if you want to translate the object to the origin of the screen,
		//first calculate the centre of the object, then move all the vertices
		//back so that the centre is on the origin.
		//model.calcCentrePoint();
		//model.centreOnZero();


		modelCollider.calcVertNormalsUsingOctree();  //the method will construct the octree if it hasn't already been created.


		//turn on VBO by setting useVBO to true in threeDmodel.cpp default constructor - only permitted on 8 series cards and higher
		modelCollider.initDrawElements();
		modelCollider.initVBO(myShader);
		//model.deleteVertexFaceData();

	}
	else
	{
		cout << " model failed to load " << endl;
	}
	//for (int i = 0; i < modelCollider.numberOfVertices; i++)
	//{
	//	cout << modelCollider.theVerts[i] << "\n";
	//}
}

void Player::move()
{
	if (App::keys[VK_UP])
	{
		rotationForce.z += spinForce * App::deltaTime;
		//position.y += accelerationForce * App::deltaTime;
	}
	if (App::keys[VK_DOWN])
	{
		rotationForce.z += -spinForce * App::deltaTime;
		//position.y -= accelerationForce * App::deltaTime;
	}
	if (App::keys[VK_LEFT])
	{
		rotationForce.y += spinForce * App::deltaTime;
	}
	if (App::keys[VK_RIGHT])
	{
		rotationForce.y += -spinForce * App::deltaTime;
	}
	if (App::keys[VK_BACK] && rotationForce.x < 499.0)
	{
		rotationForce.x = 500;
	}

	float xDecrement = (abs(rotationForce.x) * linearRotationDamping + staticRotationDamping) * App::deltaTime;
	if (xDecrement >= abs(rotationForce.x)) rotationForce.x = 0;
	else rotationForce.x = (rotationForce.x > 0) ? rotationForce.x - xDecrement : rotationForce.x + xDecrement;

	float yDecrement = (abs(rotationForce.y) * linearRotationDamping + staticRotationDamping) * App::deltaTime;
	if (yDecrement >= abs(rotationForce.y)) rotationForce.y = 0;
	else rotationForce.y = (rotationForce.y > 0) ? rotationForce.y - yDecrement : rotationForce.y + yDecrement;

	float zDecrement = (abs(rotationForce.z) * linearRotationDamping + staticRotationDamping) * App::deltaTime;
	if (zDecrement >= abs(rotationForce.z)) rotationForce.z = 0;
	else rotationForce.z = (rotationForce.z > 0) ? rotationForce.z - zDecrement : rotationForce.z + zDecrement;

	//rotation = glm::radians(glm::eulerAngles(glm::quat_cast(objectRotation)));
	//glm::vec3 aa = App::ConvertMatToEulerAnglesXYZ(glm::mat3(objectRotation));
	//cout << aa.x << ", " << aa.y << ", " << aa.z << "\t\t\t";

	//rotation.x += rotationForce.x * App::deltaTime;
	//rotation.y += rotationForce.y * App::deltaTime;
	//rotation.z += rotationForce.z * App::deltaTime;

	//objectRotation = glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z);

	glm::quat currentRotation;

	glm::quat newRotation;
	currentRotation = glm::quat_cast(objectRotation);

	glm::mat4 matrixX, matrixXY;
	glm::mat4 matrixXR, matrixXYR;

	glm::vec3 localZSub = glm::vec3(objectRotation[2][0], objectRotation[2][1], objectRotation[2][2]);
	
	glm::vec3 globalX = glm::vec3(1, 0, 0);
	glm::vec3 globalY = glm::vec3(0, 1, 0);
	glm::vec3 globalZ = glm::vec3(0, 0, 1);

	glm::vec3 i = localZSub + position;
	glm::vec3 p = position;
	glm::vec3 b = i - p;
	float dist = glm::dot(b, globalY);

	glm::vec3 axis = localZSub;

	rotationForce.x += dist * App::deltaTime * xCorrectionForce;

	localZSub = glm::vec3(objectRotation[0][0], objectRotation[0][1], objectRotation[0][2]);
	i = localZSub + position;
	p = position;
	b = i - p;
	dist = glm::dot(b, globalY);

	rotationForce.z -= pow(dist, 2) * App::deltaTime * zCorrectionForce * (dist < 0 ? -1 : 1);
	if (rotationForce.z < 8 || rotationForce.z > -12)
	{
		rotationForce.z -= (dist < 0 ? -1 : 1) * staticZCorrectionForce * App::deltaTime;
	}

	rotorSpin += glm::dot(localForward, velocity) * App::deltaTime * rotorSpeed;

	//float angle = glm::dot(b, g);
	//cout << localZSub.x << "," << localZSub.y << "," << localZSub.z << "\n";
	//cout << axis.x << "," << axis.y << "," << axis.z << ", " << angle << "\n";
	
	//rotation about the local x axis
	newRotation = glm::angleAxis(rotationForce.x * (float)App::deltaTime, glm::vec3(objectRotation[0][0], objectRotation[0][1], objectRotation[0][2]));
	matrixX = glm::mat4_cast(newRotation) * objectRotation;

	//rotation about the local y axis
	newRotation = glm::angleAxis(rotationForce.y * (float)App::deltaTime, glm::vec3(matrixX[1][0], matrixX[1][1], matrixX[1][2]));
	matrixXY = glm::mat4_cast(newRotation) * matrixX;

	//rotation about the local z axis
	newRotation = glm::angleAxis(rotationForce.z * (float)App::deltaTime, glm::vec3(matrixXY[2][0], matrixXY[2][1], matrixXY[2][2]));
	objectRotation = glm::mat4_cast(newRotation) * matrixXY;

	newRotation = glm::angleAxis(rotorSpin, glm::vec3(objectRotation[0][0], objectRotation[0][1], objectRotation[0][2]));
	RotorRotation = glm::mat4_cast(newRotation) * objectRotation;

	//newRotation = glm::angleAxis((float)(glm::degrees(angle) * App::deltaTime * 0.1), glm::vec3(objectRotation[0][0], objectRotation[0][1], objectRotation[0][2]));
	//objectRotation = glm::mat4_cast(newRotation) * objectRotation;

	currentRotation = glm::quat_cast(objectRotation);
	//cout << currentRotation.x << ", " << currentRotation.y << ", " << currentRotation.z << ", " << currentRotation.w << "\n";

	rotation = App::ConvertMatToEulerAnglesXYZ(glm::mat3(objectRotation));

	//cout << rotationForce.x << ", " << rotationForce.y << ", " << rotationForce.z << "\n";

	if (App::keys[0x57])
	{
		velocity += (glm::vec3)glm::translate(objectRotation, glm::vec3(-accelerationForce * App::deltaTime, 0, 0))[3];
	}
	if (App::keys[0x53])
	{
		velocity += (glm::vec3)glm::translate(objectRotation, glm::vec3(accelerationForce * App::deltaTime, 0, 0))[3];
	}
	if (App::keys[VK_SHIFT])
	{
		velocity.y -= sinkForce * App::deltaTime;
	}
	if (App::keys[VK_SPACE])
	{
		velocity.y += riseForce * App::deltaTime;
	}
	
	float newVelMagnitude = glm::length(velocity) - ((glm::length(velocity) * linearDrag + staticDrag) * App::deltaTime);
	if (newVelMagnitude <= 0)
	{
		velocity = glm::vec3(0, 0, 0);
	}
	else
	{
		velocity = glm::normalize(velocity) * newVelMagnitude;
	}

	position += velocity * (float)App::deltaTime;

	localRight = (glm::vec3)glm::translate(objectRotation, glm::vec3(0, 0, 1))[3];
	localUp = (glm::vec3)glm::translate(objectRotation, glm::vec3(0, 1, 0))[3];
	localForward = (glm::vec3)glm::translate(objectRotation, glm::vec3(1, 0, 0))[3];
}

void Player::update()
{
	move();
}
