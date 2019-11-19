
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
#include "glm\ext.hpp"

//MODEL LOADING
#include "3DStruct\threeDModel.h"
#include "Obj\OBJLoader.h"
#include "Images/imageloader.h"

#include <chrono>
#include <time.h>
#include <iostream>
#include <algorithm>
#include <thread> 
#include <stdlib.h>
#include "Images/nvImage.h"
#include <algorithm>

using namespace std;
using namespace std::chrono;


class App
{
public:
	App();

	static int	mouse_x;
	static int mouse_y;
	static bool LeftPressed;
	static bool rightPressed;
	static int screenWidth;
	static int screenHeight;
	static bool keys[256];
	static double deltaTime;
	static float waterHeight;

	static glm::vec3 ConvertMatToEulerAnglesXYZ(const glm::mat3& mat);
	static void printMatrix(const glm::mat4 & mat);
	static void printVec3(const glm::vec3 & vec);

	static glm::mat4 CloneMatrix(const glm::mat4 matrix);

	struct terrainThread
	{
		thread thread;
		bool finished = false;
	};
};

