#pragma once
#include <Component/SimpleScene.h>
#include "Camera.h"
#include "Transform3D.h"
#include "Platforms.h"

#include <vector>

#define POV3Y 1.25 // camera->distanceToTargetY + PLAYERHITRANGE
#define POV3Z 3.25 // camera->distanceToTargetZ + PLAYERHITRANGE
#define POV1Y 0.7
#define POV1Z 2.5

#define WSACCELERATION 4
#define PLAYERHITRANGE 0.25 // player = sphere of diameter 1, scaled to 0.5

#define MONITOR 0
#define NORMALDISPLAY 1
#define ORANGEEFFECT 2
#define GREENEFFECT 3
#define YELLOWEFFECT 4
#define PLUSEFFECT 5
#define MINUSEFFECT 6
#define GHOSTEFFECT 7
#define ORANGEGHOST 8
#define HITEFFECT 9

class EndlessSkyroads : public SimpleScene
{
public:
	EndlessSkyroads();
	~EndlessSkyroads();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	Mesh* CreateFuelbar(std::string name);
	Mesh* CreateLife(std::string name);
	void Scoot(float deltaTimeSeconds);
	void Jump(float deltaTimeSeconds);
	void AdvanceForward(float deltaTimeSeconds);
	void AnimationSetup();
	bool IsOnPlatform(int i);
	bool HasHitPlatform(int i);
	bool HasHitWall(int i);
	void RenderPlatform(int i);
	void RenderWall(float deltaTimeSeconds, int i);
	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, const int objectCode,
		const glm::vec3& objectPos, const glm::vec3& objectSize);
	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	SkyroadsAux::Camera* camera;
	glm::mat4 projectionMatrix;
	bool renderPlayer;
	int playerStatus;

	SkyroadsAux::Platforms* platforms;
	
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
}; 
