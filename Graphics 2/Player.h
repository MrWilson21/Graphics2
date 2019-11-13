#pragma once
#include "App.h"

class Player
{
private:
	glm::mat4 ModelViewMatrix;

	//Material properties
	float Material_Ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float Material_Diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float Material_Specular[4] = { 0.9f,0.9f,0.8f,1.0f };
	float Material_Shininess = 50;

	float spinForce = 250;
	glm::vec3 rotationForce;
	float linearRotationDamping = 4;
	float staticRotationDamping = 0.5;
	float xCorrectionForce = 300;
	float zCorrectionForce = 500;

	float accelerationForce = 30;
	float linearDrag = 1;
	float staticDrag = 0.5;

public:
	ThreeDModel model;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::mat4 objectRotation;

	glm::vec3 velocity;

	glm::vec3 localUp;
	glm::vec3 localForward;
	glm::vec3 localRight;

	Player();

	void init(OBJLoader* ObjLoader, Shader* myShader);
	void display(Shader* myShader, Shader* myBasicShader, glm::mat4* viewingMatrix, glm::mat4* ProjectionMatrix);
	void move();
	void update();
};

