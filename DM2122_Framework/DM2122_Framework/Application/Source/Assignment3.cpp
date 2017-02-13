#include "Assignment3.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include "Application.h"
#include "shader.hpp"
#include "Mtx44.h"
#include "timer.h"
#include "Meshbuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "LoadOBJ.h"

#include <string>

using std::string;
extern GLFWwindow* m_window;

Assignment3::Assignment3()
{
}

Assignment3::~Assignment3()
{
}

void Assignment3::Init()
{
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");	// Use our shader
	glUseProgram(m_programID);

	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

	//Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	//Sunlight properties
	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].position.Set(3000, 900, 700);
	light[0].color.Set(1, 1, 1);
	light[0].power = 1.5;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	//Igloo Light properties
	light[1].type = Light::LIGHT_POINT;
	light[1].position.Set(-500, 0, 0);
	light[1].color.Set(1, 1, 1);
	light[1].power = 5;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);

	//Night Light properties
	light[2].type = Light::LIGHT_DIRECTIONAL;
	light[2].position.Set(0, 3000, 0);
	light[2].color.Set(1, 1, 1);
	light[2].power = 1;
	light[2].kC = 1.f;
	light[2].kL = 0.01f;
	light[2].kQ = 0.001f;
	light[2].cosCutoff = cos(Math::DegreeToRadian(45));
	light[2].cosInner = cos(Math::DegreeToRadian(30));
	light[2].exponent = 3.f;
	light[2].spotDirection.Set(0.f, 1.f, 0.f);

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);
	glUniform1i(m_parameters[U_NUMLIGHTS], 3);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

	glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
	glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
	glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
	glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
	glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
	glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], light[2].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT2_COSINNER], light[2].cosInner);
	glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);

	// Set background color to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_DEPTH_TEST);// Enable depth test	glEnable(GL_CULL_FACE);// Enable cull test	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	glEnable(GL_BLEND);//Enable blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//Enable cursor

	//camera
	camera.Init(Vector3(0, 0, -200), Vector3(1, 0, 0), Vector3(0, 1, 0));

	//axis 
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);

	//Lightball
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("LIGHTBALL", Color(1, 1, 1), 60, 20, 1);
	meshList[GEO_LIGHTBALL2] = MeshBuilder::GenerateSphere("LIGHTBALL2", Color(1, 1, 1), 60, 20, 1);
	meshList[GEO_LIGHTBALL3] = MeshBuilder::GenerateSphere("LIGHTBALL3", Color(1, 0, 0), 60, 20, 1);
	
	//=====================================
	//DayTime
	//=====================================
	//Bottom
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1, 1);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//Bottom.tga");

	//Front skybox
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1, 1);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image/Front.tga");
	//back skybox
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1, 1);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//Back.tga");

	//Left skybox
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1, 1);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//Left.tga");

	//Right skybox
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1, 1);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//Right.tga");

	//top skybox
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1, 1);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//Top.tga");

	//=====================================
	//Night
	//=====================================
	//Front skybox
	meshList[GEO_FRONTNight] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1, 1);
	meshList[GEO_FRONTNight]->textureID = LoadTGA("Image//FrontNight.tga");
	//back skybox
	meshList[GEO_BACKNight] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1, 1);
	meshList[GEO_BACKNight]->textureID = LoadTGA("Image//BackNight.tga");

	//Left skybox
	meshList[GEO_LEFTNight] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1, 1);
	meshList[GEO_LEFTNight]->textureID = LoadTGA("Image//LeftNight.tga");

	//Right skybox
	meshList[GEO_RIGHTNight] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1, 1);
	meshList[GEO_RIGHTNight]->textureID = LoadTGA("Image//RightNight.tga");

	//top skybox
	meshList[GEO_TOPNight] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1, 1);
	meshList[GEO_TOPNight]->textureID = LoadTGA("Image//TopNight.tga");

	//text
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//ExportedFont.tga");

	meshList[GEO_SCREEN] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_SCREEN]->textureID = LoadTGA("Image//ExportedFont.tga");

	//Ground
	meshList[GROUND] = MeshBuilder::GenerateOBJ("Ground", "OBJ//Land_Mesh.obj");
	meshList[GROUND]->textureID = LoadTGA("Image//bottom.tga");
	meshList[GROUND]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
	meshList[GROUND]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GROUND]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GROUND]->material.kShininess = 1.0f;

	//Log
	meshList[Log] = MeshBuilder::GenerateOBJ("Log", "OBJ//Log.obj");
	meshList[Log]->textureID = LoadTGA("Image//log_texture.tga");

	//igloo
	meshList[Igloo] = MeshBuilder::GenerateOBJ("Igloo", "OBJ//igloo.obj");
	meshList[Igloo]->textureID = LoadTGA("Image//snow.tga");

	//=======================================
	//objects that interact
	//=======================================
	//flagpole
	meshList[FLAGPOLE] = MeshBuilder::GenerateOBJ("flagpole", "OBJ//flagpole.obj");
	meshList[FLAGPOLE]->textureID = LoadTGA("Image//FlagPoleTexture.tga");

	//Flag
	meshList[FLAG] = MeshBuilder::GenerateOBJ("flag!", "OBJ//flag.obj");
	meshList[FLAG]->textureID = LoadTGA("Image//FlagTexture.tga");

	//StrawHat
	meshList[STRAWHAT] = MeshBuilder::GenerateOBJ("strawhat", "OBJ//Strawhat.obj");
	meshList[STRAWHAT]->textureID = LoadTGA("Image//StrawHatTexture.tga");

	//Sled
	meshList[SLED] = MeshBuilder::GenerateOBJ("sled", "OBJ//Sled.obj");
	meshList[SLED]->textureID = LoadTGA("Image//SledTexture.tga");

	//Meat
	meshList[MEAT] = MeshBuilder::GenerateOBJ("meat", "OBJ//Meat.obj");
	meshList[MEAT]->textureID = LoadTGA("Image//MeatTexture.tga");

	//Flower
	meshList[FLOWER] = MeshBuilder::GenerateOBJ("flower", "OBJ//Flower.obj");
	meshList[FLOWER]->textureID = LoadTGA("Image//FlowerTexture.tga");

	//========================================
	//Parts of the Body
	//========================================
	meshList[UPPER_BODY] = MeshBuilder::GenerateHemisphere("upper body", Color(0.7, 0.55, 0.25), 60, 20, 1);
	meshList[LIMBS] = MeshBuilder::GenerateCylinder("limbs", Color(0.7, 0.55, 0.25), 60, 4, 1);
	meshList[HAND] = MeshBuilder::GenerateHemisphere("palm", Color(0.7, 0.55, 0.25), 36, 20, 1);
	meshList[HAND_LOWER] = MeshBuilder::GenerateHemisphere("lower hand", Color(0, 0, 0), 36, 20, 1);
	meshList[LEGS] = MeshBuilder::GenerateCylinder("legs", Color(0.7, 0.3, 0.2), 60, 3, 1);
	meshList[LOWER_BODY] = MeshBuilder::GenerateHemisphere("pants", Color(0.7, 0.3, 0.2), 60, 20, 1);

	//========================================
	//Parts of the Head
	//========================================
	meshList[HEAD] = MeshBuilder::GenerateSphere("head", Color(0.7, 0.55, 0.25), 60, 20, 1);
	meshList[EARS] = MeshBuilder::GenerateCone("ears", Color(0.7, 0.55, 0.25), 60, 3, 1);
	meshList[HORNS] = MeshBuilder::GenerateSphere("horns", Color(0.5, 0.25, 0.05), 60, 20, 1);
	
	//==============================================
	//Facial Expression
	//==============================================
	meshList[EYES] = MeshBuilder::GenerateHemisphere("eyes", Color(1, 1, 1), 36, 3, 1);
	meshList[PUPIL] = MeshBuilder::GenerateHemisphere("pupil", Color(0, 0, 0), 36, 2, 1);
	meshList[NOSE] = MeshBuilder::GenerateHemisphere("Nose", Color(0, 0, 0.8), 36, 2, 1);
	meshList[MOUTH] = MeshBuilder::GenerateHemisphere("Mouth", Color(1, 0.7, 0.8), 60, 20, 1);
	
	//=====================================
	//Items on Chopper
	//=====================================
	meshList[HAT] = MeshBuilder::GenerateCylinder("hat", Color(1.0, 0.8, 0.8), 60, 5, 1);
	meshList[HAT_CROSS] = MeshBuilder::GenerateCube("cross", Color(1, 1, 1), 1, 1, 1);
	meshList[BAG_HANDLE] = MeshBuilder::GenerateTorus("bag handle", Color(0.255, 0.412, 0.882), 36, 12, 3, 1);
	meshList[BAG] = MeshBuilder::GenerateCube("Bag", Color(0.255, 0.412, 0.882), 1, 1, 1);
	
	//=======================================
	//Environment objects
	//=======================================
	meshList[SNOW] = MeshBuilder::GenerateSphere("snow", Color(1, 1, 1), 36, 20, 1);
	meshList[CARROT] = MeshBuilder::GenerateCone("carrot", Color(1, 0.5, 0), 36, 1, 1);
	meshList[TREE_LEAVES] = MeshBuilder::GenerateCone("leaves", Color(1, 1, 1), 36, 1, 1);
	meshList[SNOW_UNDER] = MeshBuilder::GenerateTorus("torus", Color(1, 1, 1), 36, 12, 3, 2);
	meshList[DRUM_MOUNTAIN] = MeshBuilder::GenerateDeformedCylinder("drum mountain", Color(0.3, 0.3, 0.3), 36, 3, 1);
	
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 7000.0f);
	projectionStack.LoadMatrix(projection);

	Switch = true;
	Switch_LightBall = false;

	//for character
	Walking_X = 0.f;
	Walking_Z = 0.f;
	Jump_ = 0.f;
	Walking_Rotation_Left = 0.f;
	Walking_Rotation_Right = 0.f;
	RotateBody = 0.f;

	//interactions
	//Flag
	MoveFlag = 0.0f;
	RaiseUp = false;
	RaiseDown = false;

	//Tree
	ShakeTree = 0.0;;
	SnowballFallY = 50.0f;
	SnowballFallZ = 0.0f;
	Fall = false;
	PickUp = false;

	//Day and Night
	DayBreak = true;
	NightFall = false;

	//Talk 
	Talk = false;
}

void Assignment3::Update(double dt)
{
	double X_Pos, Y_Pos; //get cursor position
	int width, height; //get window size

	glfwGetCursorPos(m_window, &X_Pos, &Y_Pos);// getting the cursor position 
	glfwGetWindowSize(m_window, &width, &height); //get size to center cursor 
	glfwSetCursorPos(m_window, width / 2, height / 2); //set cursor to center of screen

	//modes
	if (Application::IsKeyPressed('1'))
	{
		glEnable(GL_CULL_FACE);
	}
	if (Application::IsKeyPressed('2'))
	{
		glDisable(GL_CULL_FACE);
	}
	if (Application::IsKeyPressed('3'))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	}
	if (Application::IsKeyPressed('4'))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
	}

	//Switching on and off
	if (Application::IsKeyPressed('B'))
	{
		Switch = true;
		Switch_LightBall = false;
	}

	if (Application::IsKeyPressed('V'))
	{
		Switch = false;
		Switch_LightBall = true;
	}

	fps = 1 / dt;

	//reset whole scene 
	if (Application::IsKeyPressed('Q'))
	{
		Walking_X = 0.0f;
		Walking_Z = 0.0f;
		Jump_ = 0.0f;
		Walking_Rotation_Left = 0.0f;
		Walking_Rotation_Right = 0.0f;
		RotateBody = 0.0f;

		MoveFlag = 0.0f;
		RaiseDown = false;
		RaiseUp = false;

		ShakeTree = 0.0;;
		SnowballFallY = 0.0f;
		SnowballFallZ = 0.0f;
		Fall = false;
		DayBreak = true;
		PickUp = false;
		camera.Reset();//reset camera
	}

	//===================================
	//Interactions
	//===================================
	//flag raising
	static float Direction;

	if (Application::IsKeyPressed('E') && camera.position.x < -100 && camera.position.x > -300
		&& camera.position.z < -150 && camera.position.z > -350)
	{
		if (MoveFlag < -2)
		{
			RaiseUp = true;
			RaiseDown = false;
		}
		else if (MoveFlag >= 0)
		{
			RaiseUp = false;
			RaiseDown = true;
		}
	}

	if (RaiseDown && !(Application::IsKeyPressed('E')))
	{
		Direction = -1;
		MoveFlag += (float)(10 * Direction * dt);

		if (MoveFlag < -2)
		{
			RaiseDown = false;
		}
	}

	if (RaiseUp && !(Application::IsKeyPressed('E')))
	{
		Direction = 1;
		MoveFlag += (float)(10 * Direction * dt);

		if (MoveFlag > 0)
		{
			RaiseUp = false;
		}
	}

	//Shake Tree
	static float DirTree = 1;

	if (camera.position.x > 400 && camera.position.x < 600
		&& camera.position.z > -100 && camera.position.z < 100 
		&& Application::IsKeyPressed('E'))
	{
		//for the shaking of trees
		if (ShakeTree > 10)
		{
			DirTree = -1;
		}
		else if (ShakeTree < -10)
		{
			DirTree = 1;
		}

		ShakeTree += (float)(40 * DirTree * dt);
	}

	//When not at original position after letting go E
	if (!(Application::IsKeyPressed('E')))
	{
		if (ShakeTree > 0)
		{
			DirTree = -1;
			ShakeTree += (float)(40 * DirTree * dt);
		}
		if (ShakeTree < 0)
		{
			DirTree = 1;
			ShakeTree += (float)(40 * DirTree * dt);
		}
	}

	//Snowball to drop when shaking
	if (ShakeTree < -5 && ShakeTree > -10
		|| ShakeTree > 5 && ShakeTree < 10)
	{
		Fall = true;
	}

	if (Fall)
	{
		SnowballFallY -= (float)(150 * dt);
		SnowballFallZ -= (float)(150 * dt);

		if (SnowballFallY < -50)
		{
			SnowballFallY = -50;
		}

		if (SnowballFallZ < -200)
		{
			SnowballFallZ = -200;
		}
	}

	//Changing between Day and Night
	if (Application::IsKeyPressed('E'))
	{
		if (camera.position.x < -400 && camera.position.x > -600
			&& camera.position.z > -100 && camera.position.z < 100)
		{
			if (DayBreak)
			{
				DayBreak = false;
			}
		}
	}

	if (Application::IsKeyPressed('X'))
	{
		if (camera.position.x < -400 && camera.position.x > -600
			&& camera.position.z > -100 && camera.position.z < 100)
		{
			if (DayBreak == false)
			{
				DayBreak = true;
			}
		}
	}

	//Pick up snowball
	if (camera.position.x > 450 && camera.position.x < 550
		&& camera.position.z  > SnowballFallZ - 50 && camera.position.z < SnowballFallZ + 50
		&& Fall && Application::IsKeyPressed(VK_LBUTTON))
	{
		PickUp = true;
	}

	//Talk
	if (camera.position.x > -100 && camera.position.x < 100
		&& camera.position.z  > -100 && camera.position.z < 100
		&& Application::IsKeyPressed(VK_LBUTTON))
	{
		Talk = true;
	}

	camera.Update(dt, (width / 2) - X_Pos, (height / 2) - Y_Pos);
}

void Assignment3::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y,
		camera.position.z, camera.target.x, camera.target.y,
		camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	//Passing the position of light 0 to the shader (Day)
	Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
	Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);

	//Passing the position of light 2 to the shader (Night)
	Vector3 light2Dir(light[2].position.x, light[2].position.y, light[2].position.z);
	Vector3 light2Direction_cameraspace = viewStack.Top() * lightDir;
	glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &light2Direction_cameraspace.x);

	//Passing the position of point light 1 to the shader (Igloo)
	Position light1Position_cameraspace = viewStack.Top() * light[1].position;
	glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &light1Position_cameraspace.x);

	//axes
	RenderMesh(meshList[GEO_AXES], false);

	//changing day and night
	if (DayBreak)
	{
		//sunlight
		light[0].power = 1.5f;
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);

		//igloo night and fire light
		light[1].power = 0.0f;
		light[2].power = 0.0f;
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
		glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);

		//skybox
		modelStack.PushMatrix();
		RenderSkyBox();
		modelStack.PopMatrix();

		//sunlight
		modelStack.PushMatrix();
		modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
		RenderMesh(meshList[GEO_LIGHTBALL], Switch_LightBall);
		modelStack.PopMatrix();
	}

	if (!(DayBreak))
	{
		//sunlight
		light[0].power = 0.0f;
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);

		//igloo night and night light
		light[1].power = 5.0f;
		light[2].power = 0.3f;
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
		glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);

		//skybox
		modelStack.PushMatrix();
		RenderSkyBoxNight();
		modelStack.PopMatrix();

		//night light
		modelStack.PushMatrix();
		modelStack.Translate(light[2].position.x, light[2].position.y, light[2].position.z);
		RenderMesh(meshList[GEO_LIGHTBALL3], Switch_LightBall);
		modelStack.PopMatrix();
	}

	//textured ground mesh
	modelStack.PushMatrix();
	modelStack.Translate(0, -60, 0);
	modelStack.Scale(9000, 7000, 9000);
	RenderMesh(meshList[GROUND], true);
	modelStack.PopMatrix();

	//------------------------------------------
	//Character Rendering
	//------------------------------------------
	//Character
	modelStack.PushMatrix();
	modelStack.Translate(Walking_X, Jump_, Walking_Z);
	modelStack.Scale(10, 10, 10);
	modelStack.Rotate(RotateBody, 0, 1, 0);
	Character();
	Bag();
	modelStack.PopMatrix();

	//------------------------------------------
	//Environment Rendering
	//------------------------------------------
	//Log
	modelStack.PushMatrix();
	modelStack.Translate(200, -60, 0);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[Log], true);
	modelStack.PopMatrix();

	//Igloo
	modelStack.PushMatrix();
	modelStack.Translate(-400, -110, 0);
	modelStack.Scale(600, 600, 600);
	modelStack.Rotate(270, 0, 1, 0);
	RenderMesh(meshList[Igloo], true);
	modelStack.PopMatrix();

	//flag n flagpole
	modelStack.PushMatrix();
	modelStack.Translate(-200, -52, -250);
	modelStack.Scale(50, 50, 50);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[FLAGPOLE], true);

	modelStack.PushMatrix();
	modelStack.Translate(0, MoveFlag, 0);
	RenderMesh(meshList[FLAG], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();//flagpole

	//StrawHat
	modelStack.PushMatrix();
	modelStack.Translate(0, -100, 400);
	modelStack.Scale(100, 100, 100);
	RenderMesh(meshList[STRAWHAT], true);
	modelStack.PopMatrix();//strawhat

	//Sled
	modelStack.PushMatrix();
	modelStack.Translate(-300, -70, 350);
	modelStack.Scale(100, 100, 100);
	RenderMesh(meshList[SLED], true);
	modelStack.PopMatrix();//sled

	//Meat
	modelStack.PushMatrix();
	modelStack.Translate(250, -50, 0);
	modelStack.Scale(30, 30, 30);
	RenderMesh(meshList[MEAT], true);
	modelStack.PopMatrix();//Meat


	//Snowman : left corner
	modelStack.PushMatrix();
	modelStack.Translate(400, 65, -500);
	modelStack.Rotate(135, 0, 1, 0);
	modelStack.Scale(5, 5, 5);
	Snowman();
	modelStack.PopMatrix();

	//Snowman : bottom left corner
	modelStack.PushMatrix();
	modelStack.Translate(-500, 10, -500);
	modelStack.Rotate(225, 0, 1, 0);
	modelStack.Scale(3, 3, 3);
	Snowman();
	modelStack.PopMatrix();

	//Snowman : right top
	modelStack.PushMatrix();
	modelStack.Translate(-400, 10, 500);
	modelStack.Rotate(45, 0, 1, 0);
	modelStack.Scale(3, 3, 3);
	Snowman();
	modelStack.PopMatrix();

	//Snowman : right bottom
	modelStack.PushMatrix();
	modelStack.Translate(300, 10, 500);
	modelStack.Scale(3, 3, 3);
	Snowman();
	modelStack.PopMatrix();


	//Left side of the boundary
	modelStack.PushMatrix();
	Tree_Boundary();

	//Right side
	modelStack.PushMatrix();
	modelStack.Translate(-1820, 0, 0);
	Tree_Boundary();
	modelStack.PopMatrix();

	//front of boundary
	modelStack.PushMatrix();
	modelStack.Rotate(270, 0, 1, 0);
	Tree_Boundary();
	modelStack.PopMatrix();
	modelStack.PopMatrix();//left


	//Drum Mountains : Bottom left
	modelStack.PushMatrix();
	modelStack.Translate(-1500, 400, -500);
	Drum_Island();
	modelStack.PopMatrix();

	//Drum Mountain : Behind Left
	modelStack.PushMatrix();
	modelStack.Translate(-1500, 400, 500);
	Drum_Island();
	modelStack.PopMatrix();

	//Drum Mountain : Behind Right
	modelStack.PushMatrix();
	modelStack.Translate(1500, 400, 500);
	Drum_Island();
	modelStack.PopMatrix();

	//Drum Mountain : Font Right
	modelStack.PushMatrix();
	modelStack.Translate(1500, 400, -500);
	Drum_Island();
	modelStack.PopMatrix();

	//Tree shake
	modelStack.PushMatrix();
	modelStack.Translate(500, -50, 0);
	modelStack.Scale(2, 2, 2);
	modelStack.Rotate(ShakeTree, 1, 0, 0);
	Tree();
	modelStack.PopMatrix();//tree

	//snowball
	if (!(PickUp))
	{
		modelStack.PushMatrix();
		modelStack.Translate(500, SnowballFallY, SnowballFallZ);
		modelStack.Scale(2, 2, 2);
		RenderMesh(meshList[SNOW], true);
		modelStack.PopMatrix();//snowball
	}
	else
	{
		modelStack.Translate(camera.position.x + (camera.target.x - camera.position.x),
			camera.position.y + ((camera.target.y - camera.position.y) - 0.5),
			camera.position.z + (camera.target.z - camera.position.z));
		modelStack.Scale(0.2, 0.2, 0.2);
		RenderMesh(meshList[SNOW], true);
		modelStack.PopMatrix();//snowball

		Fall = false;
	}

	//Flower
	modelStack.PushMatrix();
	modelStack.Translate(-500, -55, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[FLOWER], true);
	modelStack.PopMatrix();

	//=================================
	//Text on the screen
	//=================================
	modelStack.PushMatrix();
	string frames = "FPS: " + std::to_string(fps);
	string x = "x: " + std::to_string((int)camera.position.x);
	string y = "y: " + std::to_string((int)camera.position.y);
	string z = "z: " + std::to_string((int)camera.position.z);
	RenderTextOnScreen(meshList[GEO_TEXT], frames, Color(0, 0, 0), 2, 0, 29);
	RenderTextOnScreen(meshList[GEO_TEXT], "Welcome to Chopper's Mini World!!", Color(0, 0, 0), 2, 0, 28);
	RenderTextOnScreen(meshList[GEO_TEXT], "Interact and see what happens!", Color(0, 0, 0), 2, 0, 27);

	//xyz
	RenderTextOnScreen(meshList[GEO_TEXT], x, Color(0, 0, 0), 2, 0, 4);
	RenderTextOnScreen(meshList[GEO_TEXT], y, Color(0, 0, 0), 2, 0, 3);
	RenderTextOnScreen(meshList[GEO_TEXT], z, Color(0, 0, 0), 2, 0, 2);

	//flagpole and flag
	if (camera.position.x < -100 && camera.position.x > -300
		&& camera.position.z < -150 && camera.position.z > -350)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Press 'E' to raise flag", Color(0, 0, 0), 2, 1, 1);
	}

	//Skaking tree
	if (camera.position.x > 400 && camera.position.x < 600
		&& camera.position.z > -100 && camera.position.z < 100)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Hold 'E' to Shake Tree!", Color(0, 0, 0), 2, 1, 1);
	}

	//Flower to change light
	if (camera.position.x < -400 && camera.position.x > -600
		&& camera.position.z < 100 && camera.position.z > -100)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Press 'X' for DayTime", Color(0, 0, 0), 2, 1, 2);
		RenderTextOnScreen(meshList[GEO_TEXT], "Press 'E' for NightTime", Color(0, 0, 0), 2, 1, 1);
	}

	//E appearing above snowball 
	if (Fall && camera.position.x > 450 && camera.position.x < 550
		&& camera.position.z > -250 && camera.position.z < -150)
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_SCREEN], "Look Down! Pick up?", Color(0, 0, 0), 2, 1, 1);
		modelStack.PopMatrix();
	}

	//interaction with chopper
	if (camera.position.x > -100 && camera.position.x < 100
		&& camera.position.z  > -100 && camera.position.z < 100)
	{
		if (Talk)
		{
			modelStack.PushMatrix();
			RenderTextOnScreen(meshList[GEO_SCREEN], "I'm not a tanuki, I'm a reindeer!", Color(0, 0, 0), 2, 1, 1);
			modelStack.PopMatrix();
		}
		else
		{
			RenderTextOnScreen(meshList[GEO_SCREEN], "Talk to chopper!", Color(0, 0, 0), 2, 1, 1);
		}
	}
	else
	{
		Talk = false;
	}
}

void Assignment3::RenderSkyBox()
{
	//sky box
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
	modelStack.Translate(0, -1250, 0);
	modelStack.Scale(3000, 3000, 3000);

	//Ground
	modelStack.PushMatrix();
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	//Front
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.498, 0.498);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	//Top
	modelStack.PushMatrix();
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Translate(0, 0.98, 0);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	//Back
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.498, -0.498);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	//Left
	modelStack.PushMatrix();
	modelStack.Translate(0.498, 0.4975, 0);
	modelStack.Rotate(270, 0, 1, 0);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	//Right
	modelStack.PushMatrix();
	modelStack.Translate(-0.498, 0.498, 0);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();//skybox
}

void Assignment3::RenderSkyBoxNight()
{
	//sky box
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
	modelStack.Translate(0, -1250, 0);
	modelStack.Scale(3000, 3000, 3000);

	//Ground
	modelStack.PushMatrix();
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	//Front
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.498, 0.498);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_FRONTNight], false);
	modelStack.PopMatrix();

	//Top
	modelStack.PushMatrix();
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Translate(0, 0.98, 0);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_TOPNight], false);
	modelStack.PopMatrix();

	//Back
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.498, -0.498);
	RenderMesh(meshList[GEO_BACKNight], false);
	modelStack.PopMatrix();

	//Left
	modelStack.PushMatrix();
	modelStack.Translate(0.498, 0.4975, 0);
	modelStack.Rotate(270, 0, 1, 0);
	RenderMesh(meshList[GEO_LEFTNight], false);
	modelStack.PopMatrix();

	//Right
	modelStack.PushMatrix();
	modelStack.Translate(-0.498, 0.498, 0);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_RIGHTNight], false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();//skybox
}

void Assignment3::RenderHead()
{
	//Head
	modelStack.PushMatrix();
	modelStack.Translate(0, 17.3, 0);
	modelStack.Scale(8, 8, 8);
	RenderMesh(meshList[HEAD], true);
	modelStack.PopMatrix();//head

	//Ears left
	modelStack.PushMatrix();
	modelStack.Translate(9, 19.8, 0);
	modelStack.Rotate(-50, 0, 0, 1);
	RenderMesh(meshList[EARS], true);
	modelStack.PopMatrix();//Ears left

	//Ears right
	modelStack.PushMatrix();
	modelStack.Translate(-9, 19.8, 0);
	modelStack.Rotate(50, 0, 0, 1);
	RenderMesh(meshList[EARS], true);
	modelStack.PopMatrix();//Ears right

	//Horns base(left)
	modelStack.PushMatrix();
	modelStack.Translate(8, 21.3, 0);
	modelStack.Rotate(300, 0, 0, 1);
	modelStack.Scale(1, 7, 1);
	RenderMesh(meshList[HORNS], true);
	modelStack.PopMatrix();

	//Horns base(right)
	modelStack.PushMatrix();
	modelStack.Translate(-8, 21.3, 0);
	modelStack.Rotate(60, 0, 0, 1);
	modelStack.Scale(1, 7, 1);
	RenderMesh(meshList[HORNS], true);
	modelStack.PopMatrix();

	//------------------------
	//facial expressions
	//------------------------
	//Eyes Left : Scelera
	modelStack.PushMatrix();
	modelStack.Translate(3, 15, -6.5);
	modelStack.Scale(1.5, 1.5, 1.5);
	modelStack.Rotate(345, 0, 1, 0);
	modelStack.Rotate(255, 1, 0, 0);
	RenderMesh(meshList[EYES], true);

	//Eyes Left : Pupil
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.5, 0);
	modelStack.Scale(0.75, 0.75, 0.75);
	RenderMesh(meshList[PUPIL], true);
	modelStack.PopMatrix();//Eyes : Pupil
	modelStack.PopMatrix();//Eyes : Scelera

	//Eyes Right : Scelera
	modelStack.PushMatrix();
	modelStack.Translate(-3, 15, -6.5);
	modelStack.Scale(1.5, 1.5, 1.5);
	modelStack.Rotate(15, 0, 1, 0);
	modelStack.Rotate(255, 1, 0, 0);
	RenderMesh(meshList[EYES], true);

	//Eyes Right: Pupil
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.5, 0);
	modelStack.Scale(0.75, 0.75, 0.75);
	RenderMesh(meshList[PUPIL], true);
	modelStack.PopMatrix();//Eyes : Pupil
	modelStack.PopMatrix();//Eyes : Scelera

	//Nose
	modelStack.PushMatrix();
	modelStack.Translate(0, 14, -7);
	modelStack.Scale(0.7, 0.7, 0.7);
	modelStack.Rotate(270, 1, 0, 0);
	RenderMesh(meshList[NOSE], true);
	modelStack.PopMatrix();//Nose

	//Mouth
	modelStack.PushMatrix();
	modelStack.Translate(0, 13, -6);
	modelStack.Rotate(340, 1, 0, 0);
	modelStack.Scale(2, 2, 1);
	modelStack.Rotate(180, 0, 0, 1);
	RenderMesh(meshList[MOUTH], true);
	modelStack.PopMatrix();//Mouth
}

void Assignment3::RenderBody()
{
	//Body (upper body)
	modelStack.PushMatrix();
	modelStack.Translate(0, 7.3, 0);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[UPPER_BODY], true);

	//Body (pants)
	modelStack.PushMatrix();
	modelStack.Rotate(180, 1, 0, 0);
	RenderMesh(meshList[LOWER_BODY], true);
	modelStack.PopMatrix();//pants
	modelStack.PopMatrix();//upper body

	////Defence mode Chopper
	//modelStack.PushMatrix();
	//modelStack.Translate(0, 7.3, 0);
	//modelStack.Scale(Transform, Transform, Transform);
	//RenderMesh(meshList[HEAD], true);
	//modelStack.PopMatrix();//defence mode
}

void Assignment3::RenderHat()
{
	//Hat top
	modelStack.PushMatrix();
	modelStack.Translate(0, 23.8, 0);
	modelStack.Scale(8, 2, 8);
	RenderMesh(meshList[HAT], true);
	modelStack.PopMatrix();//hat top

	//cross on hat (left)
	modelStack.PushMatrix();
	modelStack.Translate(0, 23.8, -8);
	modelStack.Rotate(45, 0, 0, 1);
	modelStack.Scale(1, 5, 1);
	RenderMesh(meshList[HAT_CROSS], true);
	modelStack.PopMatrix();//cross (left)

	//cross (right)
	modelStack.PushMatrix();
	modelStack.Translate(0, 23.8, -8);
	modelStack.Rotate(-45, 0, 0, 1);
	modelStack.Scale(1, 5, 1);
	RenderMesh(meshList[HAT_CROSS], true);
	modelStack.PopMatrix();//cross (right)

	//Hat (flap)
	modelStack.PushMatrix();
	modelStack.Translate(0, 17.8, 0);
	modelStack.Scale(15, 0.4, 15);
	RenderMesh(meshList[HAT], true);
	modelStack.PopMatrix();//flap
}

void Assignment3::RenderArm_Right()
{
	//Right Arm
	modelStack.PushMatrix();
	modelStack.Translate(4.5, 8.05, 0);
	modelStack.Rotate(Walking_Rotation_Left, 1, 0, 0);
	
	//Arm
	modelStack.PushMatrix();
	modelStack.Rotate(40, 0, 0, 1);
	RenderMesh(meshList[LIMBS], true);
	modelStack.PopMatrix();//Arm

	//Upper Hand (left)
	modelStack.PushMatrix();
	modelStack.Translate(1.7, -2.25, 0);
	modelStack.Rotate(30, 0, 0, 1);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[HAND], true);

	//Lower Hand (left)
	modelStack.PushMatrix();
	modelStack.Rotate(180, 0, 0, 1);
	RenderMesh(meshList[HAND_LOWER], true);

	//Hooves 
	modelStack.PushMatrix();
	modelStack.Translate(-0.3, 0.5, -0.5);
	modelStack.Scale(0.5, 0.65, 1);
	modelStack.Rotate(180, 1, 0, 0);
	RenderMesh(meshList[HAND_LOWER], true);
	modelStack.PopMatrix();//Hooves

	//Hooves (left)
	modelStack.PushMatrix();
	modelStack.Translate(0.3, 0.5, -0.5);
	modelStack.Scale(0.5, 0.65, 1);
	modelStack.Rotate(180, 1, 0, 0);
	RenderMesh(meshList[HAND_LOWER], true);
	modelStack.PopMatrix();//Hooves (left)

	modelStack.PopMatrix();//Lower Hand (left)
	modelStack.PopMatrix();//Upper Hand (left)
	modelStack.PopMatrix();//Left Arm
}

void Assignment3::RenderArm_Left()
{
	//left Arm
	modelStack.PushMatrix();
	modelStack.Translate(-4.5, 8.05, 0);
	modelStack.Rotate(Walking_Rotation_Right, 1, 0, 0);

	//Arm
	modelStack.PushMatrix();
	modelStack.Rotate(140, 0, 0, 1);
	RenderMesh(meshList[LIMBS], true);
	modelStack.PopMatrix();//Arm

	//Upper Hand (right)
	modelStack.PushMatrix();
	modelStack.Translate(-1.8, -2.25, 0);
	modelStack.Rotate(330, 0, 0, 1);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[HAND], true);

	//Lower Hand (right)
	modelStack.PushMatrix();
	modelStack.Rotate(180, 0, 0, 1);
	RenderMesh(meshList[HAND_LOWER], true);

	//Hooves 
	modelStack.PushMatrix();
	modelStack.Translate(-0.3, 0.5, -0.5);
	modelStack.Scale(0.5, 0.65, 1);
	modelStack.Rotate(180, 1, 0, 0);
	RenderMesh(meshList[HAND_LOWER], true);

	modelStack.PopMatrix();//Hooves

	//Hooves (left)
	modelStack.PushMatrix();
	modelStack.Translate(0.3, 0.5, -0.5);
	modelStack.Scale(0.5, 0.65, 1);
	modelStack.Rotate(180, 1, 0, 0);
	RenderMesh(meshList[HAND_LOWER], true);
	modelStack.PopMatrix();//Hooves (left)

	modelStack.PopMatrix();//Lower Hand (right)
	modelStack.PopMatrix();//Upper Hand (right)
	modelStack.PopMatrix();//Right Arm
}

void Assignment3::RenderLeg_Right()
{
	//Left leg
	modelStack.PushMatrix();
	modelStack.Translate(2, 3.3, 0);
	modelStack.Rotate(Walking_Rotation_Right, 1, 0, 0);

	//Thigh
	modelStack.PushMatrix();
	modelStack.Rotate(10, 0, 0, 1);
	modelStack.Scale(1, 2, 1);
	RenderMesh(meshList[LEGS], true);
	modelStack.PopMatrix();//Thigh

	//Feet (left)
	modelStack.PushMatrix();
	modelStack.Translate(0.3, -3.3, -0.3);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[HAND], true);

	//Hooves 
	modelStack.PushMatrix();
	modelStack.Translate(-0.3, 0.05, -0.5);
	modelStack.Scale(0.5, 0.65, 1);
	RenderMesh(meshList[HAND_LOWER], true);
	modelStack.PopMatrix();//Hooves

	//Hooves (left)
	modelStack.PushMatrix();
	modelStack.Translate(0.3, 0.05, -0.5);
	modelStack.Scale(0.5, 0.65, 1);
	RenderMesh(meshList[HAND_LOWER], true);
	modelStack.PopMatrix();//Hooves (left)

	modelStack.PopMatrix();//Feet (left)
	modelStack.PopMatrix();//Left leg
}

void Assignment3::RenderLeg_Left()
{
	//Right leg
	modelStack.PushMatrix();
	modelStack.Translate(-2, 3.3, 0);
	modelStack.Rotate(Walking_Rotation_Left, 1, 0, 0);

	//Thigh
	modelStack.PushMatrix();
	modelStack.Rotate(-10, 0, 0, 1);
	modelStack.Scale(1, 2, 1);
	RenderMesh(meshList[LEGS], true);
	modelStack.PopMatrix();//Thigh

	//Feet (right)
	modelStack.PushMatrix();
	modelStack.Translate(-0.3, -3.3, -0.3);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[HAND], true);

	//Hooves 
	modelStack.PushMatrix();
	modelStack.Translate(-0.3, 0.05, -0.5);
	modelStack.Scale(0.5, 0.65, 1);
	RenderMesh(meshList[HAND_LOWER], true);
	modelStack.PopMatrix();//Hooves

	//Hooves (left)
	modelStack.PushMatrix();
	modelStack.Translate(0.3, 0.05, -0.5);
	modelStack.Scale(0.5, 0.65, 1);
	RenderMesh(meshList[HAND_LOWER], true);
	modelStack.PopMatrix();//Hooves (left)

	modelStack.PopMatrix();//Feet (right)
	modelStack.PopMatrix();//Right leg
}

void Assignment3::Bag()
{
	//Bag
	modelStack.PushMatrix();
	modelStack.Translate(0, -2.85, 1.15);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(3.6, 1.8, 3);
	RenderMesh(meshList[BAG], true);

	//Bag Handle : Left
	modelStack.PushMatrix();
	modelStack.Translate(-0.4, 0.3, 0);
	modelStack.Scale(0.1, 0.2, 0.1);
	modelStack.Rotate(90, 0, 0, 1);
	RenderMesh(meshList[BAG_HANDLE], true);
	modelStack.PopMatrix();//Bag Handle : Left

	//Bag Handle : Right
	modelStack.PushMatrix();
	modelStack.Translate(0.4, 0.3, 0);
	modelStack.Scale(0.1, 0.2, 0.1);
	modelStack.Rotate(90, 0, 0, 1);
	RenderMesh(meshList[BAG_HANDLE], true);
	modelStack.PopMatrix();//Bag Handle : Right

	modelStack.PopMatrix();//Bag
}

void Assignment3::Character()
{
	//head rendering
	modelStack.PushMatrix();
	modelStack.Translate(0,-5.2,0);
	modelStack.Scale(0.3,0.3,0.3);
	RenderHead();
	RenderHat();//hat rendering

	//body rendering 
	modelStack.PushMatrix();
	RenderBody();

	//Left arm rendering
	modelStack.PushMatrix();
	RenderArm_Left();
	modelStack.PopMatrix();//Left arm

	//Right Arm rendering
	modelStack.PushMatrix();
	RenderArm_Right();
	modelStack.PopMatrix();//Right Arm

	//Left Leg rendering 
	modelStack.PushMatrix();
	RenderLeg_Left();
	modelStack.PopMatrix();//Left Leg

	//Right Leg rendering 
	modelStack.PushMatrix();
	RenderLeg_Right();
	modelStack.PopMatrix();//Right Leg

	modelStack.PopMatrix();//Body
	modelStack.PopMatrix();//Head
}

void Assignment3::Tree()
{
	modelStack.PushMatrix();
	modelStack.Scale(1.5,1.5,1.5);

	//Tree Trunk
	modelStack.PushMatrix();
	modelStack.Translate(0, 30, 0);
	modelStack.Scale(5, 30, 5);
	RenderMesh(meshList[HORNS], true);
	modelStack.PopMatrix();

	//Tree Leaves : Top layer
	modelStack.PushMatrix();
	modelStack.Translate(0, 55, 0);
	modelStack.Scale(17, 30, 17);
	RenderMesh(meshList[TREE_LEAVES], true);

	//Tree Leaves : Bottom layer
	modelStack.PushMatrix();
	modelStack.Translate(0, -0.5, 0);
	RenderMesh(meshList[TREE_LEAVES], true);
	modelStack.PopMatrix();//Tree Leaves : Bottom layer
	modelStack.PopMatrix();//Tree Leaves : Top layer

	//Snow piled under tree
	modelStack.PushMatrix();
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[SNOW_UNDER], true);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void Assignment3::Snowman()
{
	//Snowman : Head
	modelStack.PushMatrix();
	modelStack.Scale(7, 7, 7);
	RenderMesh(meshList[SNOW], true);
	modelStack.PopMatrix();//Head

	//Snowman : Body
	modelStack.PushMatrix();
	modelStack.Translate(0, -14.5, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[SNOW], true);
	modelStack.PopMatrix();//Body

	//Snowman : Nose
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -8);
	modelStack.Rotate(270, 1, 0, 0);
	modelStack.Scale(1, 4, 1);
	RenderMesh(meshList[CARROT], true);
	modelStack.PopMatrix();//Nose

	//Snowman : Eyes Left
	modelStack.PushMatrix();
	modelStack.Translate(2, 2, -6);
	modelStack.Rotate(270, 1, 0, 0);
	RenderMesh(meshList[PUPIL], true);
	modelStack.PopMatrix();//Eyes Left

	//Snowman : Eyes Right
	modelStack.PushMatrix();
	modelStack.Translate(-2, 2, -6);
	modelStack.Rotate(270, 1, 0, 0);
	RenderMesh(meshList[PUPIL], true);
	modelStack.PopMatrix();//Eyes Right

	//Snowman : Buttons
	modelStack.PushMatrix();
	modelStack.Translate(0, -8.5, -7.5);
	modelStack.Rotate(270, 1, 0, 0);
	RenderMesh(meshList[PUPIL], true);

	modelStack.PushMatrix();
	modelStack.Translate(0, 1, -2);
	RenderMesh(meshList[PUPIL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 2, -4);
	RenderMesh(meshList[PUPIL], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();//Buttons
}

void Assignment3::Tree_Boundary()
{
	int Distance = 0;
	for (int i = 0; i < 9; i++)
	{
		Distance += 200;
		modelStack.PushMatrix();
		modelStack.Translate(900, -60, -970 + Distance);
		modelStack.Scale(3, 3, 3);
		Tree();
		modelStack.PopMatrix();
	}
}

void Assignment3::Drum_Island()
{
	modelStack.PushMatrix();
	modelStack.Scale(180, 300, 180);
	RenderMesh(meshList[DRUM_MOUNTAIN], true);

	modelStack.PushMatrix();//snow under mountain
	modelStack.Translate(0, 1.5, 0);
	modelStack.Scale(0.25, 0.1, 0.25);
	RenderMesh(meshList[SNOW_UNDER], true);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void Assignment3::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void Assignment3::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)//x = translate by x;
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);

	//Add these code just after glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	//Add these code just before glEnable(GL_DEPTH_TEST);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void Assignment3::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight && Switch)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}

	mesh->Render(); //this line should only be called once

	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Assignment3::Exit()
{
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}