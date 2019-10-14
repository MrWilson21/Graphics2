#pragma once
#include <windows.h>		// Header File For Windows
#include "gl/glew.h"
#include "gl/wglew.h"
#pragma comment(lib, "glew32.lib")

#include "shaders/Shader.h"   // include shader header file, this is not part of OpenGL

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"

//MODEL LOADING
#include "3DStruct\threeDModel.h"
#include "Obj\OBJLoader.h"


class App
{
public:
	App();
	~App();
};

