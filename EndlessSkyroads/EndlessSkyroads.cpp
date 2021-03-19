#include "EndlessSkyroads.h"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include <Core/Engine.h>

using namespace std;

EndlessSkyroads::EndlessSkyroads()
{
}

EndlessSkyroads::~EndlessSkyroads()
{
}

void EndlessSkyroads::Init()
{
	camera = new SkyroadsAux::Camera();
	platforms = new SkyroadsAux::Platforms();

	renderPlayer = true;
	playerStatus = NORMALDISPLAY;

	speedMin = 1;
	speedMax = 10;
	speed = 1;
	right = false;
	left = false;
	jump = false;
	switchPov = false;
	glide = false;
	jumpSpeed = scootSpeed;

	camera->Set(glm::vec3(0, POV3Y, POV3Z), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)); // 3rd pov default
	groundLevel = camera->position[1];

	platformScaleX = 2;
	platformScaleY = 0.25;

	onPlatform = true;
	gameEnd = false;
	stuckOnMaxSpeed = false;
	goGhost = false;

	maxSpeedStartTime = 0;
	maxSpeedStopTime = 0;
	goGhostStartTime = 0;
	goGhostStopTime = 0;
	powerUpAnimationStartTime = 0;
	powerUpAnimationStopTime = 0;

	falling = false;
	fallAcelerate = 0;

	lives = 3;
	fuel = maxFuel;
	whiteBarSize[0] = maxFuel / 10;
	fuelGain = 0.25;
	fuelLose = 0.25;
	horisont = -FIELDLENGHT;

	Mesh* box = new Mesh("box");
	box->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	meshes[box->GetMeshID()] = box;
	
	Mesh* sphere = new Mesh("sphere");
	sphere->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
	meshes[sphere->GetMeshID()] = sphere;

	Mesh* fuelBar = CreateFuelbar("fuelBar");
	meshes[fuelBar->GetMeshID()] = fuelBar;

	Mesh* life = CreateLife("life");
	meshes[life->GetMeshID()] = life;

	Shader* shader = new Shader("ShaderTema2");
	shader->AddShader("Source/EndlessSkyroads/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/EndlessSkyroads/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	// cleanup in case of R
	platforms->logList.clear();
	platforms->walls.clear();

	float lenFirstRow = -platforms->NewLength();
	platforms->logList.push_back(SkyroadsAux::PlarformLog(CENTER, camera->position[2], lenFirstRow, PURPLE));
	platforms->farthestZ = std::min(platforms->logList[platforms->logList.size() - 1].endZ, platforms->farthestZ);

	lenFirstRow = -platforms->NewLength();
	platforms->logList.push_back(SkyroadsAux::PlarformLog(RIGHTLANE, camera->position[2], lenFirstRow, BLUE));
	platforms->farthestZ = std::min(platforms->logList[platforms->logList.size() - 1].endZ, platforms->farthestZ);

	lenFirstRow = -platforms->NewLength();
	platforms->logList.push_back(SkyroadsAux::PlarformLog(LEFTLANE, camera->position[2], lenFirstRow, BLUE));
	platforms->farthestZ = std::min(platforms->logList[platforms->logList.size() - 1].endZ, platforms->farthestZ);

	platforms->farthestAuxZ = platforms->farthestZ;
}

Mesh* EndlessSkyroads::CreateFuelbar(std::string name)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat({ 0, 0, 0 }, { 0, 0, 0 }),
		VertexFormat({ 1, 0, 0 }, { 0, 0, 0 }),
		VertexFormat({ 1, 1, 0 }, { 0, 0, 0 }),
		VertexFormat({ 0, 1, 0 }, { 0, 0, 0 })
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2,
											3, 0, 2 };

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* EndlessSkyroads::CreateLife(std::string name)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat({ 0, 0, 0 }, { 0, 0, 0 }),
		VertexFormat({ 0.25, 0.5, 0 }, { 0, 0, 0 }),
		VertexFormat({ 0.5, 0.1, 0 }, { 0, 0, 0 }),
		VertexFormat({ 0.75, 0.5, 0 }, { 0, 0, 0 }),
		VertexFormat({ 1, 0, 0 }, { 0, 0, 0 }),
		VertexFormat({ 0.5, -0.75, 0 }, { 0, 0, 0 })
	};

	Mesh* heart = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2,
											2, 3, 4,
											0, 2, 4,
											0, 4, 5 };

	heart->InitFromData(vertices, indices);
	return heart;
}

void EndlessSkyroads::FrameStart()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void EndlessSkyroads::Update(float deltaTimeSeconds)
{
	if (!gameEnd)
	{
		AdvanceForward(deltaTimeSeconds);

		if (fuel <= 0)
		{
			fuel = 0;
			lives--;

			if (lives == 0)
			{
				gameEnd = true;
			}
			else
			{
				fuel = maxFuel;
			}
		}
		
		// check if special platform animations ended
		if (effectAnimation && (Engine::GetElapsedTime() >= powerUpAnimationStopTime))
		{
			effectAnimation = false;

			if (stuckOnMaxSpeed && goGhost && (Engine::GetElapsedTime() < maxSpeedStopTime) && (Engine::GetElapsedTime() < goGhostStopTime))
			{
				playerStatus = ORANGEGHOST;
			}
			else if (stuckOnMaxSpeed && (Engine::GetElapsedTime() < maxSpeedStopTime)) // orange still in effect
			{
				playerStatus = ORANGEEFFECT; // return to orange deformation
			}
			else if (goGhost && (Engine::GetElapsedTime() < goGhostStopTime)) // ghost still in effect
			{
				playerStatus = GHOSTEFFECT; // return to ghost deformation
			}
			else
			{
				playerStatus = NORMALDISPLAY;
			}
		}
		if (stuckOnMaxSpeed && (Engine::GetElapsedTime() >= maxSpeedStopTime))
		{
			stuckOnMaxSpeed = false;
			playerStatus = NORMALDISPLAY;
		}
		if (goGhost && (Engine::GetElapsedTime() >= goGhostStopTime))
		{
			goGhost = false;
			playerStatus = NORMALDISPLAY;
		}


		if (left || right)
		{
			Scoot(deltaTimeSeconds);
		}

		if (jump)
		{
			Jump(deltaTimeSeconds);
		}

		if (switchPov)
		{
			if (renderPlayer)
			{ // switch back to 3rd pov
				camera->TranslateUpword(POV1Y);
				camera->TranslateForward(-POV1Z);
			}
			else
			{ // switch to 1st pov
				camera->TranslateUpword(-POV1Y);
				camera->TranslateForward(POV1Z);
			}

			groundLevel = camera->position[1];
			switchPov = false;
		}
		if (renderPlayer) // 3rd pov
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(camera->GetTargetPosition());
			modelMatrix *= Transform3D::Scale(glm::vec3(0.5f));
			RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema2"], modelMatrix, { 0, 1, 1 }, playerStatus,
				camera->GetTargetPosition(), noNeed);
		}

		// fill distance to horisont
		while (platforms->farthestZ > horisont) // smallest number = farthest away
		{
			platforms->AddPlatforms(lives);
		}

		{ // render fuel bar
			RenderSimpleMesh(meshes["fuelBar"], shaders["ShaderTema2"], glm::mat4(1), { 1, 1, 1 }, MONITOR, whiteBarPos, whiteBarSize);

			fuelSize[0] = fuel / 10;
			float fuelLeft = fuel / maxFuel;
			RenderSimpleMesh(meshes["fuelBar"], shaders["ShaderTema2"], glm::mat4(1), { 1 - fuelLeft, fuelLeft, 0 }, MONITOR, fuelPos, fuelSize);
		}

		if (lives == 0)
		{
			gameEnd = true;
		}
		else
		{
			for (int i = 0; i < lives; i++)
			{
				if (i < 3)
				{
					RenderSimpleMesh(meshes["life"], shaders["ShaderTema2"], glm::mat4(1), { 1, 0, 0 }, MONITOR,
						{ livesPos[0] + float(i) / 10, livesPos[1], livesPos[2] }, livesSize);
				}
				else // player can gather one extra life
				{
					RenderSimpleMesh(meshes["life"], shaders["ShaderTema2"], glm::mat4(1), { 1, 0.5, 0.5 }, MONITOR,
						{ livesPos[0] + float(i - 1) / 10, livesPos[1] - 0.1, livesPos[2] }, livesSize);
				}
			}
		}

		onPlatform = false; // used to check if it fell off

		for (int i = 0; i < platforms->logList.size(); i++)
		{
			if (platforms->logList[i].endZ > camera->position[2]) // offscreen; don't render it anymore
			{
				platforms->logList.erase(platforms->logList.begin() + i);
				i--; // no platform is skipped
			}
			else // render platform
			{
				RenderPlatform(i);
			}
		}

		if (!onPlatform)
		{
			falling = true;
			camera->TranslateUpword(-deltaTimeSeconds * (speed + fallAcelerate));
			if (camera->position[1] < fallStop)
			{
				camera->position[1] = fallStop;
				gameEnd = true;
			}
		}

		for (int i = 0; i < platforms->walls.size(); i++)
		{
			if ( // offscreen
				( ( platforms->walls[i].startZ - WALLTHICKNESS ) > camera->position[2] )

				|| ( // or on same lane as player, behind player
					(
						( platforms->walls[i].x >= ( (camera->GetTargetPosition())[0] - LANEWIDTH / 2) )
						&& ( platforms->walls[i].x <= ( (camera->GetTargetPosition())[0] + LANEWIDTH / 2) )
					)

					&& (
						(
							renderPlayer
							&& ( platforms->walls[i].startZ - WALLTHICKNESS ) > ( (camera->GetTargetPosition())[2] + PLAYERHITRANGE )
						)
						|| (
							(!renderPlayer)
							&& ( ( platforms->walls[i].startZ - WALLTHICKNESS ) > ( camera->position[2] - POV1Y + PLAYERHITRANGE ) )
						   )
					   )
				   )
				)
			{ // don't render it anymore
				platforms->walls.erase(platforms->walls.begin() + i);
				i--; // no wall is skipped
			}
			else // render wall
			{
				RenderWall(deltaTimeSeconds,i);
			}
		}
	}
}

void EndlessSkyroads::FrameEnd()
{
}

void EndlessSkyroads::Scoot(float deltaTimeSeconds)
{
	if (right && (camera->position[0] < scootEndX))
	{ // must go right
		camera->TranslateRight(deltaTimeSeconds * scootSpeed);
	}
	else if (left && (camera->position[0] > scootEndX))
	{ // must go left
		camera->TranslateRight(-deltaTimeSeconds * scootSpeed);
	}
	else
	{
		camera->position[0] = scootEndX;
		right = false;
		left = false;
	}
}

void EndlessSkyroads::Jump(float deltaTimeSeconds)
{
	if (rise)
	{ // first half of jump: rise
		camera->TranslateUpword(deltaTimeSeconds * jumpSpeed);
		if (camera->position[1] >= (groundLevel + jumpHight))
		{
			camera->position[1] = groundLevel + jumpHight;
			rise = false;

			if (camera->position[2] > startScootZ - (jumpLength / 2))
			{ // max height was reached before middle oj jump: glide a bit before descending
				glide = true;
				glideLength = camera->position[2] - startScootZ + (jumpLength / 2);
			}
		}
	}
	else if (glide)
	{
		if (camera->position[2] <= startScootZ - (jumpLength / 2) - glideLength)
		{
			glide = false;
		}
	}
	else if (camera->position[1] > groundLevel)
	{ // second half of jump: descend
		camera->TranslateUpword(-deltaTimeSeconds * jumpSpeed);
		if (camera->position[1] < groundLevel)
		{
			camera->position[1] = groundLevel;
			jump = false;
		}
	}
	else
	{ // land
		camera->position[1] = groundLevel;
		jump = false;
	}
}

void EndlessSkyroads::AdvanceForward(float deltaTimeSeconds)
{
	if (falling) // for more realistc falling animation: move slower forward, quicker downward
	{
		fallAcelerate += (deltaTimeSeconds * 2);
		if ((speed - fallAcelerate) < (speedMin / 2))
		{
			fallAcelerate = speed - (speedMin / 2);
		}
	}
	else // increase speed in time and wall chance
	{
		speedMin += (deltaTimeSeconds / 100);
		speedMax += (deltaTimeSeconds / 100);
		if (speed < speedMin)
		{
			speed = speedMin;
		}
		fuel -= deltaTimeSeconds * (speed / 100); // fuel is not consumed when falling

		if (platforms->wallLimit != 100)
		{
			platforms->wallLimit += deltaTimeSeconds;
		}
		if (platforms->wallLimit > 100)
		{
			platforms->wallLimit = 100;
		}

		if (platforms->springLimit != 100)
		{
			platforms->springLimit += (deltaTimeSeconds / 2);
		}
		if (platforms->springLimit > 100)
		{
			platforms->springLimit = 100;
		}
	}

	camera->MoveForward(deltaTimeSeconds * (speed - fallAcelerate));
	horisont -= deltaTimeSeconds * speed; // camera and player move towards -inf
}

bool EndlessSkyroads::IsOnPlatform(int i) // not airborn, on platform (maintaining course on crossing during scoot)
{
	return (
			( (!jump) && (groundLevel == camera->position[1]) ) // not airborn

			&& ( // on the platform lane
				( platforms->logList[i].x >= ( (camera->GetTargetPosition())[0] - LANEWIDTH / 2 ) )
				&& ( platforms->logList[i].x <= ( (camera->GetTargetPosition())[0] + LANEWIDTH / 2 ) )
				)

			&& ( // between platform sartZ and endZ
				(
					renderPlayer // player sphere is between platform bounds
					&& ( // startZ >= back edge of sphere;  endZ >= front edge of sphere
						( platforms->logList[i].startZ >= ( (camera->GetTargetPosition())[2] - PLAYERHITRANGE ) )
						&& ( platforms->logList[i].endZ <= ( (camera->GetTargetPosition())[2] + PLAYERHITRANGE ) )
						)
				)

				|| ( // screen edge between platform sartZ and endZ
					(!renderPlayer)
					&& ( // camera was set at POV3Z, translated with POV1Z
						( platforms->logList[i].startZ >= (camera->position[2] - POV1Y + PLAYERHITRANGE) )
						&& ( platforms->logList[i].endZ <= (camera->position[2] - POV1Y + PLAYERHITRANGE) )
						)
					)
				)
			);
}

bool EndlessSkyroads::HasHitPlatform(int i)
{
	return false;
}

bool EndlessSkyroads::HasHitWall(int i) // in wall (maintaining course or crossing during scoot)
{
	return (
			( // on same lane
				( platforms->walls[i].x >= ( (camera->GetTargetPosition())[0] - LANEWIDTH / 2 ) )
				&& ( platforms->walls[i].x <= ( (camera->GetTargetPosition())[0] + LANEWIDTH / 2 ) )
			)

			&& ( // intersects with player on z axis
				(
					renderPlayer
					&& (
						( // front side in wall
							( platforms->walls[i].startZ >= ( (camera->GetTargetPosition())[2] - PLAYERHITRANGE ) )
							&& ( platforms->walls[i].startZ <= ((camera->GetTargetPosition())[2] + PLAYERHITRANGE ) )
						)
						|| ( // backside in wall
							( ( platforms->walls[i].startZ - WALLTHICKNESS ) >= ( (camera->GetTargetPosition())[2] - PLAYERHITRANGE ) )
							&& ( ( platforms->walls[i].startZ - WALLTHICKNESS ) <= ( (camera->GetTargetPosition())[2] + PLAYERHITRANGE ) )
							)
						|| ( // wall inside sphere
							( platforms->walls[i].startZ <= ( (camera->GetTargetPosition())[2] + PLAYERHITRANGE ) )
							&& ( ( platforms->walls[i].startZ - WALLTHICKNESS ) >= ( (camera->GetTargetPosition())[2] - PLAYERHITRANGE ) )
							)
					   )
				)

				|| (
					(!renderPlayer)
					&& (
						( platforms->walls[i].startZ >= (camera->position[2] - POV1Y + PLAYERHITRANGE) )
						&& ( ( platforms->walls[i].startZ - WALLTHICKNESS ) <= (camera->position[2] - POV1Y + PLAYERHITRANGE) )
						)
					)
			   )

			&& ( // intersects with player on y axis
				(
					renderPlayer
					&& ( ( platforms->walls[i].hight - PLATFORMTHICKNESS ) >= ( (camera->GetTargetPosition())[1] - PLAYERHITRANGE ) )
				)

				|| (
					(!renderPlayer)
					&& ( ( platforms->walls[i].hight - PLATFORMTHICKNESS ) >= (camera->position[1] - POV1Y + PLAYERHITRANGE) )
					)
			   )
		  );
}

void EndlessSkyroads::AnimationSetup() // for powerups that take effect instantenuously
{
	effectAnimation = true;
	powerUpAnimationStartTime = Engine::GetElapsedTime();
	powerUpAnimationStopTime = powerUpAnimationStartTime + powerUpAnimationTime;
}

void EndlessSkyroads::RenderPlatform(int i)
{
	// Translate needs coordonates for the center of the object
	float length = platforms->logList[i].endZ - platforms->logList[i].startZ;
	float offset = platforms->logList[i].startZ - abs(length / 2);

	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate({ platforms->logList[i].x, PLATFORMTBOTTOM, offset });
	modelMatrix *= Transform3D::Scale({ LANEWIDTH, PLATFORMTHICKNESS, abs(length) });

	switch (platforms->logList[i].colorCode)
	{
	case PURPLE:
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, { 1, 0, 1 }, NORMALDISPLAY, noNeed, noNeed);
		break;

	case BLUE:
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, { 0, 0, 1 }, NORMALDISPLAY, noNeed, noNeed);
		break;

	case GREEN:
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, { 0, 1, 0 }, NORMALDISPLAY, noNeed, noNeed);
		break;

	case ORANGE:
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, { 1, 0.5, 0 }, NORMALDISPLAY, noNeed, noNeed);
		break;

	case YELLOW:
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, { 1, 1, 0 }, NORMALDISPLAY, noNeed, noNeed);
		break;

	case RED:
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, { 1, 0, 0 }, NORMALDISPLAY, noNeed, noNeed);
		break;

	case MINUSLIFE:
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, { 1, 0.5, 0.5 }, NORMALDISPLAY, noNeed, noNeed);
		break;

	case PLUSLIFE:
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, { 0.5, 0, 0 }, NORMALDISPLAY, noNeed, noNeed);
		break;

	case GHOST:
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, { 0, 0.5, 0.5 }, NORMALDISPLAY, noNeed, noNeed);
		break;

	default:
		break;
	}

	if (jump && (!falling)) // airborn
	{
		onPlatform = true;
	}
	else if (IsOnPlatform(i))
	{
		onPlatform = true;
		if (platforms->logList[i].colorCode != PURPLE)
		{
			switch (platforms->logList[i].colorCode)
			{
			case GREEN:
				fuel += fuelGain;
				if (fuel > maxFuel)
				{
					fuel = maxFuel;
				}
				AnimationSetup();
				playerStatus = GREENEFFECT;
				break;

			case YELLOW:
				fuel -= fuelLose;
				if (fuel < 0)
				{
					fuel = 0;
					lives--;

					if (lives == 0)
					{
						gameEnd = true;
					}
					else
					{
						fuel = maxFuel;
					}
				}
				AnimationSetup();
				playerStatus = YELLOWEFFECT;
				break;

			case ORANGE:
				stuckOnMaxSpeed = true;
				speed = speedMax;
				maxSpeedStartTime = Engine::GetElapsedTime();
				maxSpeedStopTime = maxSpeedStartTime + orangeActionTime;

				if (goGhost) // ghost also active
				{
					playerStatus = ORANGEGHOST;
				}
				else
				{
					playerStatus = ORANGEEFFECT;
				}
				break;

			case RED:
				gameEnd = true;
				break;

			case MINUSLIFE:
				lives--;
				if (lives == 0)
				{
					gameEnd = true;
				}
				AnimationSetup();
				playerStatus = MINUSEFFECT;
				break;

			case PLUSLIFE:
				if (lives < 4)
				{
					lives++;
				}
				AnimationSetup();
				playerStatus = PLUSEFFECT;
				break;

			case GHOST:
				goGhost = true;
				goGhostStartTime = Engine::GetElapsedTime();
				goGhostStopTime = goGhostStartTime + ghostActionTime;

				if (stuckOnMaxSpeed) // orange also active
				{
					playerStatus = ORANGEGHOST;
				}
				else
				{
					playerStatus = GHOSTEFFECT;
				}
				break;

			default:
				break;
			}
			platforms->logList[i].colorCode = PURPLE;
		}
	}
}

void EndlessSkyroads::RenderWall(float deltaTimeSeconds, int i)
{
	// walls rise from within platform
	// find center of wall
	float offset = PLATFORMTBOTTOM + platforms->walls[i].hight / 2;

	// width wall < width lane so no ugly glitching at the bottom of the wall where it intersects with the platform
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate({ platforms->walls[i].x, offset, platforms->walls[i].startZ });
	modelMatrix *= Transform3D::Scale({ LANEWIDTH - 0.05, platforms->walls[i].hight, WALLTHICKNESS });
	RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, { 1, 1, 1 }, 1, noNeed, noNeed);

	if ((((camera->GetTargetPosition())[2] - SPRINGDISTANCE * speed) <= platforms->walls[i].startZ)
		&& platforms->walls[i].spring
		&& (platforms->walls[i].hight != platforms->walls[i].finalHight))
		// player aproaches spring wall
	{
		if (platforms->walls[i].hight != platforms->walls[i].finalHight)
		{
			platforms->walls[i].hight += deltaTimeSeconds * WALLRISESPEED;
		}
		if (platforms->walls[i].hight > platforms->walls[i].finalHight)
		{
			platforms->walls[i].hight = platforms->walls[i].finalHight;
		}
	}

	if ((!goGhost) && HasHitWall(i))
	{
		lives--;
		if (lives == 0)
		{
			gameEnd = true;
		}
		else
		{
			AnimationSetup();
			playerStatus = HITEFFECT;

			// delete hit wall
			platforms->walls.erase(platforms->walls.begin() + i);
		}
	}
}

void EndlessSkyroads::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, const int objectCode,
	const glm::vec3& objectPos, const glm::vec3& objectSize)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	GLint object_color_location = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(object_color_location, 1, glm::value_ptr(color));

	GLint object_position_location = glGetUniformLocation(shader->program, "object_pos");
	glUniform3fv(object_position_location, 1, glm::value_ptr(objectPos));

	GLint object_size_location = glGetUniformLocation(shader->program, "object_size");
	glUniform3fv(object_size_location, 1, glm::value_ptr(objectSize));

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	int loc_object_code = glGetUniformLocation(shader->program, "object_code");
	glUniform1i(loc_object_code, objectCode);

	int loc_time = glGetUniformLocation(shader->program, "ElapsedTime");
	float elapsedTime = Engine::GetElapsedTime();
	glUniform1f(loc_time, elapsedTime);

	int loc_speed = glGetUniformLocation(shader->program, "speed");
	glUniform1f(loc_speed, speed);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void EndlessSkyroads::OnInputUpdate(float deltaTime, int mods)
{
	if ((!stuckOnMaxSpeed) && (!jump)) // no modifying the speed during jump
	{
		if (window->KeyHold(GLFW_KEY_W)) {
			speed += deltaTime * WSACCELERATION;
			if (speed > speedMax)
			{
				speed = speedMax;
			}
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			speed -= deltaTime * WSACCELERATION;
			if (speed < speedMin)
			{
				speed = speedMin;
			}
		}
	}
}

void EndlessSkyroads::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_R)
	{
		gameEnd = false;
		Init();
	}
	if (key == GLFW_KEY_C)
	{
		if (!jump)
		{
			renderPlayer = !renderPlayer;
			switchPov = true;
		}
	}
	if (key == GLFW_KEY_A)
	{
		if ((!falling) && (!right) && (!left)) // no scoot is in progress
		{
			left = true;
			scootEndX = camera->position[0] - LANEWIDTH; // enough to go one lane over
		}
		else if (scootEndX > (-LANEWIDTH * 4))
		{
			// A was pressed while player was still scooting
			// can go up to max 4 lanes to the left

			scootEndX -= LANEWIDTH; // longer scoot

			if (camera->position[0] < scootEndX)
			{ // must move to the right
				right = true;
				left = false;
			}
			else
			{ // must move to the left
				left = true;
				right = false;
			}
		}
	}
	if (key == GLFW_KEY_D)
	{
		if ((!falling) && (!right) && (!left)) // no scoot is in progress
		{
			right = true;
			scootEndX = camera->position[0] + LANEWIDTH; // enough to go one lane over
		}
		else if (scootEndX < (LANEWIDTH * 4))
		{
			// D was pressed while player was still scooting
			// can go up to max 4 lanes to the right

			scootEndX += LANEWIDTH; // longer scoot

			if (camera->position[0] < scootEndX)
			{ // must move to the right
				right = true;
				left = false;
			}
			else
			{ // must move to the left
				left = true;
				right = false;
			}
		}
	}
	if (key == GLFW_KEY_SPACE)
	{
		if ((!jump) && (!falling))
		{
			jump = true;
			rise = true;
			glide = false;
			startScootZ = camera->position[2];
			float deltaT = (jumpLength / 2) / speed;
			jumpSpeed = sqrt(jumpHight * jumpHight + (jumpLength * jumpLength) / 2) / deltaT;
		}
		// no double jump allowed
	}
}

void EndlessSkyroads::OnKeyRelease(int key, int mods)
{
}

void EndlessSkyroads::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void EndlessSkyroads::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void EndlessSkyroads::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void EndlessSkyroads::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void EndlessSkyroads::OnWindowResize(int width, int height)
{
}