#include "App.h"

int	App::mouse_x = 0;
int App::mouse_y = 0;
bool App::LeftPressed = false;
int App::screenWidth = 600;
int App::screenHeight = 600;
bool App::keys[256];
double App::deltaTime = 0;


App::App()
{

}

glm::vec3 App::ConvertMatToEulerAnglesXYZ(const glm::mat3& mat)
{
	const float xEuler = std::atan2(mat[1].z, mat[2].z);
	const float cosYangle = glm::sqrt(pow(mat[0].x, 2) + pow(mat[0].y, 2));
	const float yEuler = std::atan2(-mat[0].z, cosYangle);
	const float sinXangle = glm::sin(xEuler);
	const float cosXangle = glm::cos(xEuler);
	const float zEuler = std::atan2(
		sinXangle * mat[2].x - cosXangle * mat[1].x,
		cosXangle * mat[1].y - sinXangle * mat[2].y);

	return glm::vec3(xEuler, yEuler, zEuler);
}

void App::printMatrix(const glm::mat4 & mat)
{
	cout << mat[0][0] << ", " << mat[0][1] << ", " << mat[0][2] << ", " << mat[0][3] << "\n";
	cout << mat[1][0] << ", " << mat[1][1] << ", " << mat[1][2] << ", " << mat[1][3] << "\n";
	cout << mat[2][0] << ", " << mat[2][1] << ", " << mat[2][2] << ", " << mat[2][3] << "\n";
	cout << mat[3][0] << ", " << mat[3][1] << ", " << mat[3][2] << ", " << mat[3][3] << "\n";
}

glm::mat4 App::CloneMatrix(const glm::mat4 matrix)
{
	glm::mat4 copy = glm::mat4(matrix[0], matrix[1], matrix[2], matrix[3]);

	return copy;
}
