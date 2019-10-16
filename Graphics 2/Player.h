#pragma once
#include "App.h"

class Player
{
private:
	ThreeDModel model;
	glm::mat4 ModelViewMatrix;

	//Material properties
	float Material_Ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float Material_Diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float Material_Specular[4] = { 0.9f,0.9f,0.8f,1.0f };
	float Material_Shininess = 50;

	float spinForce = 2.5;
	glm::vec3 rotationForce;
	float linearRotationDamping = 4;
	float staticRotationDamping = 0.1;

	glm::vec3 velocity;
	float accelerationForce = 30;
	float linearDrag = 5;
	float staticDrag = 3;

public:
	glm::vec3 postion;
	glm::vec3 rotation;
	glm::mat4 objectRotation;

	Player();

	void init(OBJLoader* ObjLoader, Shader* myShader);
	void display(Shader* myShader, Shader* myBasicShader, glm::mat4* viewingMatrix, glm::mat4* ProjectionMatrix);
	void move();
	void update();
};

