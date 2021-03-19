#include "Tema1.h"

#include <vector>
#include <iostream>
#include <math.h>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Objects.h"

using namespace std;

BowAndArrow::BowAndArrow()
{
	scaleMouseX = 0;
	scaleMouseY = 0;
	globalMouseX = 0;
	globalMouseY = 0;
	mouseAngle = 0;

	bowX = 0.02;
	bowY = 0;
	bowAngle = 0;

	arrowX = 0;
	arrowY = 0;
	arrowTipX = 0;
	arrowTipY = 0;
	arrowAngle = 0;
	fired = false;
	arrowSpeed = 1;
	verticalOffset = 0;

	go = false;
	greet = false;
	score = 0;
	lives = 3;
	mode = 0;
	highscores = { 0, 0, 0 };
	inc = 0.1;
	maxSpeed = 150;
	badChance = 3;
}

BowAndArrow::~BowAndArrow()
{
}

void BowAndArrow::Init()
{
	auto camera = GetSceneCamera();
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::ivec2 resolution = window->GetResolution();

	logicSpace.x = 0;
	logicSpace.y = 0;
	logicSpace.width = (float)resolution.x / 160;
	logicSpace.height = (float)resolution.y / 160;
	scaleMouseX = logicSpace.width / (float)resolution.x;
	scaleMouseY = logicSpace.height / (float)resolution.y;

	Mesh* bow = Objects::CreateBow("bow", glm::vec3(0, 1, 0));
	AddMeshToList(bow);
	Mesh* arrowLine = Objects::CreateArrowLine("arrowLine", glm::vec3(0, 1, 0));
	AddMeshToList(arrowLine);
	Mesh* arrowTip = Objects::CreateArrowTip("arrowTip", glm::vec3(0, 1, 0));
	AddMeshToList(arrowTip);
	Mesh* speedBar = Objects::CreateSpeedBar("speedBar", glm::vec3(0, 1, 0));
	AddMeshToList(speedBar);
	Mesh* redBaloonHead = Objects::CreateBaloonHead("redBaloonHead", glm::vec3(1, 0, 0));
	AddMeshToList(redBaloonHead);
	Mesh* redBaloonBase = Objects::CreateBaloonBase("redBaloonBase", glm::vec3(1, 0, 0));
	AddMeshToList(redBaloonBase);
	Mesh* redBaloonString = Objects::CreateBaloonString("redBaloonString", glm::vec3(1, 0, 0));
	AddMeshToList(redBaloonString);
	Mesh* yellowBaloonHead = Objects::CreateBaloonHead("yellowBaloonHead", glm::vec3(1, 1, 0));
	AddMeshToList(yellowBaloonHead);
	Mesh* yellowBaloonBase = Objects::CreateBaloonBase("yellowBaloonBase", glm::vec3(1, 1, 0));
	AddMeshToList(yellowBaloonBase);
	Mesh* yellowBaloonString = Objects::CreateBaloonString("yellowBaloonString", glm::vec3(1, 1, 0));
	AddMeshToList(yellowBaloonString);
	Mesh* shuriken = Objects::CreateShuriken("shuriken", glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
	AddMeshToList(shuriken);
	Mesh* instaKill = Objects::CreateShuriken("instaKill", glm::vec3(0, 0, 0), glm::vec3(0, 1, 1));
	AddMeshToList(instaKill);
	Mesh* blade = Objects::CreateBlade("blade", glm::vec3(0, 0, 1));
	AddMeshToList(blade);
	Mesh* killBlade = Objects::CreateBlade("killBlade", glm::vec3(0, 1, 1));
	AddMeshToList(killBlade);
}

// uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 BowAndArrow::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
	float sx, sy, tx, ty, smin;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	if (sx < sy)
		smin = sx;
	else
		smin = sy;
	tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
	ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

	return glm::transpose(glm::mat3(
		smin, 0.0f, tx,
		0.0f, smin, ty,
		0.0f, 0.0f, 1.0f));
}

void BowAndArrow::SetViewportArea(const ViewportSpace &viewSpace, glm::vec3 colorColor, bool clear)
{
	glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	glEnable(GL_SCISSOR_TEST);
	glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
	GetSceneCamera()->Update();
}

void BowAndArrow::InitGameObjects()
{
	score = 0;
	lives = 3;
	go = true;

	bowY = logicSpace.height / 2;
	bowAngle = 0;
	arrowAngle = 0;
	fired = false;
	shurikenAngle = 0;

	float speed;
	for (float i = 2; i < logicSpace.width; i += 0.75)
	{
		if (baloons.size() != 0)
		{
			speed = abs((float)(rand() % maxSpeed) / 100 + 1 - baloons[baloons.size() - 1].x) + 0.2;
		}
		else
		{
			speed = (float)(rand() % maxSpeed) / 100 + 1;
		}
		if ((rand() % 10) >= badChance)
		{
			baloons.push_back(ObjectLog(i, 0, speed, true, false));
		}
		else
		{
			baloons.push_back(ObjectLog(i, 0, speed, false, false));
		}
		baloonPopStatus.push_back(PopStatus(0.25, 0.4, false));
	}

	for (float i = 0.5; i < logicSpace.height; i += 0.5)
	{
		if (shurikens.size() != 0)
		{
			speed = abs((float)(rand() % maxSpeed) / 100 + 1 - shurikens[shurikens.size() - 1].y) + 0.5;
		}
		else
		{
			speed = (float)(rand() % maxSpeed) / 100 + 1;
		}
		if ((rand() % 10) >= badChance)
		{
			shurikens.push_back(ObjectLog(logicSpace.width, i, speed, false, false));
		}
		else
		{
			shurikens.push_back(ObjectLog(logicSpace.width, i, speed, true, false));
		}
		shurikenPopStatus.push_back(PopStatus(0, 0.25, false));
	}
}

void BowAndArrow::RenderBaloon(glm::vec2 position, bool red)
{
	if (red)
	{
		modelMatrix = visMatrix * Transform2D::Translate(position[0], position[1]);
		modelMatrix *= Transform2D::Scale(0.25, 0.25);
		RenderMesh2D(meshes["redBaloonHead"], shaders["VertexColor"], modelMatrix);
		modelMatrix = visMatrix * Transform2D::Translate(position[0], position[1] - 0.4);
		modelMatrix *= Transform2D::Scale(0.25, 0.25);
		RenderMesh2D(meshes["redBaloonBase"], shaders["VertexColor"], modelMatrix);
		modelMatrix = visMatrix * Transform2D::Translate(position[0], position[1] - 0.4);
		RenderMesh2D(meshes["redBaloonString"], shaders["VertexColor"], modelMatrix);
	}
	else
	{
		modelMatrix = visMatrix * Transform2D::Translate(position[0], position[1]);
		modelMatrix *= Transform2D::Scale(0.25, 0.25);
		RenderMesh2D(meshes["yellowBaloonHead"], shaders["VertexColor"], modelMatrix);
		modelMatrix = visMatrix * Transform2D::Translate(position[0], position[1] - 0.4);
		modelMatrix *= Transform2D::Scale(0.25, 0.25);
		RenderMesh2D(meshes["yellowBaloonBase"], shaders["VertexColor"], modelMatrix);
		modelMatrix = visMatrix * Transform2D::Translate(position[0], position[1] - 0.4);
		RenderMesh2D(meshes["yellowBaloonString"], shaders["VertexColor"], modelMatrix);
	}
}

void BowAndArrow::PopBaloon(int position, float deltaTimeSeconds)
{
	if (baloons[position].red)
	{
		modelMatrix = visMatrix * Transform2D::Translate(baloons[position].x, baloons[position].y);
		modelMatrix *= Transform2D::Scale(baloonPopStatus[position].distructionOffset, baloonPopStatus[position].distructionOffset);
		RenderMesh2D(meshes["redBaloonHead"], shaders["VertexColor"], modelMatrix);
		modelMatrix = visMatrix * Transform2D::Translate(baloons[position].x, baloons[position].y - baloonPopStatus[position].fadeAway);
		modelMatrix *= Transform2D::Scale(0.25, 0.25);
		RenderMesh2D(meshes["redBaloonBase"], shaders["VertexColor"], modelMatrix);
		modelMatrix = visMatrix * Transform2D::Translate(baloons[position].x, baloons[position].y - baloonPopStatus[position].fadeAway);
		RenderMesh2D(meshes["redBaloonString"], shaders["VertexColor"], modelMatrix);
	}
	else
	{
		modelMatrix = visMatrix * Transform2D::Translate(baloons[position].x, baloons[position].y);
		modelMatrix *= Transform2D::Scale(baloonPopStatus[position].distructionOffset, baloonPopStatus[position].distructionOffset);
		RenderMesh2D(meshes["yellowBaloonHead"], shaders["VertexColor"], modelMatrix);
		modelMatrix = visMatrix * Transform2D::Translate(baloons[position].x, baloons[position].y - baloonPopStatus[position].fadeAway);
		modelMatrix *= Transform2D::Scale(0.25, 0.25);
		RenderMesh2D(meshes["yellowBaloonBase"], shaders["VertexColor"], modelMatrix);
		modelMatrix = visMatrix * Transform2D::Translate(baloons[position].x, baloons[position].y - baloonPopStatus[position].fadeAway);
		RenderMesh2D(meshes["yellowBaloonString"], shaders["VertexColor"], modelMatrix);
	}
	
	if (baloonPopStatus[position].distructionOffset > 0)
	{
		baloonPopStatus[position].distructionOffset -= baloons[position].speed * deltaTimeSeconds;
	}
	else
	{
		baloonPopStatus[position].distructionOffset = 0;
	}
	baloonPopStatus[position].fadeAway += baloons[position].speed * deltaTimeSeconds;
	if (baloons[position].y - baloonPopStatus[position].fadeAway < 0)
	{
		baloonPopStatus[position].done = true;
	}
}

void BowAndArrow::PopShuriken(int position, float deltaTimeSeconds)
{
	if (shurikens[position].red)
	{
		modelMatrix = visMatrix * Transform2D::Translate(
			shurikens[position].x + shurikenPopStatus[position].distructionOffset,
			shurikens[position].y + shurikenPopStatus[position].distructionOffset);
		modelMatrix *= Transform2D::Rotate(90 * M_PI / 180);
		modelMatrix *= Transform2D::Scale(shurikenPopStatus[position].fadeAway, shurikenPopStatus[position].fadeAway);
		RenderMesh2D(meshes["killBlade"], shaders["VertexColor"], modelMatrix);

		modelMatrix = visMatrix * Transform2D::Translate(
			shurikens[position].x - shurikenPopStatus[position].distructionOffset,
			shurikens[position].y + shurikenPopStatus[position].distructionOffset);
		modelMatrix *= Transform2D::Rotate(115 * M_PI / 180);
		modelMatrix *= Transform2D::Scale(shurikenPopStatus[position].fadeAway, shurikenPopStatus[position].fadeAway);
		RenderMesh2D(meshes["killBlade"], shaders["VertexColor"], modelMatrix);

		modelMatrix = visMatrix * Transform2D::Translate(
			shurikens[position].x - shurikenPopStatus[position].distructionOffset,
			shurikens[position].y - shurikenPopStatus[position].distructionOffset);
		modelMatrix *= Transform2D::Rotate(205 * M_PI / 180);
		modelMatrix *= Transform2D::Scale(shurikenPopStatus[position].fadeAway, shurikenPopStatus[position].fadeAway);
		RenderMesh2D(meshes["killBlade"], shaders["VertexColor"], modelMatrix);

		modelMatrix = visMatrix * Transform2D::Translate(
			shurikens[position].x + shurikenPopStatus[position].distructionOffset,
			shurikens[position].y - shurikenPopStatus[position].distructionOffset);
		modelMatrix *= Transform2D::Rotate(315 * M_PI / 180);
		modelMatrix *= Transform2D::Scale(shurikenPopStatus[position].fadeAway, shurikenPopStatus[position].fadeAway);
		RenderMesh2D(meshes["killBlade"], shaders["VertexColor"], modelMatrix);
	}
	else
	{
		modelMatrix = visMatrix * Transform2D::Translate(
			shurikens[position].x + shurikenPopStatus[position].distructionOffset,
			shurikens[position].y + shurikenPopStatus[position].distructionOffset);
		modelMatrix *= Transform2D::Rotate(90 * M_PI / 180);
		modelMatrix *= Transform2D::Scale(shurikenPopStatus[position].fadeAway, shurikenPopStatus[position].fadeAway);
		RenderMesh2D(meshes["blade"], shaders["VertexColor"], modelMatrix);

		modelMatrix = visMatrix * Transform2D::Translate(
			shurikens[position].x - shurikenPopStatus[position].distructionOffset,
			shurikens[position].y + shurikenPopStatus[position].distructionOffset);
		modelMatrix *= Transform2D::Rotate(115 * M_PI / 180);
		modelMatrix *= Transform2D::Scale(shurikenPopStatus[position].fadeAway, shurikenPopStatus[position].fadeAway);
		RenderMesh2D(meshes["blade"], shaders["VertexColor"], modelMatrix);

		modelMatrix = visMatrix * Transform2D::Translate(
			shurikens[position].x - shurikenPopStatus[position].distructionOffset,
			shurikens[position].y - shurikenPopStatus[position].distructionOffset);
		modelMatrix *= Transform2D::Rotate(205 * M_PI / 180);
		modelMatrix *= Transform2D::Scale(shurikenPopStatus[position].fadeAway, shurikenPopStatus[position].fadeAway);
		RenderMesh2D(meshes["blade"], shaders["VertexColor"], modelMatrix);

		modelMatrix = visMatrix * Transform2D::Translate(
			shurikens[position].x + shurikenPopStatus[position].distructionOffset,
			shurikens[position].y - shurikenPopStatus[position].distructionOffset);
		modelMatrix *= Transform2D::Rotate(315 * M_PI / 180);
		modelMatrix *= Transform2D::Scale(shurikenPopStatus[position].fadeAway, shurikenPopStatus[position].fadeAway);
		RenderMesh2D(meshes["blade"], shaders["VertexColor"], modelMatrix);
	}

	shurikenPopStatus[position].distructionOffset += shurikens[position].speed * deltaTimeSeconds;
	if (shurikenPopStatus[position].fadeAway > 0)
	{
		shurikenPopStatus[position].fadeAway -= shurikens[position].speed / 2 * deltaTimeSeconds;
	}
	else
	{
		shurikenPopStatus[position].fadeAway = 0;
		shurikenPopStatus[position].done = true;
	}
}

void BowAndArrow::RenderArrow(glm::vec2 position, float angle)
{
	modelMatrix = visMatrix * Transform2D::Translate(position[0], position[1]);
	modelMatrix *= Transform2D::Scale(0.5, 0.5);
	modelMatrix *= Transform2D::Rotate(angle);
	RenderMesh2D(meshes["arrowLine"], shaders["VertexColor"], modelMatrix);
	modelMatrix = visMatrix * Transform2D::Translate(position[0], position[1]);
	modelMatrix *= Transform2D::Rotate(angle);
	modelMatrix *= Transform2D::Translate(0.5, -0.05);
	modelMatrix *= Transform2D::Scale(0.25, 0.25);
	RenderMesh2D(meshes["arrowTip"], shaders["VertexColor"], modelMatrix);
}

void BowAndArrow::FrameStart()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void BowAndArrow::Update(float deltaTimeSeconds)
{
	if (go)
	{
		glm::ivec2 resolution = window->GetResolution();

		viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
		SetViewportArea(viewSpace, glm::vec3(0), true);

		visMatrix = glm::mat3(1);
		visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

		if (bowY < globalMouseY)
		{
			mouseAngle = atan2(globalMouseY - bowY, globalMouseX - bowX);
			bowAngle = mouseAngle;
		}
		else
		{
			mouseAngle = atan2(bowY - globalMouseY, globalMouseX - bowX);
			bowAngle = 2 * M_PI - mouseAngle;
		}

		modelMatrix = visMatrix * Transform2D::Translate(bowX, bowY);
		modelMatrix *= Transform2D::Rotate(bowAngle);
		modelMatrix *= Transform2D::Scale(0.5, 0.5);
		modelMatrix *= Transform2D::Scale(0.5, 1); // squish
		RenderMesh2D(meshes["bow"], shaders["VertexColor"], modelMatrix);

		if (!fired)
		{
			modelMatrix = visMatrix * Transform2D::Translate(bowX, logicSpace.height - 0.25);
			modelMatrix *= Transform2D::Scale(arrowSpeed - 1, 1);
			RenderMesh2D(meshes["speedBar"], shaders["VertexColor"], modelMatrix);

			arrowAngle = bowAngle;
			arrowX = bowX;
			arrowY = bowY;
			arrowTipX = bowX + 0.5 * cos(arrowAngle);
			arrowTipY = bowY + 0.5 * sin(arrowAngle);
		}
		else
		{
			arrowX += arrowSpeed * deltaTimeSeconds;
			arrowY = verticalOffset + arrowX * tan(arrowAngle);
			arrowTipX = arrowX + 0.5 * cos(arrowAngle);
			arrowTipY = arrowY + 0.5 * sin(arrowAngle);

			if ((arrowX > logicSpace.width) || (arrowY > logicSpace.height) || (arrowY < 0))
			{
				fired = false;
				arrowSpeed = 1;
			}
		}
		RenderArrow(glm::vec2(arrowX, arrowY), arrowAngle);

		shurikenAngle += deltaTimeSeconds;
		for (int i = 0; i < shurikens.size(); i++)
		{
			if (!shurikens[i].popped)
			{
				float shurikenCenterX = shurikens[i].x + 0.25;
				float shurikenCenterY = shurikens[i].y + 0.25;

				modelMatrix = visMatrix * Transform2D::Translate(shurikens[i].x, shurikens[i].y);
				modelMatrix *= Transform2D::Rotate(shurikenAngle * shurikens[i].speed);
				modelMatrix *= Transform2D::Scale(0.25, 0.25);
				if (!shurikens[i].red)
				{
					RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrix);
				}
				else
				{
					RenderMesh2D(meshes["instaKill"], shaders["VertexColor"], modelMatrix);
				}

				float widthY = cos(bowAngle) > 0.5 ? cos(bowAngle) : 0.5;
				float widthX = abs(sin(bowAngle)) > 0.25 ? abs(sin(bowAngle)) : 0.25;
				if ((shurikenCenterX <= bowX + widthX)
					&& (shurikenCenterY <= bowY + widthY) && (shurikenCenterY >= bowY - widthY))
				{
					if (!shurikens[i].red)
					{
						lives--;
					}
					else
					{
						lives = 0;
					}
					printf(" YOU'VE BEEN HIT!\n SCORE: %0.1f\n LIVES LEFT: %d\n\n", score, lives);
					if (lives == 0)
					{
						printf("\n================================\n\n YOU LOST!\n");
						if (highscores[mode] < score)
						{
							highscores[mode] = score;
						}
						printf(" YOUR FINAL SCORE: %0.1f\n", score);
						switch (mode)
						{
						case 0:
							printf(" EASY MODE HIGHSCORE: %0.1f\n\n", highscores[mode]);
							break;
						case 1:
							printf(" NORMAL MODE HIGHSCORE: %0.1f\n\n", highscores[mode]);
							break;
						case 2:
							printf(" HARD MODE HIGHSCORE: %0.1f\n\n", highscores[mode]);
							break;
						default:
							break;
						}
						printf("\n================================\n\n");

						go = false;
						greet = false;
					}
					else
					{
						shurikens[i].x = logicSpace.width;
						if ((rand() % 10) >= badChance)
						{
							shurikens[i].red = false;
						}
						else
						{
							shurikens[i].red = true;
						}
					}
				}
				else if (fired // cannot poke shuriken close to bow with unfired arrow
					&& (arrowTipX >= shurikenCenterX - 0.25) && (arrowTipX <= shurikenCenterX + 0.25)
					&& (arrowTipY >= shurikenCenterY - 0.25) && (arrowTipY <= shurikenCenterY + 0.25))
				{
					shurikens[i].popped = true;
					if (!shurikens[i].red)
					{
						score += 0.5;
						printf(" SHURIKEN SHOT DOWN!\n SCORE: %0.1f\n LIVES LEFT: %d\n\n", score, lives);
					}
					else
					{
						score += 1.5;
						printf(" INSTANT KILL SHURIKEN SHOT DOWN!\n SCORE: %0.1f\n LIVES LEFT: %d\n\n", score, lives);
					}
				}
				else if (shurikens[i].x < 0)
				{
					shurikens[i].x = logicSpace.width;
					shurikens[i].speed += inc; // increase difficulty
				}

				shurikens[i].x -= shurikens[i].speed * deltaTimeSeconds;
			}
			else
			{
				if (!shurikenPopStatus[i].done)
				{
					PopShuriken(i, deltaTimeSeconds);
				}
				else
				{
					shurikens[i].popped = false;
					shurikens[i].x = logicSpace.width;
					shurikens[i].speed += inc; // increase difficulty

					if ((rand() % 10) >= badChance)
					{
						baloons[i].red = false;
					}
					else
					{
						baloons[i].red = true;
					}

					shurikenPopStatus[i].distructionOffset = 0;
					shurikenPopStatus[i].fadeAway = 0.25;
					shurikenPopStatus[i].done = false;
				}
			}
		}

		for (int i = 0; i < baloons.size(); i++)
		{
			if (!baloons[i].popped)
			{
				RenderBaloon(glm::vec2(baloons[i].x, baloons[i].y), baloons[i].red);

				if ((arrowTipX >= baloons[i].x - 0.25) && (arrowTipX <= baloons[i].x + 0.25)
					&& (arrowTipY >= baloons[i].y - 0.5) && (arrowTipY <= baloons[i].y + 0.5))
				{
					baloons[i].popped = true;
					if (baloons[i].red)
					{
						score++;
						printf(" RED BALOON!\n SCORE: %0.1f\n LIVES LEFT: %d\n\n", score, lives);
					}
					else
					{
						score -= 2;
						printf(" YELLOW BALOON!\n SCORE: %0.1f\n LIVES LEFT: %d\n\n", score, lives);
					}
				}

				baloons[i].y += baloons[i].speed * deltaTimeSeconds;
				if (baloons[i].y > logicSpace.height)
				{
					baloons[i].y = -0.5;

					if ((rand() % 10) >= badChance)
					{
						baloons[i].red = true;
					}
					else
					{
						baloons[i].red = false;
					}
				}
			}
			else
			{
				if (!baloonPopStatus[i].done)
				{
					PopBaloon(i, deltaTimeSeconds);
				}
				else
				{
					baloons[i].popped = false;
					baloons[i].y = -0.5;
					baloons[i].speed += inc; // increase difficulty

					if ((rand() % 10) >= badChance)
					{
						baloons[i].red = true;
					}
					else
					{
						baloons[i].red = false;
					}
					
					baloonPopStatus[i].distructionOffset = 0.25;
					baloonPopStatus[i].fadeAway = 0.4;
					baloonPopStatus[i].done = false;
				}
			}
		}
	}
	else
	{
		if (!greet)
		{
			printf("\n BOW AND ARROW TARGETS:\n  RED BALOON: +1P\n  YELLOW BALOON: -2P\n  BULE SHURIKEN: +0.5P\n  CYAN SHURIKEN: +1.5P\n\n");
			printf("\n PRESS KEY TO SELECT GAME MODE:\n  E - EASY\n  N - NORMAL\n  H - HARD\n\n");
			greet = true;
		}
	}
}

void BowAndArrow::FrameEnd()
{
}

void BowAndArrow::OnInputUpdate(float deltaTime, int mods)
{
	float speed = deltaTime * 2;

	if (window->KeyHold(GLFW_KEY_W))
	{
		bowY += speed;
		if (bowY > logicSpace.height)
		{
			bowY = 0;
		}
	}
	if (window->KeyHold(GLFW_KEY_S))
	{
		bowY -= speed;
		if (bowY < 0)
		{
			bowY = logicSpace.height;
		}
	}
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && arrowSpeed < 4)
	{
		arrowSpeed += deltaTime * 0.5;
	}
}

void BowAndArrow::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_E)
	{
		baloons.clear();
		baloonPopStatus.clear();
		shurikens.clear();
		shurikenPopStatus.clear();

		arrowSpeed = 1;

		inc = 0.1;
		maxSpeed = 150;
		badChance = 2;
		mode = 0;

		InitGameObjects();
	}
	if (key == GLFW_KEY_N)
	{
		baloons.clear();
		baloonPopStatus.clear();
		shurikens.clear();
		shurikenPopStatus.clear();

		arrowSpeed = 1;

		inc = 0.2;
		maxSpeed = 300;
		badChance = 3;
		mode = 1;

		InitGameObjects();
	}
	if (key == GLFW_KEY_H)
	{
		baloons.clear();
		baloonPopStatus.clear();
		shurikens.clear();
		shurikenPopStatus.clear();

		arrowSpeed = 1;

		inc = 0.5;
		maxSpeed = 500;
		badChance = 5;
		mode = 2;

		InitGameObjects();
	}
}

void BowAndArrow::OnKeyRelease(int key, int mods)
{
}

void BowAndArrow::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	globalMouseX = mouseX * scaleMouseX;
	globalMouseY = logicSpace.height - mouseY * scaleMouseY;
}

void BowAndArrow::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void BowAndArrow::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT))
	{
		fired = true;
		verticalOffset = arrowY;
	}
}

void BowAndArrow::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}