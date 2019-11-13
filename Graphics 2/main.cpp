//OPENGL 3.2 DEMO FOR RENDERING OBJECTS LOADED FROM OBJ FILES

//includes areas for keyboard control, mouse control, resizing the window
//and draws a spinning rectangle

#include "App.h"
#include "Player.h"
#include "NoiseChunk.h"
#include "Box.h"
#include "worldBox.h"
#include "wave.h"
#include "Skybox.h"
#include "Octree/Octree.h"
#include "Utilities/IntersectionTests.h"

Shader* myShader;  ///shader object 
Shader* myBasicShader;
Shader* terrainShader;
Shader* skyboxShader;
Shader* waterShader;
Shader* waveShader;

double maxFrameTime = 0.5;	//Unusual object movement can occur if a frame takes too long to render so a max should be set
int maxFps = 200;
steady_clock::time_point totalFrameTime = steady_clock::now();
float timeScale = 1;


float amount = 0;
float temp = 0.002f;
	
const int renderDist = 0;
const int chunksAmount = renderDist * 2 + 1;
NoiseChunk terrainGenerator[chunksAmount][chunksAmount];
bool terrainGenStatus[chunksAmount][chunksAmount];
bool terrainRenderStatus[chunksAmount][chunksAmount];
int maxThreads;
int currentThreads = 0;
vector <App::terrainThread> threads;
vector<glm::vec2> chunkQueue;

float waterRenderDist = 1000;

glm::vec3 chunkOffset = glm::vec3(renderDist + 0.5f, 1, renderDist + 0.5f);
float chunkHalfSize = ((NoiseChunk::size - 1.0f) * NoiseChunk::chunkScale / 2.0f);

Player player = Player();
worldBox b;
worldBox b2;
Wave w;
Skybox skybox;

OBJLoader objLoader;
///END MODEL LOADING

glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
glm::mat4 ModelViewMatrix = glm::mat4(1.0);
glm::mat4 viewingMatrix;

//Light Properties
float Light_Ambient_And_Diffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float Light_Specular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float LightPos[4] = {0.0f, 10.0f, 0.0f, 0.0f};

float camAngleX = 0;
float camAngleY = 0;
float camRadius = 15.0f;
float camRadiusMin = 3;
float camRadiusMax = 20;
glm::vec2 mousePrevious = glm::vec2(0, 0);
glm::vec2 mouseDifference = glm::vec2(0, 0);
float spinSpeed = 0.005;

float camMode = 1;
int collisionCount = 0;
float timer = 0;

//OPENGL FUNCTION PROTOTYPES
void display();				//called in winmain to draw everything to the screen
void reshape(int width, int height);				//called when the window is resized
void init();				//called in winmain when the program starts.
void update();				//called in winmain to update variables
void genNewChunk(int i, int j, bool* finished);
void updateRenderStatus();
void updateThreads();
void doCollisions(Octree* playerOct, Octree* terrainOct, ThreeDModel* terrainModel);

struct vec3;
struct OBB;
bool getSeparatingPlane(const vec3& RPos, const vec3& Plane, const OBB& box1, const OBB&box2);
bool getCollision(const OBB& box1, const OBB&box2);

struct vec3
{
	float x, y, z;
	vec3 operator- (const vec3 & rhs) const { return{ x - rhs.x, y - rhs.y, z - rhs.z }; }
	float operator* (const vec3 & rhs) const { return{ x * rhs.x + y * rhs.y + z * rhs.z }; } // DOT PRODUCT
	vec3 operator^ (const vec3 & rhs) const { return{ y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x }; } // CROSS PRODUCT
	vec3 operator* (const float& rhs)const { return vec3{ x * rhs, y * rhs, z * rhs }; }
};

// set the relevant elements of our oriented bounding box
struct OBB
{
	vec3 Pos, AxisX, AxisY, AxisZ, Half_size;
};

/*************    START OF OPENGL FUNCTIONS   ****************/
void display()									
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);

	glUseProgram(myShader->handle());  // use the shader

	amount += temp;
	if(amount > 1.0f || amount < -0.5f)
		temp = -temp;
	//amount = 0;
	glUniform1f(glGetUniformLocation(myShader->handle(), "displacement"), amount);

	GLuint matLocation = glGetUniformLocation(myShader->handle(), "ProjectionMatrix");  
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);

	//glm::mat4 viewingMatrix = glm::translate(glm::mat4(1.0),glm::vec3(0, 0, -50));
	if (camMode == 0)
	{
		glm::vec3 forward = glm::vec3(0, 0, 1);
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = glm::vec3(1, 0, 0);
		glm::vec3 cosAngleX = player.localForward * camRadius * cos(camAngleX);
		glm::vec3 sinAngleX = player.localRight * camRadius * sin(camAngleX);
		glm::vec3 cosAngleY = glm::normalize(cosAngleX + sinAngleX) * camRadius * sin(camAngleY);
		glm::vec3 sinAngleY = up * camRadius * sin(camAngleY);
		glm::vec3 camOffset = cosAngleX + sinAngleX + cosAngleY + sinAngleY;
		camOffset = glm::normalize(camOffset) * camRadius;
		viewingMatrix = glm::lookAt(player.position + camOffset, player.position, glm::vec3(0, 1, 0));
	}
	else if(camMode == 1)
	{
		viewingMatrix = glm::lookAt(player.position + player.localForward * 15.0f + player.localUp * 5.0f, player.position, glm::vec3(0, 1, 0));
	}
	else
	{
		viewingMatrix = glm::lookAt(player.position - player.localForward * 4.5f, player.position - player.localForward * 5.5f, glm::vec3(0, 1, 0));
	}
	glm::vec3 cameraPos = player.position + player.localForward * 15.0f + player.localUp * 5.0f;
	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);

	glUniform4fv(glGetUniformLocation(myShader->handle(), "LightPos"), 1, LightPos);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "light_ambient"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "light_diffuse"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "light_specular"), 1, Light_Specular);

	player.display(myShader, myBasicShader, &viewingMatrix, &ProjectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);

	glUseProgram(myBasicShader->handle());  // use the shader
	glUniformMatrix4fv(glGetUniformLocation(myBasicShader->handle(), "ProjectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(myBasicShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);
	for (int i = 0; i < chunksAmount; i++)
	{
		for (int j = 0; j < chunksAmount; j++)
		{
			if (terrainRenderStatus[i][j])
			{
				//terrainGenerator[i][j].model.drawBoundingBox(myBasicShader);
				//terrainGenerator[i][j].model.drawOctreeLeaves(myBasicShader);
			}
		}
	}
	//b.render();
	b2.render();
	w.render(viewingMatrix, ProjectionMatrix, skybox.cubemapTexture, cameraPos);

	glUseProgram(terrainShader->handle());  // use the shader
	glUniformMatrix4fv(glGetUniformLocation(terrainShader->handle(), "ProjectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(terrainShader->handle(), "ViewingMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);
	glUniform1f(glGetUniformLocation(terrainShader->handle(), "terraceHeight"), NoiseChunk::terraceHeight);
	glUniform1f(glGetUniformLocation(terrainShader->handle(), "waterHeight"), App::waterHeight);
	glUniform3fv(glGetUniformLocation(terrainShader->handle(), "cameraPos"), 1, &cameraPos[0]);
	for (int i = 0; i < chunksAmount; i++)
	{
		for (int j = 0; j < chunksAmount; j++)
		{
			if (terrainRenderStatus[i][j])
			{
				terrainGenerator[i][j].render();
				//glUseProgram(myBasicShader->handle());  // use the shader
				//glUniformMatrix4fv(glGetUniformLocation(myBasicShader->handle(), "ProjectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
				//glUniformMatrix4fv(glGetUniformLocation(myBasicShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

				//terrainGenerator[i][j].model.drawBoundingBox(myBasicShader);
				//terrainGenerator[i][j].model.drawOctreeLeaves(myBasicShader);
			}			
		}
	}

	skybox.render(viewingMatrix, ProjectionMatrix);

	glFlush();
}

void reshape(int width, int height)		// Resize the OpenGL window
{
	App::screenWidth=width; App::screenHeight = height;           // to ensure the mouse coordinates match 
														// we will use these values to set the coordinate system

	glViewport(0,0,width,height);						// Reset The Current Viewport

	//Set the projection matrix
	ProjectionMatrix = glm::perspective(60.0f, (GLfloat)App::screenWidth/(GLfloat)App::screenHeight, 1.0f, 50000.0f);
}

void init()
{
	SetThreadPriority(GetCurrentThread(), REALTIME_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

	glClearColor(0.2, 0.4, 0.65,0.0);						//sets the clear colour to yellow	
	glClearColor(0, 0, 0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
															//glClear(GL_COLOR_BUFFER_BIT) in the display function
														//will clear the buffer to this colour
	glEnable(GL_DEPTH_TEST);

	myShader = new Shader;
	//if(!myShader->load("BasicView", "glslfiles/basicTransformationsWithDisplacement.vert", "glslfiles/basicTransformationsWithDisplacement.frag"))
    if(!myShader->load("BasicView", "glslfiles/basicTransformations.vert", "glslfiles/basicTransformations.frag"))
	{
		cout << "failed to load shader" << endl;
	}		

	myBasicShader = new Shader;
	if(!myBasicShader->load("Basic", "glslfiles/basic.vert", "glslfiles/basic.frag"))
	{
		cout << "failed to load shader" << endl;
	}

	terrainShader = new Shader;
	if (!terrainShader->load("Terrain", "glslfiles/terrain.vert", "glslfiles/terrain.frag"))
	{
		cout << "failed to load shader" << endl;
	}

	skyboxShader = new Shader;
	if (!skyboxShader->load("skybox", "glslfiles/skybox.vert", "glslfiles/skybox.frag"))
	{
		cout << "failed to load shader" << endl;
	}

	waterShader = new Shader;
	if (!waterShader->load("water", "glslfiles/water.vert", "glslfiles/water.frag"))
	{
		cout << "failed to load shader" << endl;
	}

	waveShader = new Shader;
	if (!waveShader->load("wave", "glslfiles/wave.vert", "glslfiles/wave.frag"))
	{
		cout << "failed to load shader" << endl;
	}

	maxThreads = thread::hardware_concurrency();

	int x, y, dx, dy;
	x = y = dx = 0;
	dy = -1;
	int t = std::max(chunksAmount, chunksAmount);
	int maxI = t * t;
	for (int i = 0; i < maxI; i++) {
		if ((-chunksAmount / 2 <= x) && (x <= chunksAmount / 2) && (-chunksAmount / 2 <= y) && (y <= chunksAmount / 2)) {
			chunkQueue.push_back(glm::vec2(x + renderDist, y + renderDist));
		}
		if ((x == y) || ((x < 0) && (x == -y)) || ((x > 0) && (x == 1 - y))) {
			t = dx;
			dx = -dy;
			dy = t;
		}
		x += dx;
		y += dy;
	}
	reverse(chunkQueue.begin(), chunkQueue.end());
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	b.constructGeometry(myBasicShader, -chunkHalfSize, -chunkHalfSize, -chunkHalfSize, chunkHalfSize, chunkHalfSize, chunkHalfSize);
	b2.constructGeometry(waterShader, -waterRenderDist, -waterRenderDist, -waterRenderDist, waterRenderDist, App::waterHeight, waterRenderDist);
	player.init(&objLoader, myShader);
	w.constructGeometry(waveShader, -waterRenderDist, -waterRenderDist, waterRenderDist, waterRenderDist, App::waterHeight);
	skybox.constructGeometry(skyboxShader);
	
}

void update()
{
	updateThreads();
	updateRenderStatus();
	player.update();

	mouseDifference = glm::vec2(App::mouse_x, App::mouse_y) - mousePrevious;
	mousePrevious = glm::vec2(App::mouse_x, App::mouse_y);

	if (App::rightPressed)
	{
		camAngleX += mouseDifference.x * spinSpeed;
		camAngleY += -mouseDifference.y * spinSpeed;

		if (camAngleY > 1.2)
		{
			camAngleY = 1.2f;
		}

		if (camAngleY < -0.9)
		{
			camAngleY = -0.9f;
		}
	}

	if (App::keys[0x31])
	{
		camMode = 0;
	}
	if (App::keys[0x32])
	{
		camMode = 1;
	}
	if (App::keys[0x33])
	{
		camMode = 2;
	}
	collisionCount = 0;
	for (int i = 0; i < chunksAmount; i++)
	{
		for (int j = 0; j < chunksAmount; j++)
		{
			if (terrainRenderStatus[i][j])
			{
				doCollisions(player.model.octree, terrainGenerator[i][j].model.octree, &terrainGenerator[i][j].model);
			}
		}
	}
	glm::mat4 g = glm::mat4(1.0);
	g = glm::translate(g, player.position);
	g = g * player.objectRotation;
	glm::vec4 a = g * glm::vec4(1.0, 0, 0, 1.0f);
	glm::vec4 b = g * glm::vec4(0,0,0, 1.0f);
	glm::vec4 c = g * glm::vec4(-1,0,0, 1.0f);

	//if (timer > 0.2)
	{
		cout << "collision count: " << collisionCount << "\n";
		App::printVec3(glm::vec3(a));
		App::printVec3(glm::vec3(b));
		App::printVec3(glm::vec3(c));
		timer = 0;
	}
	timer += App::deltaTime;
}

void doCollisions(Octree* playerOct, Octree* terrainOct, ThreeDModel* terrainModel)
{
	// create two obbs
	OBB A, B;

	// set the half size
	A.Half_size.x = (playerOct->maxX - playerOct->minX) / 2.0f;
	A.Half_size.y = (playerOct->maxY - playerOct->minY) / 2.0f;
	A.Half_size.z = (playerOct->maxZ - playerOct->minZ) / 2.0f;

	// set the first obb's properties
	A.Pos = { (float)playerOct->minX + A.Half_size.x + player.position.x, (float)playerOct->minY + A.Half_size.y + player.position.y, (float)playerOct->minZ + A.Half_size.z + player.position.z}; // set its center position

	// set the axes orientation
	//A.AxisX = { 1.f, 0.f, 0.f };
	//A.AxisY = { 0.f, 1.f, 0.f };
	//A.AxisZ = { 0.f, 0.f, 1.f };
	A.AxisX = { player.objectRotation[0][0], player.objectRotation[0][1], player.objectRotation[0][2] };
	A.AxisY = { player.objectRotation[1][0], player.objectRotation[1][1], player.objectRotation[2][2] };
	A.AxisZ = { player.objectRotation[2][0], player.objectRotation[2][1], player.objectRotation[1][2] };

	// set the half size
	B.Half_size.x = (terrainOct->maxX - terrainOct->minX) / 2.0f;
	B.Half_size.y = (terrainOct->maxY - terrainOct->minY) / 2.0f;
	B.Half_size.z = (terrainOct->maxZ - terrainOct->minZ) / 2.0f;

	// set the second obb's properties
	B.Pos = { (float)terrainOct->minX + B.Half_size.x, (float)terrainOct->minY + B.Half_size.y, (float)terrainOct->minZ + B.Half_size.z }; // set its center position

	// set the axes orientation
	B.AxisX = { 1.f, 0.f, 0.f };
	B.AxisY = { 0.f, 1.f, 0.f };
	B.AxisZ = { 0.f, 0.f, 1.f };

	//cout << "start\n";
	//cout << A.Half_size.x << ", " << A.Half_size.y  << ", " << A.Half_size.z << ", " << A.Pos.x << ", " << A.Pos.y << ", " << A.Pos.z << "\n";
	//cout << B.Half_size.x << ", " << B.Half_size.y << ", " << B.Half_size.z << ", " << B.Pos.x << ", " << B.Pos.y << ", " << B.Pos.z << "\n";
	if (getCollision(A, B))
	{
		if (playerOct->getLevel() >= MAX_DEPTH) //leaf
		{
			if (terrainOct->VertexListSize > 0)
			{
				for (int i = 0; i < playerOct->VertexListSize; i++)
				{	
					for (int j = 0; j < terrainOct->VertexListSize; j++)
					{
						//cout << "vertexlist\n";
						for (int k = 0; k < terrainOct->VertexListSize; k++)
						{
							//cout << terrainModel->numberOfVertices << "\n";
							//cout << terrainOct->VertexListSize << "\n";
							//cout << k << "\n";
							//cout << terrainOct->VertexList[k] << "\n";
							//cout << terrainModel->theVerts[terrainOct->VertexList[k]] << "\n";
							//cout << terrainModel->theVerts[terrainOct->VertexList[k] + 1] << "\n";
							//cout << terrainModel->theVerts[terrainOct->VertexList[k] + 2] << "\n";
						}
						float p1[3];
						float p2[3];
						float p3[3];

						float t1[3];
						float t2[3];
						float t3[3];

						if ((terrainOct->VertexList[j] - 2) % 3 != 0)
						{
							continue;
						}

						t1[0] = terrainModel->theVerts[terrainOct->VertexList[j] - 2][0];
						t1[1] = terrainModel->theVerts[terrainOct->VertexList[j] - 2][1];
						t1[2] = terrainModel->theVerts[terrainOct->VertexList[j] - 2][2];
						t2[0] = terrainModel->theVerts[terrainOct->VertexList[j] - 1][0];
						t2[1] = terrainModel->theVerts[terrainOct->VertexList[j] - 1][1];
						t2[2] = terrainModel->theVerts[terrainOct->VertexList[j] - 1][2];
						t3[0] = terrainModel->theVerts[terrainOct->VertexList[j]][0];
						t3[1] = terrainModel->theVerts[terrainOct->VertexList[j]][1];
						t3[2] = terrainModel->theVerts[terrainOct->VertexList[j]][2];

						glm::mat4 g = glm::mat4(1.0);
						g = glm::translate(g, player.position);
						g = g * player.objectRotation;

						p1[0] = player.model.theVerts[playerOct->VertexList[i]][0];
						p1[1] = player.model.theVerts[playerOct->VertexList[i]][1];
						p1[2] = player.model.theVerts[playerOct->VertexList[i]][2];
						p2[0] = player.model.theVerts[playerOct->VertexList[i] + 1][0];
						p2[1] = player.model.theVerts[playerOct->VertexList[i] + 1][1];
						p2[2] = player.model.theVerts[playerOct->VertexList[i] + 1][2];
						p3[0] = player.model.theVerts[playerOct->VertexList[i] + 2][0];
						p3[1] = player.model.theVerts[playerOct->VertexList[i] + 2][1];
						p3[2] = player.model.theVerts[playerOct->VertexList[i] + 2][2];

						glm::vec4 a = g * glm::vec4(p1[0], p1[1], p1[2], 1.0f);
						glm::vec4 b = g * glm::vec4(p2[0], p2[1], p2[2], 1.0f);
						glm::vec4 c = g * glm::vec4(p3[0], p3[1], p3[2], 1.0f);

						p1[0] = a.x;
						p1[1] = a.y;
						p1[2] = a.z;
						p2[0] = b.x;
						p2[1] = b.y;
						p2[2] = b.z;
						p3[0] = c.x;
						p3[1] = c.y;
						p3[2] = c.z;

						if (IntersectionTests::NoDivTriTriIsect(p1, p2, p3, t1, t2, t3) == 1)
						{
							collisionCount++;
							Vector3d n = terrainModel->theFaceNormals[j];
							glm::vec3 normal = glm::vec3(n.x, n.y, n.z);
							player.velocity = glm::reflect(player.velocity, normal);
							player.position += player.velocity * (float)App::deltaTime;
							//cout << "tri1\n";
							//App::printVec3(glm::vec3(a));
							//App::printVec3(glm::vec3(b));
							//App::printVec3(glm::vec3(c));
							//cout << t1[0] << ", " << t1[1] << ", " << t1[2] << "\n";
							//cout << t2[0] << ", " << t2[1] << ", " << t2[2] << "\n";
							//cout << t3[0] << ", " << t3[1] << ", " << t3[2] << "\n";
							//cout << terrainOct->VertexList[j] - 2 << ", " << terrainOct->VertexList[j] - 1 << ", " << terrainOct->VertexList[j] << "\n";
						}


						/*if (IntersectionTests::tri_tri_overlap_test_3d(p1, p2, p3, t1, t2, t3) == 1)
						{
							collisionCount++;
							Vector3d n = terrainModel->theFaceNormals[j];
							glm::vec3 normal = glm::vec3(n.x, n.y, n.z);
							player.velocity = glm::reflect(player.velocity, normal);
							player.position += player.velocity * (float)App::deltaTime;
							cout << "tri1\n";
							App::printVec3(glm::vec3(a));
							App::printVec3(glm::vec3(b));
							App::printVec3(glm::vec3(c));
							cout << t1[0] << ", " << t1[1] << ", " << t1[2] << "\n";
							cout << t2[0] << ", " << t2[1] << ", " << t2[2] << "\n";
							cout << t3[0] << ", " << t3[1] << ", " << t3[2] << "\n";

							float d1[3] = { -4.29521, -8.50098, -1.07187 };
							float d2[3] = { -4.15965, -8.54044, -1.11133 };
							float d3[3] = { -4.5, -12, -0.753403 };

							float q1[3] = { -4.5, -12, -0.753403 };
							float q2[3] = { -4.5, -12, -0.753403 };
							float q3[3] = { -4.5, -10.8124, -4.5 };

							int u = 0;
							float h[3] = { 0,0,0 };
							if (IntersectionTests::tri_tri_overlap_test_3d(d1, q1, d2, q2, d3, q3) == 1)
							{
								cout << "yesyes\n";
							}
							else
							{
								cout << "no\n";
							}
						}*/
						//glm::vec3(-4.71739, -8.26703, -0.837914)
							//- 4.29521, -8.50098, -1.07187
							//- 4.15965, -8.54044, -1.11133
							//- 4.5, -12, -0.753403
							//- 4.5, -12, -0.753403
							//- 4.5, -10.8124, -4.5
					}
				}
			}
		}
		else
		{
			//For each oct in player
			for (int i = 0; i < 8; i++)
			{
				//If valid player oct
				if (playerOct->children[i] != NULL)
				{
					//For each oct in terrain
					for (int j = 0; j < 8; j++)
					{
						//If valid terrain oct
						if (terrainOct->children[j] != NULL)
						{
							doCollisions(playerOct->children[i], terrainOct->children[j], terrainModel);
						}
					}
				}
			}
		}
	}
}

void genNewChunk(int i, int j, bool* finished)
{
	//SetThreadAffinityMask(GetCurrentThread(), 1 << 12);
	terrainGenerator[i][j].genTerrain(terrainShader, glm::vec3(i, 0, j) - chunkOffset);
	terrainGenStatus[i][j] = true;
	for (int i = 0; i < threads.size(); i++)
	{
		if (threads[i].thread.get_id() == this_thread::get_id())
		{
			threads[i].finished = true;
		}
	}
}

void updateRenderStatus()
{
	for (int i = 0; i < chunksAmount; i++)
	{
		for (int j = 0; j < chunksAmount; j++)
		{
			if (terrainGenStatus[i][j])
			{
				if (!terrainRenderStatus[i][j])
				{
					terrainGenerator[i][j].applyTerrain();
					terrainRenderStatus[i][j] = true;
				}
			}
		}
	}
}

void updateThreads()
{
	while (threads.size() < maxThreads && chunkQueue.size() > 0)
	{
		glm::vec2 chunk = chunkQueue.back();
		chunkQueue.pop_back();
		App::terrainThread t;
		threads.push_back(std::move(t));
		threads.back().thread = thread(genNewChunk, chunk.x, chunk.y, &(threads.back().finished));
	}

	for (int i = 0; i < threads.size(); i++)
	{
		if (threads[i].finished)
		{
			threads[i].thread.join();
			threads.erase(threads.begin() + i);
		}
	}
}

// check if there's a separating plane in between the selected axes
bool getSeparatingPlane(const vec3& RPos, const vec3& Plane, const OBB& box1, const OBB&box2)
{
	return (fabs(RPos*Plane) >
		(fabs((box1.AxisX*box1.Half_size.x)*Plane) +
			fabs((box1.AxisY*box1.Half_size.y)*Plane) +
			fabs((box1.AxisZ*box1.Half_size.z)*Plane) +
			fabs((box2.AxisX*box2.Half_size.x)*Plane) +
			fabs((box2.AxisY*box2.Half_size.y)*Plane) +
			fabs((box2.AxisZ*box2.Half_size.z)*Plane)));
}

// test for separating planes in all 15 axes
bool getCollision(const OBB& box1, const OBB&box2)
{
	static vec3 RPos;
	RPos = box2.Pos - box1.Pos;

	return !(getSeparatingPlane(RPos, box1.AxisX, box1, box2) ||
		getSeparatingPlane(RPos, box1.AxisY, box1, box2) ||
		getSeparatingPlane(RPos, box1.AxisZ, box1, box2) ||
		getSeparatingPlane(RPos, box2.AxisX, box1, box2) ||
		getSeparatingPlane(RPos, box2.AxisY, box1, box2) ||
		getSeparatingPlane(RPos, box2.AxisZ, box1, box2) ||
		getSeparatingPlane(RPos, box1.AxisX^box2.AxisX, box1, box2) ||
		getSeparatingPlane(RPos, box1.AxisX^box2.AxisY, box1, box2) ||
		getSeparatingPlane(RPos, box1.AxisX^box2.AxisZ, box1, box2) ||
		getSeparatingPlane(RPos, box1.AxisY^box2.AxisX, box1, box2) ||
		getSeparatingPlane(RPos, box1.AxisY^box2.AxisY, box1, box2) ||
		getSeparatingPlane(RPos, box1.AxisY^box2.AxisZ, box1, box2) ||
		getSeparatingPlane(RPos, box1.AxisZ^box2.AxisX, box1, box2) ||
		getSeparatingPlane(RPos, box1.AxisZ^box2.AxisY, box1, box2) ||
		getSeparatingPlane(RPos, box1.AxisZ^box2.AxisZ, box1, box2));
}

/**************** END OPENGL FUNCTIONS *************************/

//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow();									// releases and destroys the window
bool CreateGLWindow(char* title, int width, int height); //creates the window
int WINAPI WinMain(	HINSTANCE, HINSTANCE, LPSTR, int);  // Win32 main function

//win32 global variabless
HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


/******************* WIN32 FUNCTIONS ***************************/
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	bool	done=false;								// Bool Variable To Exit Loop

	AllocConsole();
	FILE* stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);

	// Create Our OpenGL Window
	if (!CreateGLWindow("OpenGL Win32 Example",App::screenWidth,App::screenHeight))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if(App::keys[VK_ESCAPE])
				done = true;
			
			display();					// Draw The Scene
			update();					// update variables
			SwapBuffers(hDC);				// Swap Buffers (Double Buffering)

			App::deltaTime = double(duration_cast<duration<double>>(steady_clock::now() - totalFrameTime).count()) * timeScale;

			//Limit fps to prevent cpu being hogged for unneccesarily large fps
			if (timeScale / App::deltaTime > maxFps)
			{
				double timeToWait = (1.0 / maxFps) - App::deltaTime * 1.0 / timeScale;
				while (timeToWait > 0.0)
				{
					std::this_thread::sleep_for(std::chrono::nanoseconds{ 0 });
					App::deltaTime = double(duration_cast<duration<double>>(steady_clock::now() - totalFrameTime).count());
					timeToWait = (1.0 / maxFps) - App::deltaTime * 1.0 / timeScale;
				}
			}

			App::deltaTime = double(duration_cast<duration<double>>(steady_clock::now() - totalFrameTime).count()) * timeScale;
			totalFrameTime = steady_clock::now();

			//Force delta time to be less than max frame time
			if (App::deltaTime > maxFrameTime)
			{
				App::deltaTime = maxFrameTime;
			}
		}
	}

	//Kill remaining threads
	for(int i = 0; i < threads.size(); i++)
	{
		threads[i].thread.join();
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}
		break;

		case WM_SIZE:								// Resize The OpenGL Window
		{
			reshape(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
		break;

		case WM_RBUTTONDOWN:
		{
			App::mouse_x = LOWORD(lParam);
			App::mouse_y = App::screenHeight - HIWORD(lParam);
			App::rightPressed = true;
		}
		break;

		case WM_RBUTTONUP:
		{
			App::rightPressed = false;
		}
		break;

		case WM_LBUTTONDOWN:
			{
	            App::mouse_x = LOWORD(lParam);          
				App::mouse_y = App::screenHeight - HIWORD(lParam);
				App::LeftPressed = true;
			}
		break;

		case WM_LBUTTONUP:
			{
				App::LeftPressed = false;
			}
		break;

		case WM_MOUSEMOVE:
			{
				App::mouse_x = LOWORD(lParam);
				App::mouse_y = App::screenHeight  - HIWORD(lParam);
			}
		break;
		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			App::keys[wParam] = true;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}
		break;
		case WM_KEYUP:								// Has A Key Been Released?
		{
			App::keys[wParam] = false;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}
		break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*/
 
bool CreateGLWindow(char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}
	
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	HGLRC tempContext = wglCreateContext(hDC);
	wglMakeCurrent(hDC, tempContext);

	glewInit();

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		0
	};
	
    if(wglewIsSupported("WGL_ARB_create_context") == 1)
    {
		hRC = wglCreateContextAttribsARB(hDC,0, attribs);
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(tempContext);
		wglMakeCurrent(hDC, hRC);
	}
	else
	{	//It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
		hRC = tempContext;
		cout << " not possible to make context "<< endl;
	}

	//Checking GL version
	const GLubyte *GLVersionString = glGetString(GL_VERSION);

	cout << GLVersionString << endl;

	//OpenGL 3.2 way of checking the version
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

	cout << OpenGLVersion[0] << " " << OpenGLVersion[1] << endl;

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	reshape(width, height);					// Set Up Our Perspective GL Screen

	init();
	
	return true;									// Success
}



