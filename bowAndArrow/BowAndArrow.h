#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

class BowAndArrow : public SimpleScene
{
public:
	BowAndArrow();
	~BowAndArrow();

	void Init() override;

	struct ViewportSpace
	{
		ViewportSpace() : x(0), y(0), width(1), height(1) {}
		ViewportSpace(int x, int y, int width, int height)
			: x(x), y(y), width(width), height(height) {}
		int x;
		int y;
		int width;
		int height;
	};

	struct LogicSpace
	{
		LogicSpace() : x(0), y(0), width(1), height(1) {}
		LogicSpace(float x, float y, float width, float height)
			: x(x), y(y), width(width), height(height) {}
		float x;
		float y;
		float width;
		float height;
	};

	struct ObjectLog
	{
		ObjectLog() : x(0), y(0), speed(1), red(true), popped(false){}
		ObjectLog(float x, float y, float speed, bool red, bool popped)
			: x(x), y(y), speed(speed), red(red), popped(popped) {}
		float x;
		float y;
		float speed;
		bool red;
		bool popped;
	};

	struct PopStatus
	{
		PopStatus() : distructionOffset(0), fadeAway(0), done(false) {}
		PopStatus(float distructionOffset, float fadeAway, bool done)
			: distructionOffset(distructionOffset), fadeAway(fadeAway), done(done) {}
		float distructionOffset;
		float fadeAway;
		bool done;
	};

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;

	glm::mat3 VisualizationTransf2DUnif(const LogicSpace &logicSpace, const ViewportSpace &viewSpace);
	void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

	void InitGameObjects();

	void RenderBaloon(glm::vec2 position, bool red);
	void PopBaloon(int position, float deltaTimeSeconds);
	void PopShuriken(int position, float deltaTimeSeconds);
	void RenderArrow(glm::vec2 position, float angle);


protected:
	ViewportSpace viewSpace;
	LogicSpace logicSpace;
	glm::mat3 modelMatrix, visMatrix;

	float scaleMouseX, scaleMouseY;
	float globalMouseX, globalMouseY;
	float mouseAngle;

	float bowX, bowY;
	float bowAngle;

	float arrowX , arrowY;
	float arrowTipX, arrowTipY;
	float arrowAngle;
	bool fired = false;
	float arrowSpeed;
	float verticalOffset;

	bool go, greet;
	float score;
	int lives;
	int mode;
	std::vector<float> highscores;
	float inc;
	int maxSpeed;
	int badChance;

	std::vector<ObjectLog> baloons;
	std::vector<PopStatus> baloonPopStatus;
	std::vector<ObjectLog> shurikens;
	std::vector<PopStatus> shurikenPopStatus;
	float shurikenAngle = 0;
};
