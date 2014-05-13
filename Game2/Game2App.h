#include "d3dApp.h"
#include "Box.h"
#include "GameObject.h"
#include "Line.h"
#include "Quad.h"
#include <d3dx9math.h>
#include "LineObject.h"
#include "Score.h"
#include "Player.h"
#include "Obstacle.h"
#include "Light.h"
#include "Floor.h"
#include "Level.h"
#include "textureclass.h"
#include "RayBox.h"

#include <math.h>
#include <ctime>
#include <vector>
#include <string>
using std::string;
using std::vector;
using std::time;
using std::srand;
using std::rand;


#define toString(x) Text::toString(x)



struct PlayState {
	int level;
	int pickUpsRemaining;
	bool completedLevel;
	bool newLevel;
	int livesRemaining;
	int health;
	PlayState()
	{
		health = 100;
	}
	PlayState(int l, int pUR, bool cL) {
		level = l;
		pickUpsRemaining = pUR;
		completedLevel = cL;
		newLevel = true;
		health = 100;
	}
};

enum GameState {PLAY, TITLE, HOWTO, LEVELWIN, GAMEWIN, LEVELLOSE, GAMEOVER, CREDITS, LOADING, INTRO, GUN, SWORD, REMINDER, LASTLEVEL, ENDSTORY};

class Game2App : public D3DApp
{
public:
	Game2App(HINSTANCE hInstance);
	~Game2App();

	void initApp();
	void onResize();
	void updateScene(float dt);
	void drawScene(); 

	void showSplash();

	int cameraMode;
	int firstPerson;
	int topDown;
	
	float camTheta;
	float camPhi;
	float camTurnSpeed;
	float camZoom;
	float zoomSpeed;
	float maxZoom, minZoom;

private:
	void buildFX();
	void buildHudFX();
	void buildVertexLayouts();
	void setNewObstacleCluster();
 
private:

	vector<GameObject> fallingBlocks;
	vector<GameObject> bullets;
	Vector3 left, right, forward, back, up, down, zero;

	Quad splash;
	Quad teleChargeBorder;
	Quad teleCharge;
	Quad healthBorder;
	Quad healthLevel;
	Vector3 teleHudPos;
	Vector3 healthHudPos;
	bool splashScreenIsUp;
	TextureClass teleChargeBorderTex, teleChargeTex, healthBorderTex, healthLevelTex;

	Vector3 targetDir;
	Vector3 targetRight;
	Vector3 targetUp;

	Box* playerBox;
	Box* enemyBox;
	Box* wallBox;
	Box* blackBox;

	GameObject outline[11];
	Player player;
	Level* level;
	Level* level1;
	Level* level2;
	Level* level3;
	Level* level4;
	Level* level5;
	Level* level6;
	Floor floor;
	PlayState playState;
	GameState gameState;
	int numberOfObstacles;
	vector<Box*> obstacleBoxes;
	vector<Obstacle> obstacles;




	float floorMovement;

	//Lighting
	vector<Light> lights;
	Light ambientLight;
	Light splashLight;
	Light spotLight;
	Light teleportLight;
	int numberOfLights;
	int numberOfSpotLights;
	int lightType; // 0-parallel, 1-pointlight, 2-spotlight
	bool useTex;

	bool wallDetectionIsOn;

	float fallRatePerSecond;
	float avgFallSpeed;
	float elapsed;
	float bulletElapsed;
	float bulletsPerSecond;

	float floorSectionLength;
	int floorClusterCounter;
	int floorClusterThreshold;
	int floorSpeedIncrease;
	

	int playerBlock;
	int lives;
	int ammo;
	bool lifeGained;
	bool activeMessage;
	bool matchMade;
	std::wstring message;
	float messageTimer;


	float totalDist;
	bool distSet;
	Score score;

	bool gameOver;
	bool spotted;

	//New Spectrum HUD stuff by Andy
	Box specHudBox[6];
	Box cursorBox;
	GameObject spectrum[6];
	GameObject cursor;

	float getMultiplier();


	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;

	
	ID3D10Effect* mFXHud;
	ID3D10EffectTechnique* mTechHud;
	ID3D10InputLayout* mVertexLayoutHud;

	TextureClass diffuseMap, specMap;

	TextureClass floorTexMap, floorSpecMap;

	TextureClass titleSplash, controlsSplash, howToSplash, winSplash;
	TextureClass creditsSplash, loadSplash, gameOverSplash, splashSpecMap;
	TextureClass storyIntro, storyGun, storySword, storyLastLevel, storyEnd;


	ID3D10EffectShaderResourceVariable* mfxDiffuseMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecMapVar;
	
	ID3D10EffectShaderResourceVariable* mfxDiffuseMapVarHud;
	ID3D10EffectMatrixVariable* mfxWVPVarHud;
	ID3D10EffectMatrixVariable* mfxWorldVarHud;
	ID3D10EffectMatrixVariable* mfxTexMtxVarHud;

	ID3D10EffectMatrixVariable* mfxWVPVar;
	//light variables
	ID3D10EffectMatrixVariable* mfxWorldVar;
	ID3D10EffectVariable* mfxEyePosVar;
	vector<ID3D10EffectVariable*> mfxLightVar;
	vector<ID3D10EffectVariable*> mfxSpotVars;
	ID3D10EffectVariable* mfxSpotVar;
	ID3D10EffectVariable* mfxTeleVar;
	ID3D10EffectVariable* mfxAmbientVar;
	ID3D10EffectScalarVariable* mfxLightType;
	ID3D10EffectScalarVariable* mfxLightCount;
	ID3D10EffectScalarVariable* mfxSpotCount;
	ID3D10EffectScalarVariable* mfxTexVar;
	ID3D10EffectMatrixVariable* mfxTexMtxVar;
	ID3D10EffectScalarVariable* mfxSplashLightVar;
	ID3D10EffectVariable* mfxSplashLight;



	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	D3DXMATRIX mVP;

	Vector3 CameraDirection;
	Vector3 camPos;
	Vector3 target;
	Vector3 splashCamPos;
	Vector3 splashPos;
	Vector3 splashTarget;
	Vector2 mousePos, lastMousePos;

	//my edits
	D3DXMATRIX worldBox1, worldBox2;

	Weapon* teleportGun;
	Weapon* sword;

	float mTheta;
	float mPhi;

	float spotCounter;
	float timer;
	bool audioNotStarted;
	bool highwayNotStarted;


};