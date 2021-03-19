#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include "Camera.h"
#include "Platforms.h"
#include "Objects.h"

#include <iostream>
#include <vector>
#include <string>

#define POV3Y 1.25 // camera->distanceToTargetY + PLAYERHITRANGE
#define POV3Z 3.25 // camera->distanceToTargetZ + PLAYERHITRANGE
#define POV1Y 0.7
#define POV1Z 2.5

#define WSACCELERATION 4
#define PLAYER_HIT_RANGE 0.25 // player = sphere of diameter 1, scaled to 0.5

#define MONITOR 0
#define NORMAL_DISPLAY 1
#define GOGO_EFFECT 2
#define PLUS_FUEL_EFFECT 3
#define MINUS_FUEL_EFFECT 4
#define PLUS_LIFE_EFFECT 5
#define MINUS_LIFE_EFFECT 6
#define GHOST_EFFECT 7
#define GOGO_GHOST 8
#define HIT_EFFECT 9
#define LIGHT_DISPLAY 10
#define PLAYER_ROLL 11
#define LEAF_DISPLAY 12

class ParkBall : public SimpleScene
{
public:
	ParkBall();
	~ParkBall();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void InitVars();

	void Scoot(float deltaTimeSeconds);
	void Jump(float deltaTimeSeconds);
	void AdvanceForward(float deltaTimeSeconds);
	void AnimationSetup();

	bool HasHitLeaf(int i);
	bool IsOnPlatform(int i);
	bool HasHitWall(int i);

	void RenderPlatform(int i);
	void RenderWall(float deltaTimeSeconds, int i);

	void LightRGB(glm::vec3& rgb);
	void RenderLights();
	void RenderLamppost(int i);
	void RenderGroundLamp(int i);

	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture, const glm::vec3& color,
		const int objectCode, const glm::vec3& objectPos, const glm::vec3& objectSize,
		float materialShininess, float materialKd, float materialKs);

	void ShowScore();

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	TextRenderer* Text;

	ParkBallAux::Camera* camera;
	glm::mat4 projectionMatrix;
	bool renderPlayer;
	int playerStatus;
	float playerAngle;

	ParkBallAux::Platforms* platforms;
	std::vector<ParkBallAux::LightLog> lights;
	std::vector<glm::vec3> leaves;
	float leafAngle;
	
	float speedMin;
	float speedMax;
	float speed;
	const float scootSpeed = 10;
	float startScootY;
	float startScootZ;
	float scootEndX;
	bool right;
	bool left;
	bool jump;
	const float jumpHight = 4;
	const float jumpLength = 5;
	float jumpSpeed;
	bool glide;
	float glideLength;
	bool rise;

	bool switchPov;
	float groundLevel;

	glm::vec3 whiteBarPos = { -0.975, 0.85, 0.1 };
	glm::vec3 whiteBarSize = { 0.1, 0.1, 0.1 };
	glm::vec3 fuelPos = { -0.975, 0.85, 0 };
	glm::vec3 fuelSize = { 0.1, 0.1, 0.1 };
	glm::vec3 livesPos = { 0.7, 0.9, 0 };
	glm::vec3 livesSize = { 0.08, 0.08, 0.08 };
	glm::vec3 noNeed = { 0, 0, 0 };

	bool onPlatform;
	bool gameEnd;

	bool deformed;
	float endEffect;

	const float maxFuel = 5;
	float fuel;
	float fuelGain;
	float fuelLose;
	int lives;

	bool falling;
	const float fallStop = -3;
	float fallAcelerate;
	int score;
	int highScore = 0;
	bool showed;

	bool stuckOnMaxSpeed;
	float maxSpeedStartTime;
	float maxSpeedStopTime;
	const float orangeActionTime = 20;

	bool goGhost;
	float goGhostStartTime;
	float goGhostStopTime;
	const float ghostActionTime = 10;

	bool effectAnimation;
	const float powerUpAnimationTime = 0.5;
	float powerUpAnimationStartTime;
	float powerUpAnimationStopTime;

	const float greenBoost = 5;
	const float yellowMinus = 5;

	float platformScaleX;
	float platformScaleY;

	float horisont;
	float lastLight;
	const float distBetweenLights = 5;
	const float distFromCenterLane = 5;
	const float hightNonSpot = 1.5;
	const float hightSpot = 3;
	const float cutoffAng = 40;
	glm::vec3 spotDirection = { 1, 1, 0 };

	std::unordered_map<std::string, Texture2D*> mapTextures;
	GLuint randomTextureID;

	// wall texture pool
	std::vector<std::string> wallTextures;

	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightDirections;
	std::vector<glm::vec3> lightColors;
	std::vector<float> cutOffAngles;
	std::vector<int> lightIsSpot;
}; 
