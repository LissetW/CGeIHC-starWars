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
Camera  camera(vec3(0.0f, 0.5f, 0.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float rot = 0.0f;


// Light attributes
vec3 lightPos(0.0f, 0.0f, 0.0f);
//Posicion inicial del monito k frames
vec3 PosIni(2.4544f, 0.23264f, 6.2022f);
vec3 lightDirection(0.0f, -1.0f, -1.0f);

bool active;

//Variables para todos los frames
int i_max_steps = 95;
int i_curr_steps = 0;

/************************************
*									*
* VARIABLES KEYFRAMES ANAKIN GHOST	*
*									*
*************************************/
// Posicion incial anakin
vec3 PosIniAnakinG(1.4433f, 0.3221f, -2.2851f);
int i_curr_stepsAnakin = 0;

//Inicializo los valores de k frames
float posXAnakinG{ PosIniAnakinG.x },
	  posYAnakinG{ PosIniAnakinG.y },
	  posZAnakinG{ PosIniAnakinG.z },
	  rotBodyYAnakinG{ 33.9f },
	  rotBodyXAnakinG{ 0.0f },
	  rotLefTLegAnakinG{ 0.0f },
	  rotRightLegAnakinG{ 0.0f },
	  rotLeftArmAnakinG{ 0.0f },
	  rotRightArmAnakinG{ 0.0f },
	  rotRightWristAnakinG{ 0.0f },
	  rotLeftWristAnakinG{ 0.0f };

#define MAX_FRAMES_ANAKING 10

typedef struct _frameAnakin
{
	//Variables para GUARDAR Key Frames
	float posXAnakinG,
		  posYAnakinG,
		  posZAnakinG,
		  rotBodyXAnakinG,
		  rotBodyYAnakinG,
		  rotLefTLegAnakinG,
		  rotRightLegAnakinG,
		  rotLeftArmAnakinG,
		  rotRightArmAnakinG,
		  rotRightWristAnakinG,
		  rotLeftWristAnakinG;
	//Incrementos
	float incPosXAnakinG,
		  incPosYAnakinG,
		  incPosZAnakinG,
		  incRotBodyXAnakinG,
		  incRotBodyYAnakinG,
		  incrotLefTLegAnakinG,
		  incRotRightLegAnakinG,
		  incRotLeftArmAnakinG,
		  incRotRightArmAnakinG,
		  incRotRightWristAnakinG,
		  incRotLeftWristAnakinG;

}FRAME_ANAKIN;

FRAME_ANAKIN KeyFrameAnakinG[MAX_FRAMES_ANAKING];
int FrameIndexAnakin = 10;			//introducir datos
bool playAnakin = false;
int playIndexAnakin = 0;

/************************************
*									*
* FUNCIONES KEYFRAMES ANAKIN GHOST	*
*									*
*************************************/

void saveFrameAnakin(void)
{
	KeyFrameAnakinG[FrameIndexAnakin].posXAnakinG = posXAnakinG;
	KeyFrameAnakinG[FrameIndexAnakin].posYAnakinG = posYAnakinG;
	KeyFrameAnakinG[FrameIndexAnakin].posZAnakinG = posZAnakinG;

	KeyFrameAnakinG[FrameIndexAnakin].rotBodyXAnakinG = rotBodyXAnakinG;
	KeyFrameAnakinG[FrameIndexAnakin].rotBodyYAnakinG = rotBodyYAnakinG;

	KeyFrameAnakinG[FrameIndexAnakin].rotLefTLegAnakinG = rotLefTLegAnakinG;
	KeyFrameAnakinG[FrameIndexAnakin].rotRightLegAnakinG = rotRightLegAnakinG;
	KeyFrameAnakinG[FrameIndexAnakin].rotLeftArmAnakinG = rotLeftArmAnakinG;
	KeyFrameAnakinG[FrameIndexAnakin].rotRightArmAnakinG = rotRightArmAnakinG;
	KeyFrameAnakinG[FrameIndexAnakin].rotRightWristAnakinG = rotRightWristAnakinG;
	KeyFrameAnakinG[FrameIndexAnakin].rotLeftWristAnakinG = rotLeftWristAnakinG;

	cout << "******************************\n"
		<< "   FRAME INDEX:" << FrameIndexAnakin << endl
		<< "******************************\n"
		<< "posXAnakinG: " << posXAnakinG << endl
		<< "posYAnakinG: " << posYAnakinG << endl
		<< "posZAnakinG: " << posZAnakinG << endl
		<< "rotBodyXAnakinG: " << rotBodyXAnakinG << endl
		<< "rotBodyYAnakinG: " << rotBodyYAnakinG << endl
		<< "rotLefTLegAnakinG: " << rotLefTLegAnakinG << endl
		<< "rotRihtLegAnakinG: " << rotRightLegAnakinG << endl
		<< "rotLeftArmAnakinG: " << rotLeftArmAnakinG << endl
		<< "rotRightArmAnakinG: " << rotRightArmAnakinG << endl
		<< "rotRightWristAnakinG: " << rotRightWristAnakinG << endl
		<< "rotLeftWristAnakinG: " << rotLeftWristAnakinG << endl
		<< "{" << posXAnakinG << ", " << posYAnakinG << ", " << posZAnakinG << ", "
		<< rotBodyXAnakinG << ", " << rotBodyYAnakinG << ", "
		<< rotLefTLegAnakinG << ", " << rotRightLegAnakinG << ", " << rotLeftArmAnakinG << ", " << rotRightArmAnakinG << ", "
		<< rotRightWristAnakinG << ", " << rotLeftWristAnakinG << "}"
		<< "\n" << endl;
	FrameIndexAnakin++;
}
//para resetear las variables que manejan los frames
void resetElementsAnakin(void)
{
	posXAnakinG = KeyFrameAnakinG[0].posXAnakinG;
	posYAnakinG = KeyFrameAnakinG[0].posYAnakinG;
	posZAnakinG = KeyFrameAnakinG[0].posZAnakinG;

	rotBodyXAnakinG = KeyFrameAnakinG[0].rotBodyXAnakinG;
	rotBodyYAnakinG = KeyFrameAnakinG[0].rotBodyYAnakinG;

	rotLefTLegAnakinG = KeyFrameAnakinG[0].rotLefTLegAnakinG;
	rotRightLegAnakinG = KeyFrameAnakinG[0].rotRightLegAnakinG;
	rotLeftArmAnakinG = KeyFrameAnakinG[0].rotLeftArmAnakinG;
	rotRightArmAnakinG = KeyFrameAnakinG[0].rotRightArmAnakinG;
	rotRightWristAnakinG = KeyFrameAnakinG[0].rotRightWristAnakinG;
	rotLeftWristAnakinG = KeyFrameAnakinG[0].rotLeftWristAnakinG;

}

void interpolationAnakin(void)
{
	
	KeyFrameAnakinG[playIndexAnakin].incPosXAnakinG = (KeyFrameAnakinG[playIndexAnakin + 1].posXAnakinG - KeyFrameAnakinG[playIndexAnakin].posXAnakinG) / i_max_steps;
	KeyFrameAnakinG[playIndexAnakin].incPosYAnakinG = (KeyFrameAnakinG[playIndexAnakin + 1].posYAnakinG - KeyFrameAnakinG[playIndexAnakin].posYAnakinG) / i_max_steps;
	KeyFrameAnakinG[playIndexAnakin].incPosZAnakinG = (KeyFrameAnakinG[playIndexAnakin + 1].posZAnakinG - KeyFrameAnakinG[playIndexAnakin].posZAnakinG) / i_max_steps;

	KeyFrameAnakinG[playIndexAnakin].incRotBodyXAnakinG = (KeyFrameAnakinG[playIndexAnakin + 1].rotBodyXAnakinG - KeyFrameAnakinG[playIndexAnakin].rotBodyXAnakinG) / i_max_steps;
	KeyFrameAnakinG[playIndexAnakin].incRotBodyYAnakinG = (KeyFrameAnakinG[playIndexAnakin + 1].rotBodyYAnakinG - KeyFrameAnakinG[playIndexAnakin].rotBodyYAnakinG) / i_max_steps;

	KeyFrameAnakinG[playIndexAnakin].incrotLefTLegAnakinG = (KeyFrameAnakinG[playIndexAnakin + 1].rotLefTLegAnakinG - KeyFrameAnakinG[playIndexAnakin].rotLefTLegAnakinG) / i_max_steps;
	KeyFrameAnakinG[playIndexAnakin].incRotRightLegAnakinG = (KeyFrameAnakinG[playIndexAnakin + 1].rotRightLegAnakinG - KeyFrameAnakinG[playIndexAnakin].rotRightLegAnakinG) / i_max_steps;
	KeyFrameAnakinG[playIndexAnakin].incRotLeftArmAnakinG = (KeyFrameAnakinG[playIndexAnakin + 1].rotLeftArmAnakinG - KeyFrameAnakinG[playIndexAnakin].rotLeftArmAnakinG) / i_max_steps;
	KeyFrameAnakinG[playIndexAnakin].incRotRightWristAnakinG = (KeyFrameAnakinG[playIndexAnakin + 1].rotRightWristAnakinG - KeyFrameAnakinG[playIndexAnakin].rotRightWristAnakinG) / i_max_steps;
	KeyFrameAnakinG[playIndexAnakin].incRotLeftWristAnakinG = (KeyFrameAnakinG[playIndexAnakin + 1].rotLeftWristAnakinG - KeyFrameAnakinG[playIndexAnakin].rotLeftWristAnakinG) / i_max_steps;


}


/************************************
*									*
* VARIABLES KEYFRAMES SANTA CLAUS	*
*									*
*************************************/
// Posicion incial Santa
vec3 PosIniSanta(1.827f, 0.24312f, -1.662f);
//rotacion inical santa
int i_curr_stepsSanta = 0;
//Inicializo los valores de k frames
float posXSanta{ PosIniSanta.x },
posYSanta{ PosIniSanta.y },
posZSanta{ PosIniSanta.z },
rotBodyYSanta{ -143.0f },
rotBodyXSanta{ 0.0f },
rotLeftLegSanta{ 0.0f },
rotRightLegSanta{ 0.0f },
rotLeftArmSanta{ 0.0f },
rotRightArmSanta{ 0.0f },
rotRightWristSanta{ 0.0f };

#define MAX_FRAMES_SANTA 10

typedef struct _frameSanta
{
	//Variables para GUARDAR Key Frames
	float posXSanta,
		  posYSanta,
		  posZSanta,
		  rotBodyXSanta,
		  rotBodyYSanta,
		  rotLeftLegSanta,
		  rotRightLegSanta,
		  rotLeftArmSanta,
		  rotRightArmSanta,
		  rotRightWristSanta;
	//Incrementos
	float incPosXSanta,
		  incPosYSanta,
		  incPosZSanta,
		  incRotBodyXSanta,
		  incRotBodyYSanta,
		  incrotLeftLegSanta,
		  incRotRightLegSanta,
		  incRotLeftArmSanta,
		  incRotRightArmSanta,
		  incRotRightWristSanta;

}FRAME_SANTA;

FRAME_SANTA KeyFrameSanta[MAX_FRAMES_SANTA];
int FrameIndexSanta = 10;			//introducir datos
bool playSanta = false;
int playIndexSanta = 0;

/************************************
*									*
* FUNCIONES KEYFRAMES Santa GHOST	*
*									*
*************************************/

void saveFrameSanta(void)
{
	KeyFrameSanta[FrameIndexSanta].posXSanta = posXSanta;
	KeyFrameSanta[FrameIndexSanta].posYSanta = posYSanta;
	KeyFrameSanta[FrameIndexSanta].posZSanta = posZSanta;

	KeyFrameSanta[FrameIndexSanta].rotBodyXSanta = rotBodyXSanta;
	KeyFrameSanta[FrameIndexSanta].rotBodyYSanta = rotBodyYSanta;

	KeyFrameSanta[FrameIndexSanta].rotLeftLegSanta = rotLeftLegSanta;
	KeyFrameSanta[FrameIndexSanta].rotRightLegSanta = rotRightLegSanta;
	KeyFrameSanta[FrameIndexSanta].rotLeftArmSanta = rotLeftArmSanta;
	KeyFrameSanta[FrameIndexSanta].rotRightArmSanta = rotRightArmSanta;
	KeyFrameSanta[FrameIndexSanta].rotRightWristSanta = rotRightWristSanta;

	cout << "******************************\n"
		<< "     FRAME INDEX:" << FrameIndexSanta << endl
		<< "******************************\n"
		<< "posXSanta: " << posXSanta << endl
		<< "posYSanta: " << posYSanta << endl
		<< "posZSanta: " << posZSanta << endl
		<< "rotBodyXSanta: " << rotBodyXSanta << endl
		<< "rotBodyYSanta: " << rotBodyYSanta << endl
		<< "rotLeftLegSanta: " << rotLeftLegSanta << endl
		<< "rotRightLegSanta: " << rotRightLegSanta << endl
		<< "rotLeftArmSanta: " << rotLeftArmSanta << endl
		<< "rotRightArmSanta: " << rotRightArmSanta << endl
		<< "rotRightWristSanta: " << rotRightWristSanta << endl
		<< "{" << posXSanta << ", " << posYSanta << ", " << posZSanta << ", "
		<< rotBodyXSanta << ", " << rotBodyYSanta << ", "
		<< rotLeftLegSanta << ", " << rotRightLegSanta << ", " << rotLeftArmSanta << ", " << rotRightArmSanta << ", "
		<< rotRightWristSanta << "}" << "\n" << endl;
	
	FrameIndexSanta++;
}
//para resetear las variables que manejan los frames
void resetElementsSanta(void)
{
	posXSanta = KeyFrameSanta[0].posXSanta;
	posYSanta = KeyFrameSanta[0].posYSanta;
	posZSanta = KeyFrameSanta[0].posZSanta;

	rotBodyXSanta = KeyFrameSanta[0].rotBodyXSanta;
	rotBodyYSanta = KeyFrameSanta[0].rotBodyYSanta;

	rotLeftLegSanta = KeyFrameSanta[0].rotLeftLegSanta;
	rotRightLegSanta = KeyFrameSanta[0].rotRightLegSanta;
	rotLeftArmSanta = KeyFrameSanta[0].rotLeftArmSanta;
	rotRightArmSanta = KeyFrameSanta[0].rotRightArmSanta;
	rotRightWristSanta = KeyFrameSanta[0].rotRightWristSanta;


}

void interpolationSanta(void)
{

	KeyFrameSanta[playIndexSanta].incPosXSanta = (KeyFrameSanta[playIndexSanta + 1].posXSanta - KeyFrameSanta[playIndexSanta].posXSanta) / i_max_steps;
	KeyFrameSanta[playIndexSanta].incPosYSanta = (KeyFrameSanta[playIndexSanta + 1].posYSanta - KeyFrameSanta[playIndexSanta].posYSanta) / i_max_steps;
	KeyFrameSanta[playIndexSanta].incPosZSanta = (KeyFrameSanta[playIndexSanta + 1].posZSanta - KeyFrameSanta[playIndexSanta].posZSanta) / i_max_steps;

	KeyFrameSanta[playIndexSanta].incRotBodyXSanta = (KeyFrameSanta[playIndexSanta + 1].rotBodyXSanta - KeyFrameSanta[playIndexSanta].rotBodyXSanta) / i_max_steps;
	KeyFrameSanta[playIndexSanta].incRotBodyYSanta = (KeyFrameSanta[playIndexSanta + 1].rotBodyYSanta - KeyFrameSanta[playIndexSanta].rotBodyYSanta) / i_max_steps;

	KeyFrameSanta[playIndexSanta].incrotLeftLegSanta = (KeyFrameSanta[playIndexSanta + 1].rotLeftLegSanta - KeyFrameSanta[playIndexSanta].rotLeftLegSanta) / i_max_steps;
	KeyFrameSanta[playIndexSanta].incRotRightLegSanta = (KeyFrameSanta[playIndexSanta + 1].rotRightLegSanta - KeyFrameSanta[playIndexSanta].rotRightLegSanta) / i_max_steps;
	KeyFrameSanta[playIndexSanta].incRotLeftArmSanta = (KeyFrameSanta[playIndexSanta + 1].rotLeftArmSanta - KeyFrameSanta[playIndexSanta].rotLeftArmSanta) / i_max_steps;
	KeyFrameSanta[playIndexSanta].incRotRightArmSanta = (KeyFrameSanta[playIndexSanta + 1].rotRightArmSanta - KeyFrameSanta[playIndexSanta].rotRightArmSanta) / i_max_steps;
	KeyFrameSanta[playIndexSanta].incRotRightWristSanta = (KeyFrameSanta[playIndexSanta + 1].rotRightWristSanta - KeyFrameSanta[playIndexSanta].rotRightWristSanta) / i_max_steps;


}
// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Keyframes
//Inicializo los valores de k frames
float posX =PosIni.x, posY = PosIni.y, posZ = PosIni.z,rotY=90, rotPataFder = 0, rotPataTder = 0, 
rotPataFizq = 0, rotPataTizq = 0;


#define MAX_FRAMES 9
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
vec3 pointLightPositions[] = {
	vec3(posX,posY,posZ),
	vec3(-10.0,-10.0,-10.0),
	vec3(-10.0,-10.0,-10.0),
	vec3(-10.0,-10.0,-10.0),
};

vec3 LightP1;



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
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
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
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
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
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(modelMesa));
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
	Model AnakinGhostBody((char*)"Models/LegoStarWarsTCSModels/Characters/AnakinGhost/anakinGhostLightSaberBody.obj");
	Model AnakinGhostLeftArm((char*)"Models/LegoStarWarsTCSModels/Characters/AnakinGhost/anakinGhostLightSaberLeftArm.obj");
	Model AnakinGhostRightArm((char*)"Models/LegoStarWarsTCSModels/Characters/AnakinGhost/anakinGhostLightSaberRightArm.obj");
	Model AnakinGhostLeftHand((char*)"Models/LegoStarWarsTCSModels/Characters/AnakinGhost/anakinGhostLightSaberLeftHand.obj");
	Model AnakinGhostLightSaber((char*)"Models/LegoStarWarsTCSModels/Characters/AnakinGhost/anakinGhostLightSaberBlue.obj");
	Model AnakinGhostRightHand((char*)"Models/LegoStarWarsTCSModels/Characters/AnakinGhost/anakinGhostLightSaberRightHand.obj");
	Model AnakinGhostRightLeg((char*)"Models/LegoStarWarsTCSModels/Characters/AnakinGhost/anakinGhostLightSaberRightLeg.obj");
	Model AnakinGhostLeftLeg((char*)"Models/LegoStarWarsTCSModels/Characters/AnakinGhost/anakinGhostLightSaberLeftLeg.obj");

	// Santa Claus
	Model SantaClausBody((char*)"Models/LegoStarWarsTCSModels/Characters/SantaClaus/SantaBody.obj");
	Model SantaClausRightArm((char*)"Models/LegoStarWarsTCSModels/Characters/SantaClaus/SantaRightArm.obj");
	Model SantaClausRightHand((char*)"Models/LegoStarWarsTCSModels/Characters/SantaClaus/SantaRightHand.obj");
	Model SantaClausLeftArm((char*)"Models/LegoStarWarsTCSModels/Characters/SantaClaus/SantaLeftArm.obj");
	Model SantaClausLeftLeg((char*)"Models/LegoStarWarsTCSModels/Characters/SantaClaus/SantaLeftLeg.obj");
	Model SantaClausRightLeg((char*)"Models/LegoStarWarsTCSModels/Characters/SantaClaus/SantaRightLeg.obj");

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
	//Botas navideñas
	Model Bota1((char*)"Models/LegoStarWarsTCSModels/Decoration/botaNavidena/bota1.obj");
	Model Bota2((char*)"Models/LegoStarWarsTCSModels/Decoration/botaNavidena/bota2.obj");
	Model Bota3((char*)"Models/LegoStarWarsTCSModels/Decoration/botaNavidena/bota3.obj");
	Model Bota4((char*)"Models/LegoStarWarsTCSModels/Decoration/botaNavidena/bota4.obj");
	Model Bota5((char*)"Models/LegoStarWarsTCSModels/Decoration/botaNavidena/bota10.obj");
	Model Bota6((char*)"Models/LegoStarWarsTCSModels/Decoration/botaNavidena/bota11.obj");
	Model Bota7((char*)"Models/LegoStarWarsTCSModels/Decoration/botaNavidena/bota12.obj");
	Model Bota8((char*)"Models/LegoStarWarsTCSModels/Decoration/botaNavidena/bota13.obj");
	/*********************************
			Vehiculos
	*********************************/

	//DewBack
	Model DewbackBody((char*)"Models/LegoStarWarsTCSModels/Vehichles/DewBack/DewbackBody.obj");

	Model DewbackFRightLeg((char*)"Models/LegoStarWarsTCSModels/Vehichles/DewBack/DewbackFRightLeg.obj");

	Model DewbackBRightLeg((char*)"Models/LegoStarWarsTCSModels/Vehichles/DewBack/DewbackBRightLeg.obj");

	Model DewbackFLeftLeg((char*)"Models/LegoStarWarsTCSModels/Vehichles/DewBack/DewbackFLeftLeg.obj");

	Model DewbackBLeftLeg((char*)"Models/LegoStarWarsTCSModels/Vehichles/DewBack/DewbackBLeftLeg.obj");

	////Modelo de animación
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

	float animAnakin[MAX_FRAMES_ANAKING][11] = {
		{1.0433, 0.3221, -2.7851, 0, 33.9, 0, 0, -40, 21, 0, 79},
		{1.4433, 0.3221, -2.4351, 0, 33.9, 36, -19, -40, -34, 0, 53},
		{1.4933, 0.3221, -2.3351, 0, 68.9, 36, -19, -40, -80, 0, 34},
		{1.49329, 0.3221, -2.48509, 3, 37.8999, 36, 1, -62, 21, 0, -1.99998},
		{1.49329, 0.6221, -2.33508, 3, 37.8998, 35, 26, -100, -71, 0, 4.00006},
		{1.64329, 0.8221, -1.98508, 59, 0.899818, -75, -73, -100, -71, 0, 4.00006},
		{1.84329, 0.8221, -1.63508, 59, -115.1, -75, -21, -75, -53, 0, -35.9999},
		{1.69329, 0.3221, -1.58508, 7, -146.1, 3.8147e-05, -40, -75, -53, 0, -68.9999},
		{1.69329, 0.3221, -1.68508, 7, -210.1, 3.8147e-05, -40, -75, -53, 0, -90.9999},
		{1.69329, 0.3221, -1.48508, 7, -208.1, 3.8147e-05, -1, -75, 6, 0, -9.99994}
	};
	float animSanta[MAX_FRAMES_ANAKING][10] = {
		{1.827, 0.24312, -1.662, 0, -143, 0, -1, 0, 61, 0},
		{1.627, 0.24312, -2.062, 0, -143, -44, 37, 0, -44, 28},
		{1.627, 0.24312, -2.062, 0, -105, -1, 67, -28, -52, 40},
		{1.627, 0.24312, -1.96199, -6, -158, -0.999961, -32.0001, -28, -51.9999, 70},
		{1.577, 0.19312, -1.91199, -25, -160, -55.9999, -66.0002, 71, -51.9999, 69.9999},
		{1.577, 0.19312, -1.91199, 22, -171, 24.0001, -66.0002, 36, -57.9999, 43.9999},
		{1.577, 0.29312, -1.91199, 1, -109, 24.0001, -15.0002, 36, -78.9999, 7.99992},
		{1.577, 0.29312, -1.91199, 1, -50, 24.0001, -15.0002, 60, -100, 7.99992},
		{1.577, 0.29312, -1.91199, 1, -25.0002, 34.0001, -15.0002, -38, -131, 1.99992},
		{1.577, 0.09312, -1.91199, -87, -25.0002, -92.9999, -106, -67, -82.9999, 1.99992}
	};
	//Inicialización de KeyFrames
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
	//Inicialización de KeyFrames Anakin
	for (int i = 0; i < MAX_FRAMES_ANAKING; i++)
	{
		KeyFrameAnakinG[i].posXAnakinG = animAnakin[i][0];
		KeyFrameAnakinG[i].posYAnakinG = animAnakin[i][1];
		KeyFrameAnakinG[i].posZAnakinG = animAnakin[i][2];

		KeyFrameAnakinG[i].rotBodyXAnakinG = animAnakin[i][3];
		KeyFrameAnakinG[i].rotBodyYAnakinG = animAnakin[i][4];

		KeyFrameAnakinG[i].rotLefTLegAnakinG = animAnakin[i][5];
		KeyFrameAnakinG[i].rotRightLegAnakinG = animAnakin[i][6];
		KeyFrameAnakinG[i].rotLeftArmAnakinG = animAnakin[i][7];
		KeyFrameAnakinG[i].rotRightArmAnakinG = animAnakin[i][8];
		KeyFrameAnakinG[i].rotRightWristAnakinG = animAnakin[i][9];
		KeyFrameAnakinG[i].rotLeftWristAnakinG = animAnakin[i][10];

		KeyFrameAnakinG[i].incPosXAnakinG = 0.0f;
		KeyFrameAnakinG[i].incPosYAnakinG = 0.0f;
		KeyFrameAnakinG[i].incPosZAnakinG = 0.0f;

		KeyFrameAnakinG[i].incRotBodyXAnakinG = 0.0f;
		KeyFrameAnakinG[i].incRotBodyYAnakinG = 0.0f;
		KeyFrameAnakinG[i].incrotLefTLegAnakinG = 0.0f;
		KeyFrameAnakinG[i].incRotLeftArmAnakinG = 0.0f;
		KeyFrameAnakinG[i].incRotRightWristAnakinG = 0.0f;
		KeyFrameAnakinG[i].incRotLeftWristAnakinG = 0.0f;


	}
	
	//Inicialización de KeyFrames Santa
	for (int i = 0; i < MAX_FRAMES_SANTA; i++)
	{
		KeyFrameSanta[i].posXSanta = animSanta[i][0];
		KeyFrameSanta[i].posYSanta = animSanta[i][1];
		KeyFrameSanta[i].posZSanta = animSanta[i][2];

		KeyFrameSanta[i].rotBodyXSanta = animSanta[i][3];
		KeyFrameSanta[i].rotBodyYSanta = animSanta[i][4];

		KeyFrameSanta[i].rotLeftLegSanta = animSanta[i][5];
		KeyFrameSanta[i].rotRightLegSanta = animSanta[i][6];
		KeyFrameSanta[i].rotLeftArmSanta = animSanta[i][7];
		KeyFrameSanta[i].rotRightArmSanta = animSanta[i][8];
		KeyFrameSanta[i].rotRightWristSanta = animSanta[i][9];

		KeyFrameSanta[i].incPosXSanta = 0.0f;
		KeyFrameSanta[i].incPosYSanta = 0.0f;
		KeyFrameSanta[i].incPosZSanta = 0.0f;

		KeyFrameSanta[i].incRotBodyXSanta = 0.0f;
		KeyFrameSanta[i].incRotBodyYSanta = 0.0f;
		KeyFrameSanta[i].incrotLeftLegSanta = 0.0f;
		KeyFrameSanta[i].incRotLeftArmSanta = 0.0f;
		KeyFrameSanta[i].incRotRightWristSanta = 0.0f;


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
	vec3 cubePositions[] = {
		vec3(0.0f,  0.0f,  0.0f),
		vec3(2.0f,  5.0f, -15.0f),
		vec3(-1.5f, -2.2f, -2.5f),
		vec3(-3.8f, -2.0f, -12.3f),
		vec3(2.4f, -0.4f, -3.5f),
		vec3(-1.7f,  3.0f, -7.5f),
		vec3(1.3f, -2.0f, -2.5f),
		vec3(1.5f,  2.0f, -2.5f),
		vec3(1.5f,  0.2f, -1.5f),
		vec3(-1.3f,  1.0f, -1.5f)
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

	//Posición de mesas con 2 objetos
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

	mat4 projection = perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

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
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), cos(radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), cos(radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		mat4 view;
		view = camera.GetViewMatrix();


		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

		// Bind diffuse map
		//glBindTexture(GL_TEXTURE_2D, texture1);*/

		// Bind specular map
		/*glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);*/


		glBindVertexArray(VAO);
		mat4 tmp = mat4(1.0f); //Temp


		/*********************************
		*								 *
		*		CARGA DE MODELOS		 *
		*								 *
		**********************************/
		//Dewback
		mat4 model(1);
		mat4 DewbackBodyMatrix = model;
		DewbackBodyMatrix = translate(DewbackBodyMatrix, vec3(posX, posY, posZ));
		DewbackBodyMatrix = rotate(DewbackBodyMatrix, radians(rotY), vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(DewbackBodyMatrix));
		DewbackBody.Draw(lightingShader);


		////Pierna frontal derecha
		model = translate(DewbackBodyMatrix, vec3(-0.15935f, 0.013264f, 0.43353f));
		model = rotate(model, radians(rotPataFder), vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		DewbackFRightLeg.Draw(lightingShader);

		////Pierna frontal izquierda
		model = translate(DewbackBodyMatrix, vec3(0.13065f, 0.003264f, 0.44353f));
		model = rotate(model, radians(rotPataFizq), vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		DewbackFLeftLeg.Draw(lightingShader);

		////Pierna trasera derecha
		model = translate(DewbackBodyMatrix, vec3(-0.15935f, -0.006736f, -0.046474f));
		model = rotate(model, radians(rotPataTder), vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		DewbackBRightLeg.Draw(lightingShader);

		////Pierna trasera izquierda
		model = translate(DewbackBodyMatrix, vec3(0.17395f, 0.005927f, -0.022154f));
		model = rotate(model, radians(rotPataTizq), vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		DewbackBLeftLeg.Draw(lightingShader);

		//Cantina
		model = mat4(1);
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
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
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(Extra2Matrix));
		Extra2Body.Draw(lightingShader);

		mat4 Extra2Arm = mat4(1);
		Extra2Arm = translate(Extra2Matrix, vec3(0.036248f, 0.04077f, -0.006224f));
		Extra2Arm = rotate(Extra2Arm, radians(rotExtra2LeftArm), vec3(1.0f, 0.0f, 0.0f));
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(Extra2Arm));
		Extra2LeftArm.Draw(lightingShader);

		mat4 Extra2Hand = mat4(1);
		Extra2Hand = translate(Extra2Arm, vec3(0.054413f, -0.016304f, 0.058673));
		Extra2Hand = rotate(Extra2Hand, radians(rotExtra2LeftHand), vec3(0.0f, 0.0f, 1.0f));
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(Extra2Hand));
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
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(R2D2Matrix));
		R2D2Body.Draw(lightingShader);
		
		model = mat4(1);
		model = translate(R2D2Matrix, vec3(0.001126f, 0.195f, -0.007995f));
		model = rotate(model, radians(rotR2D2Head), vec3(0.0f, 1.0f, 0.0f));
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
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
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(StormTrooperMatrix));
		StormTrooper.Draw(lightingShader);

		//Greedo
		mat4 GreedoMatrix = mat4(1);
		GreedoMatrix = translate(GreedoMatrix, vec3(3.5887f, 0.15107f, -2.8939f));
		GreedoMatrix = rotate(GreedoMatrix, radians(-90.0f), vec3(0.0f, 1.0f, 0.0f));
		GreedoMatrix = translate(GreedoMatrix, vec3(0.0f, posGreedoBodyY, 0.0f));
		GreedoMatrix = rotate(GreedoMatrix, radians(-rotGreedoBodyZ), vec3(1.0f, 0.0f, 0.0f));
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(GreedoMatrix));
		Greedo.Draw(lightingShader);
		
		//Proyectile2
		model = mat4(1);
		model = translate(model, vec3(3.4643f, 0.24266f, -2.9742f));
		model = rotate(model, radians(-90.0f), vec3(0.0f, 1.0f, 0.0f));
		model = translate(model, vec3(0.0f, 0.0f, proyectile2T));
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		Proyectile.Draw(lightingShader);

		//Hansolo
		model = mat4(1);
		model = translate(model, vec3(3.0367f, 0.1959f, -2.9842f));
		model = rotate(model, radians(94.4f), vec3(0.0f, 1.0f, 0.0f));
		model = rotate(model, radians(-rotHanBodyY), vec3(0.0f, 1.0f, 0.0f));
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		HanSolo.Draw(lightingShader);

		//Proyectile1
		model = mat4(1);
		model = translate(model, vec3(3.195f, 0.22943f, -2.8994f));
		model = rotate(model, radians(95.3f), vec3(0.0f, 1.0f, 0.0f));
		model = rotate(model, radians(-0.812f), vec3(0.0f, 0.0f, 1.0f));
		model = translate(model, vec3(0.0f, 0.0f, proyectile1T));
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
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
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(DarthVaderMatrix));
		DarthVaderBody.Draw(lightingShader);

		model = mat4(1);
		model = translate(DarthVaderMatrix, vec3(-0.038212f, 0.054847f, -0.012091f));
		model = rotate(model, radians(rotVaderRightArm), vec3(1.0, 0.0, 0.0));
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
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
		// Botas de navidad
		drawInPosition(vec3(-3.0212f, 0.5f, -2.977f), Bota1, VAO, lightingShader, vec3(0.0f, 90.0f, 0.0f));
		drawInPosition(vec3(2.1736f, 0.5f, -2.7845f), Bota2, VAO, lightingShader, vec3(0.0f, 47.9f, 0.0f));
		drawInPosition(vec3(3.2908f, 0.5f, -1.1816f), Bota3, VAO, lightingShader, vec3(0.0f, 0.0f, 0.0f));
		drawInPosition(vec3(3.9499f, 0.5f, 0.7824f), Bota4, VAO, lightingShader, vec3(0.0f, 0.0f, 0.0f));
		drawInPosition(vec3(1.6387f, 0.5f, 2.3833f), Bota5, VAO, lightingShader, vec3(0.0f, 270.0f, 0.0f));
		drawInPosition(vec3(-0.8515f, 0.5f, 2.3833f), Bota6, VAO, lightingShader, vec3(0.0f, 270.0f, 0.0f));
		drawInPosition(vec3(-3.0212f, 0.5f, 2.3833f), Bota7, VAO, lightingShader, vec3(0.0f, 90.0f, 0.0f));
		// Santa Claus
		// Body
		mat4 SantaClausBodyMatrix = mat4(1);
		SantaClausBodyMatrix = translate(SantaClausBodyMatrix, vec3(posXSanta, posYSanta, posZSanta));
		SantaClausBodyMatrix = rotate(SantaClausBodyMatrix, radians(rotBodyYSanta), vec3(0.0f, 1.0f, 0.0));
		SantaClausBodyMatrix = rotate(SantaClausBodyMatrix, radians(rotBodyXSanta), vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(SantaClausBodyMatrix));
		SantaClausBody.Draw(lightingShader);

		// Left arm
		mat4 SantaClausLeftArmMatrix = mat4(1);
		SantaClausLeftArmMatrix = translate(SantaClausBodyMatrix, vec3(0.03577f, -0.01784f, -0.010859f));
		SantaClausLeftArmMatrix = rotate(SantaClausLeftArmMatrix, radians(rotLeftArmSanta), vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(SantaClausLeftArmMatrix));
		SantaClausLeftArm.Draw(lightingShader);

		// Right arm
		mat4 SantaClausRightArmMatrix = mat4(1);
		SantaClausRightArmMatrix = translate(SantaClausBodyMatrix, vec3(-0.038272f, -0.025719f, -0.012924f));
		SantaClausRightArmMatrix = rotate(SantaClausRightArmMatrix, radians(rotRightArmSanta), vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(SantaClausRightArmMatrix));
		SantaClausRightArm.Draw(lightingShader);

		// Right hand
		mat4 SantaClausRightHandMatrix = mat4(1);
		SantaClausRightHandMatrix = translate(SantaClausRightArmMatrix, vec3(-0.051122f, -0.06272f, 0.018834f));
		SantaClausRightHandMatrix = rotate(SantaClausRightHandMatrix, radians(50.0f), vec3(1.0f, 0.0f, 0.0));
		SantaClausRightHandMatrix = rotate(SantaClausRightHandMatrix, radians(rotRightWristSanta), vec3(0.0f, 0.0f, 1.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(SantaClausRightHandMatrix));
		SantaClausRightHand.Draw(lightingShader);

		// Right Leg
		mat4 SantaClausRightLegMatrix = mat4(1);
		SantaClausRightLegMatrix = translate(SantaClausBodyMatrix, vec3(0.001858f, -0.14761f, 0.005748f));
		SantaClausRightLegMatrix = rotate(SantaClausRightLegMatrix, radians(rotRightLegSanta), vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(SantaClausRightLegMatrix));
		SantaClausRightLeg.Draw(lightingShader);

		// Leftt Leg
		mat4 SantaClausLefttLegMatrix = mat4(1);
		SantaClausLefttLegMatrix = translate(SantaClausBodyMatrix, vec3(0.001858f, -0.14761f, 0.005748f));
		SantaClausLefttLegMatrix = rotate(SantaClausLefttLegMatrix, radians(rotLeftLegSanta), vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(SantaClausLefttLegMatrix));
		SantaClausLeftLeg.Draw(lightingShader);

		/*******************************************

				 Lamparas

		*******************************************/
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
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
		*	Anakin Ghost       *
		************************/
		// Anakin Ghost
		// Body
		mat4 AnakinGhostBodyMatrix = mat4(1);
		AnakinGhostBodyMatrix = translate(AnakinGhostBodyMatrix, vec3(posXAnakinG, posYAnakinG, posZAnakinG));
		AnakinGhostBodyMatrix = rotate(AnakinGhostBodyMatrix, radians(rotBodyYAnakinG), vec3(0.0f, 1.0f, 0.0));
		AnakinGhostBodyMatrix = rotate(AnakinGhostBodyMatrix, radians(rotBodyXAnakinG), vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(AnakinGhostBodyMatrix));
		AnakinGhostBody.Draw(lightingShader);

		// Left arm
		mat4 AnakinGhostLeftArmMatrix = mat4(1);
		AnakinGhostLeftArmMatrix = translate(AnakinGhostBodyMatrix, vec3(0.040555f, -0.049699f, 0.0f));
		AnakinGhostLeftArmMatrix = rotate(AnakinGhostLeftArmMatrix, radians(rotLeftArmAnakinG), vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(AnakinGhostLeftArmMatrix));
		AnakinGhostLeftArm.Draw(lightingShader);

		// Right arm
		mat4 AnakinGhostRightArmMatrix = mat4(1);
		AnakinGhostRightArmMatrix = translate(AnakinGhostBodyMatrix, vec3(-0.040555f, -0.049699f, 0.0f));
		AnakinGhostRightArmMatrix = rotate(AnakinGhostRightArmMatrix, radians(rotRightArmAnakinG), vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(AnakinGhostRightArmMatrix));
		AnakinGhostRightArm.Draw(lightingShader);

		// Left hand
		mat4 AnakinGhostLeftHandMatrix = mat4(1);
		AnakinGhostLeftHandMatrix = translate(AnakinGhostLeftArmMatrix, vec3(0.055805f, -0.081271f, 0.021196f));
		AnakinGhostLeftHandMatrix = rotate(AnakinGhostLeftHandMatrix, radians(41.0f), vec3(1.0f, 0.0f, 0.0));
		AnakinGhostLeftHandMatrix = rotate(AnakinGhostLeftHandMatrix, radians(rotLeftWristAnakinG), vec3(0.0f, 0.0f, 1.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(AnakinGhostLeftHandMatrix));
		AnakinGhostLeftHand.Draw(lightingShader);
		
		//Light Saber
		glDisable(GL_BLEND);
		mat4 AnakinGhostLightSaberMatrix = AnakinGhostLeftHandMatrix;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(AnakinGhostLightSaberMatrix));
		AnakinGhostLightSaber.Draw(lightingShader);

		
		// Right hand
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		mat4 AnakinGhostRightHandMatrix = mat4(1);
		AnakinGhostRightHandMatrix = translate(AnakinGhostRightArmMatrix, vec3(-0.056267f, -0.086371f, 0.025752));
		AnakinGhostRightHandMatrix = rotate(AnakinGhostRightHandMatrix, radians(45.0f), vec3(1.0f, 0.0f, 0.0));
		AnakinGhostRightHandMatrix = rotate(AnakinGhostRightHandMatrix, radians(rotRightWristAnakinG), vec3(0.0f, 0.0f, 1.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(AnakinGhostRightHandMatrix));
		AnakinGhostRightHand.Draw(lightingShader);

		// Right Leg
		mat4 AnakinGhostRightLegMatrix = mat4(1);
		AnakinGhostRightLegMatrix = translate(AnakinGhostBodyMatrix, vec3(-0.042269f, -0.18936f, -0.010731));
		AnakinGhostRightLegMatrix = rotate(AnakinGhostRightLegMatrix, radians(rotRightLegAnakinG), vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(AnakinGhostRightLegMatrix));
		AnakinGhostRightLeg.Draw(lightingShader);

		// Leftt Leg
		mat4 AnakinGhostLefttLegMatrix = mat4(1);
		AnakinGhostLefttLegMatrix = translate(AnakinGhostBodyMatrix, vec3(0.037731f, -0.18936f, -0.010731));
		AnakinGhostLefttLegMatrix = rotate(AnakinGhostLefttLegMatrix, radians(rotLefTLegAnakinG), vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(AnakinGhostLefttLegMatrix));
		AnakinGhostLeftLeg.Draw(lightingShader);
		glDisable(GL_BLEND);

		/***************************
		*
		*	Carga Adornos navideños
		*
		****************************/
		// ChristmasTree
		drawInPosition(vec3(-3.3525f, 0.31978f, 1.4678f), ChristmasTree, VAO, lightingShader, vec3(0.0f, 71.4f, 0.0f));
		/*_______________________________Personaje Animado___________________________*/ 
		/*animShader.Use();
		modelLoc = glGetUniformLocation(animShader.Program, "model");
		viewLoc = glGetUniformLocation(animShader.Program, "view");
		projLoc = glGetUniformLocation(animShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

		glUniform3f(glGetUniformLocation(animShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(animShader.Program, "material.shininess"), 32.0f);
		glUniform3f(glGetUniformLocation(animShader.Program, "light.ambient"), 0.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(animShader.Program, "light.diffuse"), 0.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(animShader.Program, "light.specular"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(animShader.Program, "light.direction"),0.0f, -1.0f, -1.0f);
		view = camera.GetViewMatrix();
		model = mat4(1);
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		animacionPersonaje.Draw(animShader);
*/







		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		


		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = mat4(mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, value_ptr(projection));

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

	/******************************
	*							  *
	*	  MOVIMIENTOS ANAKIN      *
	*							  *
	*******************************/
	if (playAnakin)
	{
		if (i_curr_stepsAnakin >= i_max_steps) //end of animation between frames?
		{
			playIndexAnakin++;
			if (playIndexAnakin > FrameIndexAnakin - 2)	//end of total animation?
			{
				cout << "Ternima anim" << endl;
				playIndexAnakin = 0;
				playAnakin = false;
			}
			else //Next frame interpolations
			{
				i_curr_stepsAnakin = 0; //Reset counter
								  //Interpolation
				interpolationAnakin();
			}
		}
		else
		{

			//Draw animation
			posXAnakinG += KeyFrameAnakinG[playIndexAnakin].incPosXAnakinG;
			posYAnakinG += KeyFrameAnakinG[playIndexAnakin].incPosYAnakinG;
			posZAnakinG += KeyFrameAnakinG[playIndexAnakin].incPosZAnakinG;

			rotBodyXAnakinG += KeyFrameAnakinG[playIndexAnakin].incRotBodyXAnakinG;
			rotBodyYAnakinG += KeyFrameAnakinG[playIndexAnakin].incRotBodyYAnakinG;

			rotLefTLegAnakinG += KeyFrameAnakinG[playIndexAnakin].incrotLefTLegAnakinG;
			rotRightLegAnakinG += KeyFrameAnakinG[playIndexAnakin].incRotRightLegAnakinG;
			rotLeftArmAnakinG += KeyFrameAnakinG[playIndexAnakin].incRotLeftArmAnakinG;
			rotRightArmAnakinG += KeyFrameAnakinG[playIndexAnakin].incRotRightArmAnakinG;
			rotRightWristAnakinG += KeyFrameAnakinG[playIndexAnakin].incRotRightWristAnakinG;
			rotLeftWristAnakinG += KeyFrameAnakinG[playIndexAnakin].incRotLeftWristAnakinG;

			i_curr_stepsAnakin++;
		}

	}
	/******************************
	*							  *
	*	  MOVIMIENTOS SANTA       *
	*							  *
	*******************************/
	if (playSanta)
	{
		if (i_curr_stepsSanta >= i_max_steps) //end of animation between frames?
		{
			playIndexSanta++;
			if (playIndexSanta > FrameIndexSanta - 2)	//end of total animation?
			{
				cout << "Ternima anim" << endl;
				playIndexSanta = 0;
				playSanta = false;
			}
			else //Next frame interpolations
			{
				i_curr_stepsSanta = 0; //Reset counter
								  //Interpolation
				interpolationSanta();
			}
		}
		else
		{

			//Draw animation
			posXSanta += KeyFrameSanta[playIndexSanta].incPosXSanta;
			posYSanta += KeyFrameSanta[playIndexSanta].incPosYSanta;
			posZSanta += KeyFrameSanta[playIndexSanta].incPosZSanta;

			rotBodyXSanta += KeyFrameSanta[playIndexSanta].incRotBodyXSanta;
			rotBodyYSanta += KeyFrameSanta[playIndexSanta].incRotBodyYSanta;

			rotLeftLegSanta += KeyFrameSanta[playIndexSanta].incrotLeftLegSanta;
			rotRightLegSanta += KeyFrameSanta[playIndexSanta].incRotRightLegSanta;
			rotLeftArmSanta += KeyFrameSanta[playIndexSanta].incRotLeftArmSanta;
			rotRightArmSanta += KeyFrameSanta[playIndexSanta].incRotRightArmSanta;
			rotRightWristSanta += KeyFrameSanta[playIndexSanta].incRotRightWristSanta;

			i_curr_stepsSanta++;
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
					cout << "Ternima anim" << endl;
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
	if (keys[GLFW_KEY_M])
	{
		if (playAnakin == false && (FrameIndexAnakin > 1))
		{

			resetElementsAnakin();
			//First Interpolation				
			interpolationAnakin();

			playAnakin = true;
			playIndexAnakin = 0;
			i_curr_stepsAnakin = 0;
		}
		else
		{
			playAnakin = false;
		}

	}

	if (keys[GLFW_KEY_M])
	{
		if (playSanta == false && (FrameIndexSanta > 1))
		{

			resetElementsSanta();
			//First Interpolation				
			interpolationSanta();

			playSanta = true;
			playIndexSanta = 0;
			i_curr_stepsSanta = 0;
		}
		else
		{
			playSanta = false;
		}

	}
	
	if (keys[GLFW_KEY_K])
	{
		if (FrameIndexAnakin< MAX_FRAMES_ANAKING)
		{
			//saveFrame();
			saveFrameAnakin();
		}

	}
	if (keys[GLFW_KEY_J])
	{
		if (FrameIndexSanta < MAX_FRAMES_SANTA)
		{
			saveFrameSanta();
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
			LightP1 = vec3(1.0f, 0.0f, 0.0f);
		else
			LightP1 = vec3(0.0f, 0.0f, 0.0f);
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

	//controles anakin
	if (keys[GLFW_KEY_UP])
		posXAnakinG += 0.05;
	if (keys[GLFW_KEY_DOWN])
		posXAnakinG -= 0.05;
	if (keys[GLFW_KEY_LEFT])
		posZAnakinG += 0.05;
	if (keys[GLFW_KEY_RIGHT])
		posZAnakinG -= 0.05;
	if (keys[GLFW_KEY_Q])
		posYAnakinG += 0.05;
	if (keys[GLFW_KEY_E])
		posYAnakinG -= 0.05;
	if (keys[GLFW_KEY_HOME])
		rotBodyXAnakinG += 1;
	if (keys[GLFW_KEY_END])
		rotBodyXAnakinG -= 1;
	if (keys[GLFW_KEY_PAGE_UP])
		rotBodyYAnakinG += 1;
	if (keys[GLFW_KEY_PAGE_DOWN])
		rotBodyYAnakinG -= 1;
	if (keys[GLFW_KEY_R])
		rotLefTLegAnakinG += 1;
	if (keys[GLFW_KEY_F])
		rotLefTLegAnakinG -= 1;
	if (keys[GLFW_KEY_T])
		rotRightLegAnakinG += 1;
	if (keys[GLFW_KEY_G])
		rotRightLegAnakinG -= 1;
	if (keys[GLFW_KEY_Y])
		rotLeftArmAnakinG += 1;
	if (keys[GLFW_KEY_H])
		rotLeftArmAnakinG -= 1;
	if (keys[GLFW_KEY_Z])
		rotRightArmAnakinG += 1;
	if (keys[GLFW_KEY_X])
		rotRightArmAnakinG -= 1;
	if (keys[GLFW_KEY_C])
		rotLeftWristAnakinG += 1;
	if (keys[GLFW_KEY_V])
		rotLeftWristAnakinG -= 1;
	//Controles santa claus
	if (keys[GLFW_KEY_F1])
		posXSanta += 0.05;
	if (keys[GLFW_KEY_1])
		posXSanta -= 0.05;
	if (keys[GLFW_KEY_F2])
		posYSanta += 0.05;
	if (keys[GLFW_KEY_2])
		posYSanta -= 0.05;
	if (keys[GLFW_KEY_F3])
		posZSanta += 0.05;
	if (keys[GLFW_KEY_3])
		posZSanta -= 0.05;
	if (keys[GLFW_KEY_F4])
		rotBodyXSanta += 1;
	if (keys[GLFW_KEY_4])
		rotBodyXSanta -= 1;
	if (keys[GLFW_KEY_F5])
		rotBodyYSanta += 1;
	if (keys[GLFW_KEY_5])
		rotBodyYSanta -= 1;
	if (keys[GLFW_KEY_F6])
		rotLeftLegSanta += 1;
	if (keys[GLFW_KEY_6])
		rotLeftLegSanta -= 1;
	if (keys[GLFW_KEY_F7])
		rotRightLegSanta += 1;
	if (keys[GLFW_KEY_7])
		rotRightLegSanta -= 1;
	if (keys[GLFW_KEY_F8])
		rotLeftArmSanta += 1;
	if (keys[GLFW_KEY_8])
		rotLeftArmSanta -= 1;
	if (keys[GLFW_KEY_F9])
		rotRightArmSanta += 1;
	if (keys[GLFW_KEY_9])
		rotRightArmSanta -= 1;
	if (keys[GLFW_KEY_F10])
		rotRightWristSanta += 1;
	if (keys[GLFW_KEY_0])
		rotRightWristSanta -= 1;


	// Camera controls
	if (keys[GLFW_KEY_W])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}






}