#include <iostream>
#include <cmath>
#include <array>

using namespace std;

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "modelAnim.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacion();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.5f, 0.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float rot = 0.0f;


// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
//Posicion inicial del monito k frames
glm::vec3 PosIni(2.4544f, 0.23264f, 6.2022f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

bool active;


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Keyframes
//Inicializo los valores de k frames
float posX =PosIni.x, posY = PosIni.y, posZ = PosIni.z,rotY=90, rotPataFder = 0, rotPataTder = 0, 
rotPataFizq = 0, rotPataTizq = 0;

#define MAX_FRAMES 9
int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotY;
	float incRotY;
	float rotPataFder;
	float rotPataTder;
	float rotPataFizq;
	float rotPataTizq;
	float incRotPataFder;
	float incRotPataTder;
	float incRotPataFizq;
	float incRotPataTizq;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 8;			//introducir datos
bool play = false;
int playIndex = 0;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(posX,posY,posZ),
	glm::vec3(-10.0,-10.0,-10.0),
	glm::vec3(-10.0,-10.0,-10.0),
	glm::vec3(-10.0,-10.0,-10.0),
};

glm::vec3 LightP1;



//Guardando los valores de los frames
void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);
	
	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotPataFder = rotPataFder;
	KeyFrame[FrameIndex].rotPataTder = rotPataTder;
	KeyFrame[FrameIndex].rotPataFizq = rotPataFizq;
	KeyFrame[FrameIndex].rotPataTizq = rotPataTizq;
	
	KeyFrame[FrameIndex].rotY = rotY;

	/*cout << "posx:" << posX << " posy:" << posY << " posZ:" << posZ
		<< " rotPataFder" << rotPataFder << " rotPataTder" << rotPataTder
		<< " rotPataFizq" << rotPataFizq << " rotPataTizq" << rotPataTizq
		<< " rotY" << rotY << endl;*/
	

	FrameIndex++;
}
//para resetear las variables que manejan los frames
void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotPataFder = KeyFrame[0].rotPataFder;
	rotPataTder = KeyFrame[0].rotPataTder;
	rotPataFizq = KeyFrame[0].rotPataFizq;
	rotPataTizq = KeyFrame[0].rotPataTizq;

	rotY = KeyFrame[0].rotY;

}

void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;
	
	KeyFrame[playIndex].incRotPataFder = (KeyFrame[playIndex + 1].rotPataFder - KeyFrame[playIndex].rotPataFder) / i_max_steps;
	KeyFrame[playIndex].incRotPataTder = (KeyFrame[playIndex + 1].rotPataTder - KeyFrame[playIndex].rotPataTder) / i_max_steps;
	KeyFrame[playIndex].incRotPataFizq = (KeyFrame[playIndex + 1].rotPataFizq - KeyFrame[playIndex].rotPataFizq) / i_max_steps;
	KeyFrame[playIndex].incRotPataTizq = (KeyFrame[playIndex + 1].rotPataTizq - KeyFrame[playIndex].rotPataTizq) / i_max_steps;

	KeyFrame[playIndex].incRotY = (KeyFrame[playIndex + 1].rotY - KeyFrame[playIndex].rotY) / i_max_steps;

}


void drawInPosition(vec3 position, Model Object, GLuint VAO, Shader shader, vec3 rotation)
{
	mat4 model = mat4(1);
	model = translate(model, position);
	model = rotate(model, radians(rotation.x), vec3(1.0f, 0.0f, 0.0f));
	model = rotate(model, radians(rotation.y), vec3(0.0f, 1.0f, 0.0f));
	model = rotate(model, radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));
	glBindVertexArray(VAO);
	GLint modelLoc = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	Object.Draw(shader);
}

void drawFromPivot(vec3 position, Model Object, GLuint VAO, Shader shader, mat4 Pivot, vec3 rotation)
{
	mat4 model = mat4(1);
	model = translate(Pivot, position);
	model = rotate(model, radians(rotation.y), vec3(0.0f, 1.0f, 0.0f));
	model = rotate(model, radians(rotation.x), vec3(1.0f, 0.0f, 0.0f));
	model = rotate(model, radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));
	glBindVertexArray(VAO);
	GLint modelLoc = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	Object.Draw(shader);
}

void drawTableTwoObjects(vec3 position, GLuint VAO, Shader shader, Model Object, Model Table, float orientation,vec3 offset, vec3 rotation1, vec3 rotation2) {
	//Table 
	mat4 modelMesa;
	mat4 model = mat4(1);
	modelMesa = translate(model, position);
	modelMesa = rotate(modelMesa, radians(orientation), vec3(0.0f, 1.0f, 0.0f));
	glBindVertexArray(VAO);
	GLint modelLoc = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMesa));
	Table.Draw(shader);

	//Object 1
	drawFromPivot(offset, Object, VAO, shader, modelMesa, rotation1);
	
	//Object 2
	drawFromPivot(vec3(offset.x, offset.y, -offset.z), Object, VAO, shader, modelMesa, rotation2);

}

struct Position
{
	vec3 coordinates;
	float orientation;
};

//anim darthVader
float rotVaderRightArm = 0.0f, rotCloneBody = 0.0f, posCloneBodyY = 0.0f;
bool playAnimDarth = false;

//anim han/greedo
float rotHanBodyY = 0.0f, rotGreedoBodyZ = 0.0f, proyectile1T = 0.0f, proyectile2T = 0.0f, posGreedoBodyY = 0.0f;
bool playAnimHan = false;


//anim extra2
float rotExtra2LeftArm = 0.0f, rotExtra2LeftHand = 0.0f;
bool playAnimExtra2 = false;

//anim r2d2
float rotR2D2Head = 0.0f;
bool playAnimR2D2;

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 12", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	printf("%f", glfwGetTime());

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.frag");


	/********************************************************************
	*																	*
	*						DECLARACION DE MODELOS						*	
	*																	*
	*********************************************************************/
	Model BotaDer((char*)"Models/Personaje/bota.obj");
	Model PiernaDer((char*)"Models/Personaje/piernader.obj");
	Model PiernaIzq((char*)"Models/Personaje/piernaizq.obj");
	Model Torso((char*)"Models/Personaje/torso.obj");
	Model BrazoDer((char*)"Models/Personaje/brazoder.obj");
	Model BrazoIzq((char*)"Models/Personaje/brazoizq.obj");
	Model Cabeza((char*)"Models/Personaje/cabeza.obj");


	//Cantina
	Model Cantina((char*)"Models/LegoStarWarsTCSModels/Locations/MosEisleyCantina/cantina.obj");

	//Banco
	Model Stool((char*)"Models/LegoStarWarsTCSModels/Forniture/Stool/Stool.obj");

	//Mesas
	Model Table((char*)"Models/LegoStarWarsTCSModels/Forniture/Table/table.obj");

	//Sillon
	Model Couch((char*)"Models/LegoStarWarsTCSModels/Forniture/Couch/couch.obj");

	//Arbol de navidad
	Model ChristmasTree((char*)"Models/LegoStarWarsTCSModels/Forniture/ChristmasTree/ChristmasTree.obj");
	/*********************************
			Personajes
	*********************************/

	//Extra1
	Model Extra1((char*)"Models/LegoStarWarsTCSModels/Characters/LegoExtra1/Extra1Seated.obj");

	//Extra2
	Model Extra2Body((char*)"Models/LegoStarWarsTCSModels/Characters/LegoExtra2/Extra2SeatedBody.obj");
	Model Extra2LeftArm((char*)"Models/LegoStarWarsTCSModels/Characters/LegoExtra2/Extra2SeatedLeftArm.obj");
	Model Extra2LeftHand((char*)"Models/LegoStarWarsTCSModels/Characters/LegoExtra2/Extra2SeatedLeftHand.obj");

	//Extra3
	Model Extra3((char*)"Models/LegoStarWarsTCSModels/Characters/LegoExtra3/Extra3Seated.obj");

	//Extra4
	Model Extra4((char*)"Models/LegoStarWarsTCSModels/Characters/LegoExtra4/Extra4Seated.obj");

	//Extra5
	Model Extra5((char*)"Models/LegoStarWarsTCSModels/Characters/LegoExtra5/Extra5Seated.obj");

	//Barman
	Model Barman((char*)"Models/LegoStarWarsTCSModels/Characters/Barman/Barman.obj");

	//Leia
	Model Leia((char*)"Models/LegoStarWarsTCSModels/Characters/Leia/LeiaSeated.obj");

	//C3PO
	Model C3PO((char*)"Models/LegoStarWarsTCSModels/Characters/C3PO/C3POSeated.obj");

	//R2D2
	Model R2D2Body((char*)"Models/LegoStarWarsTCSModels/Characters/R2D2/R2D2Body.obj");
	Model R2D2Head((char*)"Models/LegoStarWarsTCSModels/Characters/R2D2/R2D2Head.obj");

	//Jawa
	Model Jawa((char*)"Models/LegoStarWarsTCSModels/Characters/Jawa/Jawa.obj");

	//StormTrooper
	Model StormTrooper((char*)"Models/LegoStarWarsTCSModels/Characters/StormTrooper/StormTrooperSeated.obj");

	// HanSolo
	Model HanSolo((char*)"Models/LegoStarWarsTCSModels/Characters/HanSolo/HanSoloAiming.obj");

	//Greedo
	Model Greedo((char*)"Models/LegoStarWarsTCSModels/Characters/Greedo/GreedoAiming.obj");

	//LukeSkyWalker
	Model LukeSkyWalker((char*)"Models/LegoStarWarsTCSModels/Characters/LukeSkyWalker/LukeSkyWalker.obj");

	//BenKenobi
	Model BenKenobi((char*)"Models/LegoStarWarsTCSModels/Characters/BenKenobi/BenKenobiSaber.obj");

	//DarthVader
	Model DarthVaderBody((char*)"Models/LegoStarWarsTCSModels/Characters/DarthVader/DarthVaderBody.obj");
	Model DarthVaderRightArm((char*)"Models/LegoStarWarsTCSModels/Characters/DarthVader/DarthVaderRightArm.obj");


	//StormTrooper
	Model StormTrooperMounted((char*)"Models/LegoStarWarsTCSModels/Characters/StormTrooper/StormTrooperMounted.obj");

	//Chewbaca
	Model Chewbaca((char*)"Models/LegoStarWarsTCSModels/Characters/Chewbaca/Chewbaca.obj");

	//Fantasma anakin
	Model AnakinGhost((char*)"Models/LegoStarWarsTCSModels/Characters/AnakinGhost/AnakinGhost.obj");


	/*********************************
			Elementos del entorno
	*********************************/
	//MugOrange
	Model MugOrange((char*)"Models/LegoStarWarsTCSModels/Forniture/Mug/mugOrange.obj");

	//MugYellow
	Model MugYellow((char*)"Models/LegoStarWarsTCSModels/Forniture/Mug/mugYellow.obj");

	//MugRed
	Model MugRed((char*)"Models/LegoStarWarsTCSModels/Forniture/Mug/mugRed.obj");

	//LampBase
	Model LampBase((char*)"Models/LegoStarWarsTCSModels/Forniture/Lamp/LampBase.obj");
	
	//Lamp
	Model Lamp((char*)"Models/LegoStarWarsTCSModels/Forniture/Lamp/Lamp1.obj");

	//CupRed
	Model CupRed((char*)"Models/LegoStarWarsTCSModels/Forniture/Cup/CupRed.obj");

	//CupGrey
	Model CupGrey((char*)"Models/LegoStarWarsTCSModels/Forniture/Cup/CupGrey.obj");

	//CupPurple
	Model CupPurple((char*)"Models/LegoStarWarsTCSModels/Forniture/Cup/CupPurple.obj");

	//Proyectile
	Model Proyectile((char*)"Models/LegoStarWarsTCSModels/Forniture/Proyectile/Proyectile.obj");

	/*********************************
			Vehiculos
	*********************************/

	//DewBack
	Model DewbackBody((char*)"Models/LegoStarWarsTCSModels/Vehichles/DewBack/DewbackBody.obj");

	Model DewbackFRightLeg((char*)"Models/LegoStarWarsTCSModels/Vehichles/DewBack/DewbackFRightLeg.obj");

	Model DewbackBRightLeg((char*)"Models/LegoStarWarsTCSModels/Vehichles/DewBack/DewbackBRightLeg.obj");

	Model DewbackFLeftLeg((char*)"Models/LegoStarWarsTCSModels/Vehichles/DewBack/DewbackFLeftLeg.obj");

	Model DewbackBLeftLeg((char*)"Models/LegoStarWarsTCSModels/Vehichles/DewBack/DewbackBLeftLeg.obj");

	////Modelo de animaci�n
	//ModelAnim animacionPersonaje("Models/LegoStarWarsTCSModels/Characters/StormTrooper/StormTrooper.dae");
	//animacionPersonaje.initShaders(animShader.Program);

	float animDewback [MAX_FRAMES][8] = {
		{2.4540f,0.23264f,6.2022f,0.0f,0.0f,0.0f,0.0f,90.0f},
		{3.0544f,0.23264f,6.2022f,-12.0f,7.0f,17.0f,-9.0f,95.0f},
		{3.55439f,0.23264f,6.0022f,7.99998f,-10.0f,-8.99999f,13.0f,117.0f},
		{4.15439f,0.23264f,5.1022f,-21.0f,12.0f,7.0f,-15.0f,126.0f},
		{4.95437f,0.23264f,4.40215f,3.9994f,29.0f,-26.0f,4.0f,144.999f},
		{5.95437f,0.23264f,3.30215f,-35.0001f,43.0f,-3.999f,5.99998f,151.999f},
		{6.05437f,0.23264f,2.60215f,-39.0001f,-14.0f,-46.0f,-13.0f,169.999f},
		{6.05441f,0.23264f,1.90208f,-0.0f,34.0f,-8.0f,33.0f,169.997f},
		{6.05441f,0.23264f,0.9020082f,-44.0f,23.0f,-33.0f,-7.0f,169.997f},
	};
	//Inicializaci�n de KeyFrames
	//aqui se cargan
	for(int i=0; i<MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = animDewback[i][0];
		KeyFrame[i].posY = animDewback[i][1];
		KeyFrame[i].posZ = animDewback[i][2];

		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;

		KeyFrame[i].rotPataFder = animDewback[i][3];
		KeyFrame[i].rotPataTder = animDewback[i][4];
		KeyFrame[i].rotPataFizq = animDewback[i][5];
		KeyFrame[i].rotPataTizq = animDewback[i][6];
		KeyFrame[i].rotY = animDewback[i][7];

		KeyFrame[i].incRotPataFder = 0;
		KeyFrame[i].incRotPataTder = 0;
		KeyFrame[i].incRotPataFizq = 0;
		KeyFrame[i].incRotPataTizq = 0;
		KeyFrame[i].incRotY = 0;
	}



	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};


	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	//Posiciones de los bancos
	array<vec3,14> stoolPositions = {
		vec3(-0.764f, 0.027f, -0.929f),
		vec3(-1.23f, 0.027f, -0.929f),
		vec3(-1.23f, 0.027f, -0.929f),
		vec3(-1.68f, 0.027f, -0.929f),
		vec3(-2.1526f, 0.027f, -0.929f),
		vec3(-2.6063f, 0.027f, -0.929f),
		vec3(-3.0743f, 0.027f, -0.929f),
		vec3(-3.0460f, 0.027f, 0.55201f),
		vec3(-3.0460f, 0.027f, 0.55201f),
		vec3(-2.6058f, 0.027f, 0.55201f),
		vec3(-2.1471f, 0.027f, 0.55201f),
		vec3(-1.6814f, 0.027f, 0.55201f),
		vec3(-1.2191f, 0.027f, 0.55201f),
		vec3(-0.77156f, 0.027f, 0.55201f)
	};

	//Posici�n de mesas con 2 objetos
	array<Position, 6> tablePositions = {
		Position { vec3(-2.6678f, 0.14909f, -2.1579f),0.0f },
		Position { vec3(-1.0947f, 0.14909f, -2.042f),90.0f },
		Position { vec3(0.8282f, 0.14909f, -1.7235f),45.0f },
		Position { vec3(1.91f, 0.14909f, -0.2404f),0.0f },
		Position { vec3(0.97f, 0.14909f, 1.41f),-45.0f },
		Position { vec3(-0.83f, 0.14909f, 1.55f),90.0f },
		//Position { vec3(-3.6071f, 0.14909f, 1.5123f),0.0f }
	};
	array<Position, 7> tableCouchPositions = {
		Position { vec3(-2.3678f, 0.14909f, -4.061f),0.0f },
		Position { vec3(0.5606f, 0.14909f, -3.6348f),90.0f },
		Position { vec3(2.7052f, 0.14909f, -3.8304f),-20.0f },
		Position { vec3(4.25454f, 0.14909f, -2.7174f),-65.0f },
		Position { vec3(3.933f, 0.14909f, 2.2377f),135.0f },
		Position { vec3(2.501f, 0.14909f, 3.4209f),135.0f },
		Position { vec3(0.364891f, 0.14909f, 3.7824f),90.0f }
	};
	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1,&skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),&skyboxVertices,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/leftDessert.png");
	faces.push_back("SkyBox/rightDessert.png");
	faces.push_back("SkyBox/topDessert.png");
	faces.push_back("SkyBox/bottomDessert.png");
	faces.push_back("SkyBox/backDessert.png");
	faces.push_back("SkyBox/frontDessert.png");
	
	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();


		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.3f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);


		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), LightP1.x, LightP1.y, LightP1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), LightP1.x, LightP1.y, LightP1.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);



		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 1.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();


		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Bind diffuse map
		//glBindTexture(GL_TEXTURE_2D, texture1);*/

		// Bind specular map
		/*glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);*/


		glBindVertexArray(VAO);
		glm::mat4 tmp = glm::mat4(1.0f); //Temp


		/*********************************
		*								 *
		*		CARGA DE MODELOS		 *
		*								 *
		**********************************/
		//Dewback
		mat4 model(1);
		mat4 DewbackBodyMatrix = model;
		DewbackBodyMatrix = translate(DewbackBodyMatrix, glm::vec3(posX, posY, posZ));
		DewbackBodyMatrix = rotate(DewbackBodyMatrix, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(DewbackBodyMatrix));
		DewbackBody.Draw(lightingShader);


		////Pierna frontal derecha
		model = translate(DewbackBodyMatrix, glm::vec3(-0.15935f, 0.013264f, 0.43353f));
		model = rotate(model, glm::radians(rotPataFder), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DewbackFRightLeg.Draw(lightingShader);

		////Pierna frontal izquierda
		model = translate(DewbackBodyMatrix, glm::vec3(0.13065f, 0.003264f, 0.44353f));
		model = rotate(model, glm::radians(rotPataFizq), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DewbackFLeftLeg.Draw(lightingShader);

		////Pierna trasera derecha
		model = translate(DewbackBodyMatrix, glm::vec3(-0.15935f, -0.006736f, -0.046474f));
		model = rotate(model, glm::radians(rotPataTder), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DewbackBRightLeg.Draw(lightingShader);

		////Pierna trasera izquierda
		model = translate(DewbackBodyMatrix, glm::vec3(0.17395f, 0.005927f, -0.022154f));
		model = rotate(model, glm::radians(rotPataTizq), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DewbackBLeftLeg.Draw(lightingShader);

		//Cantina
		model = glm::mat4(1);
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cantina.Draw(lightingShader);

		/*
		 * Bancos de la barra
		*/
		for (int i = 0; i < stoolPositions.size(); i++) {
			drawInPosition(stoolPositions[i], Stool, VAO, lightingShader, vec3(0.0f));
		}

		/*******************************************

					Carga mesas con banco

		*******************************************/

		for (int i = 0; i < tablePositions.size(); i++) {
			drawTableTwoObjects(tablePositions[i].coordinates, VAO, lightingShader,
				Stool, Table, tablePositions[i].orientation, vec3(0.0f, -0.16f, 0.27f), vec3(0.0f), vec3(0.0f));
		}

		/*******************************************

				 Carga mesas con sillion

		*******************************************/

		for (int i = 0; i < tableCouchPositions.size(); i++) {
			drawTableTwoObjects(tableCouchPositions[i].coordinates, VAO, lightingShader,
				Couch, Table, tableCouchPositions[i].orientation, vec3(0.0f, -0.08f, -0.23f), vec3(0.0f), vec3(0.0f, 180.0f, 0.0f));
		}

		/*******************************************

				 Carga Personajes

		*******************************************/
		//Extra1
		drawInPosition(vec3(-1.330f, 0.2368f, -2.061f), Extra1, VAO, lightingShader, vec3(0.0f, 90.0f, 0.0f));
		//Extra2
		mat4 Extra2Matrix = mat4(1);
		Extra2Matrix = translate(Extra2Matrix, vec3(-0.61805f, 0.2368f, 1.5148f));
		Extra2Matrix = rotate(Extra2Matrix, radians(-90.0f), vec3(0.0f, 1.0f, 0.0f));
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Extra2Matrix));
		Extra2Body.Draw(lightingShader);

		mat4 Extra2Arm = mat4(1);
		Extra2Arm = translate(Extra2Matrix, vec3(0.036248f, 0.04077f, -0.006224f));
		Extra2Arm = rotate(Extra2Arm, radians(rotExtra2LeftArm), vec3(1.0f, 0.0f, 0.0f));
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Extra2Arm));
		Extra2LeftArm.Draw(lightingShader);

		mat4 Extra2Hand = mat4(1);
		Extra2Hand = translate(Extra2Arm, vec3(0.054413f, -0.016304f, 0.058673));
		Extra2Hand = rotate(Extra2Hand, radians(rotExtra2LeftHand), vec3(0.0f, 0.0f, 1.0f));
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Extra2Hand));
		Extra2LeftHand.Draw(lightingShader);

		//Extra3
		drawInPosition(vec3(-1.6852f, 0.2368f, -0.8967f), Extra3, VAO, lightingShader, vec3(0.0f, 0.0f, 0.0f));
		//Extra4
		drawInPosition(vec3(-0.77857f, 0.2368f, -0.91304f), Extra4, VAO, lightingShader, vec3(0.0f, 0.0f, 0.0f));
		//Extra5
		drawInPosition(vec3(-2.65032f, 0.2368f, 0.5107f), Extra5, VAO, lightingShader, vec3(0.0f, 180.0f, 0.0f));
		//Barman
		drawInPosition(vec3(-0.30177f, 0.16282f, -0.1548f), Barman, VAO, lightingShader, vec3(0.0f, 90.0f, 0.0f));
		//Leia
		drawInPosition(vec3(0.60853f, 0.1879f, 3.7939f), Leia, VAO, lightingShader, vec3(0.0f, -90.0f, 0.0f));
		//C3PO
		drawInPosition(vec3(0.1072f, 0.1474f, 3.8105f), C3PO, VAO, lightingShader, vec3(0.0f, 90.0f, 0.0f));
		//R2D2
		mat4 R2D2Matrix = mat4(1);
		R2D2Matrix = translate(R2D2Matrix, vec3(0.3168f, 0.1293f, 4.1478f));
		R2D2Matrix = rotate(R2D2Matrix, radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(R2D2Matrix));
		R2D2Body.Draw(lightingShader);
		
		model = mat4(1);
		model = translate(R2D2Matrix, vec3(0.001126f, 0.195f, -0.007995f));
		model = rotate(model, radians(rotR2D2Head), vec3(0.0f, 1.0f, 0.0f));
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		R2D2Head.Draw(lightingShader);

		//Jawa
		drawInPosition(vec3(-1.1961f, 0.2859f, 0.4284f), Jawa, VAO, lightingShader, vec3(0.0f, 180.0f, 0.0f));
		//StormTrooper1
		drawInPosition(vec3(1.9167f, 0.23015f, -0.0039f), StormTrooper, VAO, lightingShader, vec3(0.0f, 180.0f, 0.0f));

		//StormTrooper2
		mat4 StormTrooperMatrix = mat4(1);
		StormTrooperMatrix = translate(StormTrooperMatrix, vec3(1.9167f, 0.23015f, -0.4909f));
		StormTrooperMatrix = translate(StormTrooperMatrix, vec3(0.0f, posCloneBodyY, 0.0f));
		StormTrooperMatrix = rotate(StormTrooperMatrix, radians(rotCloneBody), vec3(1.0f, 0.0f, 0.0f));
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(StormTrooperMatrix));
		StormTrooper.Draw(lightingShader);

		//Greedo
		mat4 GreedoMatrix = mat4(1);
		GreedoMatrix = translate(GreedoMatrix, vec3(3.5887f, 0.15107f, -2.8939f));
		GreedoMatrix = rotate(GreedoMatrix, radians(-90.0f), vec3(0.0f, 1.0f, 0.0f));
		GreedoMatrix = translate(GreedoMatrix, vec3(0.0f, posGreedoBodyY, 0.0f));
		GreedoMatrix = rotate(GreedoMatrix, radians(-rotGreedoBodyZ), vec3(1.0f, 0.0f, 0.0f));
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(GreedoMatrix));
		Greedo.Draw(lightingShader);
		
		//Proyectile2
		model = mat4(1);
		model = translate(model, vec3(3.4643f, 0.24266f, -2.9742f));
		model = rotate(model, radians(-90.0f), vec3(0.0f, 1.0f, 0.0f));
		model = translate(model, vec3(0.0f, 0.0f, proyectile2T));
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Proyectile.Draw(lightingShader);

		//Hansolo
		model = mat4(1);
		model = translate(model, vec3(3.0367f, 0.1959f, -2.9842f));
		model = rotate(model, radians(94.4f), vec3(0.0f, 1.0f, 0.0f));
		model = rotate(model, radians(-rotHanBodyY), vec3(0.0f, 1.0f, 0.0f));
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HanSolo.Draw(lightingShader);

		//Proyectile1
		model = mat4(1);
		model = translate(model, vec3(3.195f, 0.22943f, -2.8994f));
		model = rotate(model, radians(95.3f), vec3(0.0f, 1.0f, 0.0f));
		model = rotate(model, radians(-0.812f), vec3(0.0f, 0.0f, 1.0f));
		model = translate(model, vec3(0.0f, 0.0f, proyectile1T));
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Proyectile.Draw(lightingShader);
		
		//LukeSkyWalker
		drawInPosition(vec3(-0.63197f, 0.1861f, -3.639f), LukeSkyWalker, VAO, lightingShader, vec3(0.0f, 33.0f, 0.0f));
		//BenKenobi
		drawInPosition(vec3(-0.2029f, 0.16272f, -3.2736f), BenKenobi, VAO, lightingShader, vec3(0.0f, 42.2f, 0.0f));
		//DarthVader
		mat4 DarthVaderMatrix = mat4(1);
		DarthVaderMatrix = translate(DarthVaderMatrix, vec3(4.01859f, 0.1656f, -0.16975f));
		DarthVaderMatrix = rotate(DarthVaderMatrix, radians(-90.0f), vec3(0.0f, 1.0f, 0.0f));
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(DarthVaderMatrix));
		DarthVaderBody.Draw(lightingShader);

		model = mat4(1);
		model = translate(DarthVaderMatrix, vec3(-0.038212f, 0.054847f, -0.012091f));
		model = rotate(model, radians(rotVaderRightArm), vec3(1.0, 0.0, 0.0));
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DarthVaderRightArm.Draw(lightingShader);
	
		//StormTrooperMounted1
		drawInPosition(vec3(4.25247f, 0.18164f, -0.47043f), StormTrooperMounted, VAO, lightingShader, vec3(0.0f, -90.0f, 0.0f));
		//StormTrooperMounted2
		drawInPosition(vec3(4.25247f, 0.18164f, 0.2022f), StormTrooperMounted, VAO, lightingShader, vec3(0.0f, -90.0f, 0.0f));
		//Chewbaca
		drawInPosition(vec3(2.6819f, 0.18164f, 1.9075f), Chewbaca, VAO, lightingShader, vec3(0.0f, 26.1f, 0.0f));

		/*******************************************

				 Carga Tazas

		*******************************************/
		//MugOrange
		//drawInPosition(vec3(-3.5737f, 0.252157f, 1.5198f), MugOrange, VAO, lightingShader, vec3(0.0f, 0.0f, 0.0f));
		drawInPosition(vec3(0.5606f, 0.252157f, -3.6348f), MugOrange, VAO, lightingShader, vec3(0.0f, 0.0f, 0.0f));
		drawInPosition(vec3(2.5011f, 0.252157f,  3.4209f), MugOrange, VAO, lightingShader, vec3(0.0f, 90.0f, 0.0f));

		//MugYellow
		drawInPosition(vec3(-3.2165f, 0.28177f, 0.26859f), MugYellow, VAO, lightingShader, vec3(0.0f, 0.0f, 0.0f));
		drawInPosition(vec3(-2.657f, 0.252157f, -2.1797f), MugYellow, VAO, lightingShader, vec3(0.0f, 0.0f, 0.0f));
		drawInPosition(vec3(-0.78311f, 0.28177f, -0.67255f), MugYellow, VAO, lightingShader, vec3(0.0f, 90.0f, 0.0f));
		drawInPosition(vec3(4.2541f, 0.252157f, -2.7313f), MugYellow, VAO, lightingShader, vec3(0.0f, 0.0f, 0.0f));

		//MugRed
		drawInPosition(vec3(1.01f, 0.252157f, 1.3778f), MugRed, VAO, lightingShader, vec3(0.0f, 0.0f, 0.0f));
		drawInPosition(vec3(-1.1853f, 0.27062f, -2.1602f), MugRed, VAO, lightingShader, vec3(0.0f, 180.0f, -14.8f));

		/*******************************************

				 Base Lamparas

		*******************************************/
		//LampBase
		drawInPosition(vec3(-3.0212f, 0.82373f, -2.977f ), LampBase, VAO, lightingShader, vec3(0.0f, 90.0f, 0.0f));
		drawInPosition(vec3( 2.1736f, 0.82373f, -2.7845f), LampBase, VAO, lightingShader, vec3(0.0f, 47.9f, 0.0f));
		drawInPosition(vec3( 3.2908f, 0.82373f, -1.1816f), LampBase, VAO, lightingShader, vec3(0.0f, 0.0f, 0.0f));
		drawInPosition(vec3( 3.9499f, 0.82373f,  0.7824f), LampBase, VAO, lightingShader, vec3(0.0f, 0.0f, 0.0f));
		drawInPosition(vec3( 1.6387f, 0.82373f,  2.3833f), LampBase, VAO, lightingShader, vec3(0.0f, 270.0f, 0.0f));
		drawInPosition(vec3(-0.8515f, 0.82373f,  2.3833f), LampBase, VAO, lightingShader, vec3(0.0f, 270.0f, 0.0f));
		drawInPosition(vec3(-3.0212f, 0.82373f,  2.3833f), LampBase, VAO, lightingShader, vec3(0.0f, 90.0f, 0.0f));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		/*******************************************

				 Lamparas

		*******************************************/

		//Lamp
		drawInPosition(vec3(-3.0212f, 0.82373f, -2.977f), Lamp, VAO, lightingShader, vec3(0.0f, 90.0f, 0.0f));
		drawInPosition(vec3(2.1736f, 0.82373f, -2.7845f), Lamp, VAO, lightingShader, vec3(0.0f, 47.9f, 0.0f));
		drawInPosition(vec3(3.2908f, 0.82373f, -1.1816f), Lamp, VAO, lightingShader, vec3(0.0f, 0.0f, 0.0f));
		drawInPosition(vec3(3.9499f, 0.82373f, 0.7824f), Lamp, VAO, lightingShader, vec3(0.0f, 0.0f, 0.0f));
		drawInPosition(vec3(1.6387f, 0.82373f, 2.3833f), Lamp, VAO, lightingShader, vec3(0.0f, 270.0f, 0.0f));
		drawInPosition(vec3(-0.8515f, 0.82373f, 2.3833f), Lamp, VAO, lightingShader, vec3(0.0f, 270.0f, 0.0f));
		drawInPosition(vec3(-3.0212f, 0.82373f, 2.3833f), Lamp, VAO, lightingShader, vec3(0.0f, 90.0f, 0.0f));

		//CupRed
		drawInPosition(vec3(-2.3678f, 0.27079f, -4.1481f), CupRed, VAO, lightingShader, vec3(0.0f, 0.0f, 0.0f));
		drawInPosition(vec3( 3.872f, 0.02116f, -3.244f), CupRed, VAO, lightingShader, vec3(-90.0f, 0.0f, 0.0f));
		drawInPosition(vec3( 1.9174f, 0.28963f, -0.17249f), CupRed, VAO, lightingShader, vec3(0.0f, 0.0f, 0.0f));
		model = mat4(1);
		model = translate(Extra2Hand, vec3(0.007032f, 0.030808f, 0.042931f));
		model = rotate(model, radians(-30.0f), vec3(1.0f, 0.0f, 0.0f));
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CupRed.Draw(lightingShader);

		//CupGrey
		drawInPosition(vec3(1.9174f, 0.28963f, -0.30435f), CupGrey, VAO, lightingShader, vec3(0.0f, 0.0f, 0.0f));
		drawInPosition(vec3(3.933f, 0.28029f, 2.2377f), CupGrey, VAO, lightingShader, vec3(0.0f, 0.0f, 0.0f));
		drawInPosition(vec3(-2.6566f, 0.26212f, 0.24098f), CupGrey, VAO, lightingShader, vec3(-90.0f, 76.3f, 0.0f));

		//CupPurple
		drawInPosition(vec3(-2.3678f, 0.27079f, -3.9796f), CupPurple, VAO, lightingShader, vec3(0.0f, 0.0f, 0.0f));
		drawInPosition(vec3( 0.36489f, 0.27079f, 3.7824f), CupPurple, VAO, lightingShader, vec3(0.0f, 0.0f, 0.0f));
		drawInPosition(vec3( -1.3051f,0.31865f, 0.30181f), CupPurple, VAO, lightingShader, vec3(0.0f, 0.0f, 0.0f));
		drawInPosition(vec3( -1.5894f,0.31122f, -0.80489f), CupPurple, VAO, lightingShader, vec3(-32.16f, 0.0f, 0.0f));


		/***********************
		*	Anakin Fantasma     *
		************************/
		drawInPosition(vec3(1.4433f, 0.18776f, -2.2851f), AnakinGhost, VAO, lightingShader, vec3(0.0f, 33.9f, 0.0f));
		glDisable(GL_BLEND);

		/***************************
		*
		*	Carga Adornos navide�os
		*
		****************************/
		// ChristmasTree
		drawInPosition(vec3(-3.3525f, 0.31978f, 1.4678f), ChristmasTree, VAO, lightingShader, vec3(0.0f, 71.4f, 0.0f));
		/*_______________________________Personaje Animado___________________________*/ 
		/*animShader.Use();
		modelLoc = glGetUniformLocation(animShader.Program, "model");
		viewLoc = glGetUniformLocation(animShader.Program, "view");
		projLoc = glGetUniformLocation(animShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glUniform3f(glGetUniformLocation(animShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(animShader.Program, "material.shininess"), 32.0f);
		glUniform3f(glGetUniformLocation(animShader.Program, "light.ambient"), 0.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(animShader.Program, "light.diffuse"), 0.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(animShader.Program, "light.specular"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(animShader.Program, "light.direction"),0.0f, -1.0f, -1.0f);
		view = camera.GetViewMatrix();
		model = mat4(1);
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		animacionPersonaje.Draw(animShader);
*/







		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		


		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default




		// Swap the screen buffers
		glfwSwapBuffers(window);
	}




	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();




	return 0;
}

int stateAnimDarth = 1;
int stateAnimHan = 1;
int stateAnimExtra2 = 1;
int stateAnimR2D2 = 1;
void animacion()
{
	if (playAnimR2D2) {
		switch (stateAnimR2D2)
		{
		case 1:
			rotR2D2Head += 0.5;
			if (rotR2D2Head > 45.0f) {
				stateAnimR2D2 = 2;
				break;
			}
			break;
		case 2:
			rotR2D2Head -= 0.5f;
			if (rotR2D2Head < -45.0f) {
				stateAnimR2D2 = 1;
				break;
			}
			break;
		default:
			break;
		}
	}
	if (playAnimExtra2) {
		switch (stateAnimExtra2)
		{
		case 1:
			rotExtra2LeftHand += 0.5f;
			if (rotExtra2LeftHand > 40.0f) {
				stateAnimExtra2 = 2;
				break;
			}
			break;
		case 2:
			rotExtra2LeftArm -= 0.3f;
			if (rotExtra2LeftArm < -15.0f) {
				stateAnimExtra2 = 3;
				break;
			}
			break;
		case 3:
			rotExtra2LeftArm += 0.3f;
			if (rotExtra2LeftArm > 5.0f) {
				stateAnimExtra2 = 2;
				break;
			}
			break;
		default:
			break;
		}
	}
	if (playAnimHan) {
		switch (stateAnimHan)
		{
		case 1:
			proyectile1T += 0.01f;
			cout << " proyectile1T " << proyectile1T << endl;
			proyectile2T += 0.02f;
			rotHanBodyY -= 3.0f;
			if (proyectile1T > 0.4) {
				stateAnimHan = 2;
				break;
			}
			break;
		case 2:
			proyectile1T = 10.0f;
			proyectile2T = 10.0f; 
			rotGreedoBodyZ += 1.0f;
			posGreedoBodyY -= 0.001f;
			if (rotGreedoBodyZ > 90) {
				stateAnimHan = 0;
				break;
			}
			break;
		default:
			break;
		}
	}
	
	if (playAnimDarth) {

		switch (stateAnimDarth)
		{
		case 1:
			rotVaderRightArm -= 1.5f;
			posCloneBodyY += 0.01f;
			rotCloneBody += 1.0f;
			if (posCloneBodyY > 0.5) {
				stateAnimDarth = 2;
				break;
			}
			break;
		case 2:
			rotCloneBody = (rotCloneBody + 0.5f);
		default:
			break;
		}
	}

		//Movimiento del personaje

		if (play)
		{
			if (i_curr_steps >= i_max_steps) //end of animation between frames?
			{
				playIndex++;
				if (playIndex>FrameIndex - 2)	//end of total animation?
				{
					printf("termina anim\n");
					playIndex = 0;
					play = false;
				}
				else //Next frame interpolations
				{
					i_curr_steps = 0; //Reset counter
									  //Interpolation
					interpolation();
				}
			}
			else
			{
				//Draw animation
				posX += KeyFrame[playIndex].incX;
				posY += KeyFrame[playIndex].incY;
				posZ += KeyFrame[playIndex].incZ;

				rotPataFder += KeyFrame[playIndex].incRotPataFder;
				rotPataTder += KeyFrame[playIndex].incRotPataTder;
				rotPataFizq += KeyFrame[playIndex].incRotPataFizq;
				rotPataTizq += KeyFrame[playIndex].incRotPataTizq;
				rotY += KeyFrame[playIndex].incRotY;

				i_curr_steps++;
			}

		}
	}


// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex > 1))
		{

			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}

	}

	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex<MAX_FRAMES)
		{
			saveFrame();
		}

	}

	if (keys[GLFW_KEY_P])
	{
		playAnimDarth = !playAnimDarth;

	}

	if (keys[GLFW_KEY_O])
	{
		playAnimHan = !playAnimHan;

	}
	if (keys[GLFW_KEY_I])
	{
		playAnimExtra2 = !playAnimExtra2;

	}

	if (keys[GLFW_KEY_U])
	{
		playAnimR2D2 = !playAnimR2D2;

	}

	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
			LightP1 = glm::vec3(1.0f, 0.0f, 0.0f);
		else
			LightP1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	if (keys[GLFW_KEY_1])
	{

		rotY += 1;

	}
	
	if (keys[GLFW_KEY_2])
	{
		rotExtra2LeftArm += 1.0f;
		
	}

	if (keys[GLFW_KEY_3])
	{

		rotExtra2LeftArm -= 1.0f;
	}
	if (keys[GLFW_KEY_4])
	{
	
		rotExtra2LeftHand += 1.0f;
	}

	if (keys[GLFW_KEY_5])
	{
		rotExtra2LeftHand -= 1.0f;
	}
	if (keys[GLFW_KEY_6])
	{
		proyectile1T += 0.1f;
		cout << "proyectile1T = " << proyectile1T << endl;
	}

	if (keys[GLFW_KEY_7])
	{
		proyectile1T -= 0.1f;
		cout << "proyectile1T = " << proyectile1T << endl;
	}
	if (keys[GLFW_KEY_8])
	{
		proyectile2T += 0.1f;

	}

	if (keys[GLFW_KEY_9])
	{
		proyectile2T -= 0.1f;

	}


	

	//Mov Personaje
	if (keys[GLFW_KEY_H])
	{
		posZ += 0.1;
	}

	if (keys[GLFW_KEY_Y])
	{
		posZ -= 0.1;
	}

	if (keys[GLFW_KEY_G])
	{
		posX -= 0.1;
	}

	if (keys[GLFW_KEY_J])
	{
		posX += 0.1;
	}




	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}






}