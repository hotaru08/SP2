#ifndef Assignment3_H
#define Assignment3_H

#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"

class Assignment3 : public Scene
{
	enum GEOMETRY_TYPE
	{
		//shapes
		GEO_AXES,
		GEO_LIGHTBALL,
		GEO_LIGHTBALL2,
		GEO_LIGHTBALL3,

		//skybox Day
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,

		//skybox Night
		GEO_LEFTNight,
		GEO_RIGHTNight,
		GEO_TOPNight,
		GEO_FRONTNight,
		GEO_BACKNight,

		//character shapes
		UPPER_BODY,
		LOWER_BODY,
		HEAD,
		EARS,
		HORNS,
		LIMBS,
		LEGS,
		HAND,
		HAND_LOWER,
		FEET,

		//facial expression
		EYES,
		NOSE,
		MOUTH,
		PUPIL,

		//Item
		HAT,
		HAT_CROSS,
		BAG_HANDLE,
		BAG,

		//environment
		GROUND,
		GEO_TEXT,
		GEO_SCREEN,
		Log,
		Igloo,
		DRUM_MOUNTAIN,
		SNOW,
		CARROT,
		FLAGPOLE,
		FLAG,
		SLED,
		MEAT,
		STRAWHAT,
		FLOWER,
		TREE_LEAVES,
		SNOW_UNDER,

		NUM_GEOMETRY,
	};

	//First Light
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,

		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,
		U_LIGHT2_TYPE,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,

		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,

		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_TOTAL,
	};

public:
	Assignment3();
	~Assignment3();

	float fps;

	//methods
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	//parameters
private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	float LSPEED;
	bool Switch;
	bool Switch_LightBall;

	Camera3 camera;
	MS modelStack, viewStack, projectionStack;
	Light light[4];

	//=============================
	//Character Animation
	//=============================
	float Jump_;
	float Walking_X;
	float Walking_Z;
	float Walking_Rotation_Left;
	float Walking_Rotation_Right;
	float RotateBody;

	//=============================
	//Interaction
	//=============================
	//flag
	float MoveFlag;
	bool RaiseDown;
	bool RaiseUp;

	//shake tree
	float ShakeTree;
	float SnowballFallY; // the speed snowball fall by
	float SnowballFallZ; // the speed snowball fall by

	bool Fall; //for snowball to fall
	bool PickUp; //pickup snowball

	//skybox
	bool DayBreak = true;
	bool NightFall = false;

	//talk
	bool Talk;

	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderSkyBox();
	void RenderSkyBoxNight();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);


	//=============================
	//Rendering
	//=============================
	void RenderBody();
	void RenderHead();
	void RenderHat();
	void RenderArm_Left();
	void RenderArm_Right();
	void RenderLeg_Left();
	void RenderLeg_Right();
	void Character();
	void Item();
	void Snowman();
	void Drum_Island();
	void Tree();
	void Bag();
	void Tree_Boundary();
};

#endif