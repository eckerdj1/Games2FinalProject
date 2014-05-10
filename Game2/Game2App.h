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

// Ray-Bounding box intersection
// http://people.csail.mit.edu/amy/papers/box-jgt.pdf
class Ray 
{
public:
	Ray(Vector3 &o, Vector3 &d) {
		origin = o;
		direction = d;
		inv_direction = Vector3(1/d.x, 1/d.y, 1/d.z);
		sign[0] = (inv_direction.x < 0);
		sign[1] = (inv_direction.y < 0);
		sign[2] = (inv_direction.z < 0);
	}
	Vector3 origin;
	Vector3 direction;
	Vector3 inv_direction;
	int sign[3];
};

class Box2 {
public:
	Box2(const Vector3 &min, const Vector3 &max) {
		assert(min < max);
		bounds[0] = min;
		bounds[1] = max;
	}
	bool intersect(const Ray &, float t0, float t1);
	Vector3 bounds[2];
};

struct PlayState {
	int level;
	int pickUpsRemaining;
	bool completedLevel;
	bool newLevel;
	int livesRemaining;
	PlayState(){}
	PlayState(int l, int pUR, bool cL) {
		level = l;
		pickUpsRemaining = pUR;
		completedLevel = cL;
		newLevel = true;
	}
};

enum GameState {PLAY, TITLE, HOWTO, LEVELWIN, GAMEWIN, LEVELLOSE, GAMEOVER, CREDITS, LOADING};

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

private:
	void buildFX();
	void buildVertexLayouts();
	void setNewObstacleCluster();
 
private:

	vector<GameObject> fallingBlocks;
	vector<GameObject> bullets;
	Vector3 left, right, forward, back, up, down, zero;

	Quad splash;
	bool splashScreenIsUp;

	Box* playerBox;
	Box* enemyBox;
	Box* wallBox;

	GameObject outline[11];
	Player player;
	Level* level;
	Level* level1;
	Level* level2;
	Level* level3;
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

	ID3D10ShaderResourceView* mDiffuseMapRV;
	ID3D10ShaderResourceView* mSpecMapRV;

	ID3D10ShaderResourceView* mFloorTex;
	ID3D10ShaderResourceView* mFloorSpec;
	
	ID3D10ShaderResourceView* mTitleSplash;
	ID3D10ShaderResourceView* mControlsSplash;
	ID3D10ShaderResourceView* mHowToSplash;
	ID3D10ShaderResourceView* mWinSplash;
	ID3D10ShaderResourceView* mCreditsSplash;
	ID3D10ShaderResourceView* mLoadSplash;
	ID3D10ShaderResourceView* mGameOverSplash;

	ID3D10ShaderResourceView* mSplashSpec;

	ID3D10EffectShaderResourceVariable* mfxDiffuseMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecMapVar;

	ID3D10EffectMatrixVariable* mfxWVPVar;
	//light variables
	ID3D10EffectMatrixVariable* mfxWorldVar;
	ID3D10EffectVariable* mfxEyePosVar;
	vector<ID3D10EffectVariable*> mfxLightVar;
	vector<ID3D10EffectVariable*> mfxSpotVars;
	ID3D10EffectVariable* mfxSpotVar;
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
	float camTheta;
	float camPhi;
	float camTurnSpeed;
	float camZoom;
	float zoomSpeed;
	float maxZoom, minZoom;
	Vector2 mousePos, lastMousePos;

	//my edits
	D3DXMATRIX worldBox1, worldBox2;

	float mTheta;
	float mPhi;

	float spotCounter;
	float timer;


};