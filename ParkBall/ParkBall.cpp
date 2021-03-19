#include "ParkBall.h"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include <Core/Engine.h>

using namespace std;

ParkBall::ParkBall()
{
}

ParkBall::~ParkBall()
{
}

void ParkBall::InitVars()
{
	camera = new ParkBallAux::Camera();
	platforms = new ParkBallAux::Platforms();

	renderPlayer = true;
	playerStatus = PLAYER_ROLL;
	playerAngle = 0;
	leafAngle = 0;
	score = 0;
	showed = false;

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
	horisont = -FIELD_LENGHT;

	// cleanup in case of R
	platforms->logList.clear();
	platforms->walls.clear();
	wallTextures.clear();
	lights.clear();
}

void ParkBall::Init()
{
	InitVars();

	const string textureLoc = "Source/ParkBall/Textures/";

	// load textures
	{
		Texture2D* texturePlayer = new Texture2D();
		texturePlayer->Load2D((textureLoc + "soccer-ball.jpg").c_str(), GL_REPEAT);
		mapTextures["ball"] = texturePlayer;

		Texture2D* textureBlue = new Texture2D();
		textureBlue->Load2D((textureLoc + "road-bricks.jpg").c_str(), GL_REPEAT);
		mapTextures["road-blue"] = textureBlue;

		Texture2D* texturePurple = new Texture2D();
		texturePurple->Load2D((textureLoc + "cracked.jpg").c_str(), GL_REPEAT);
		mapTextures["road-purple"] = texturePurple;

		Texture2D* textureGreen = new Texture2D();
		textureGreen->Load2D((textureLoc + "grass-bricks.jpg").c_str(), GL_REPEAT);
		mapTextures["road-green"] = textureGreen;

		Texture2D* textureOrange = new Texture2D();
		textureOrange->Load2D((textureLoc + "road-striped-edges.png").c_str(), GL_REPEAT);
		mapTextures["road-orange"] = textureOrange;

		Texture2D* textureYellow = new Texture2D();
		textureYellow->Load2D((textureLoc + "pebbels.jpg").c_str(), GL_REPEAT);
		mapTextures["road-yellow"] = textureYellow;

		Texture2D* textureRed = new Texture2D();
		textureRed->Load2D((textureLoc + "dark-cracked.jpg").c_str(), GL_REPEAT);
		mapTextures["road-red"] = textureRed;

		Texture2D* textureMinusLife = new Texture2D();
		textureMinusLife->Load2D((textureLoc + "dirt.jpg").c_str(), GL_REPEAT);
		mapTextures["road-minuslife"] = textureMinusLife;

		Texture2D* texturePlusLife = new Texture2D();
		texturePlusLife->Load2D((textureLoc + "grass.jpg").c_str(), GL_REPEAT);
		mapTextures["road-pluslife"] = texturePlusLife;

		Texture2D* textureGhost = new Texture2D();
		textureGhost->Load2D((textureLoc + "biscuits.jpg").c_str(), GL_REPEAT);
		mapTextures["road-ghost"] = textureGhost;

		Texture2D* textureWall1 = new Texture2D();
		textureWall1->Load2D((textureLoc + "bricks1.jpg").c_str(), GL_REPEAT);
		mapTextures["wall1"] = textureWall1;
		wallTextures.push_back("wall1");

		Texture2D* textureWall2 = new Texture2D();
		textureWall2->Load2D((textureLoc + "bricks2.jpg").c_str(), GL_REPEAT);
		mapTextures["wall2"] = textureWall2;
		wallTextures.push_back("wall2");

		Texture2D* textureWall3 = new Texture2D();
		textureWall3->Load2D((textureLoc + "bricks3.png").c_str(), GL_REPEAT);
		mapTextures["wall3"] = textureWall3;
		wallTextures.push_back("wall3");

		Texture2D* textureWall4 = new Texture2D();
		textureWall4->Load2D((textureLoc + "bricks4.jpg").c_str(), GL_REPEAT);
		mapTextures["wall4"] = textureWall4;
		wallTextures.push_back("wall4");

		Texture2D* textureWall5 = new Texture2D();
		textureWall5->Load2D((textureLoc + "bricks5.png").c_str(), GL_REPEAT);
		mapTextures["wall5"] = textureWall5;
		wallTextures.push_back("wall5");

		Texture2D* textureWall6 = new Texture2D();
		textureWall6->Load2D((textureLoc + "bricks6.png").c_str(), GL_REPEAT);
		mapTextures["wall6"] = textureWall6;
		wallTextures.push_back("wall6");

		Texture2D* sky = new Texture2D();;
		sky->Load2D((textureLoc + "sky.jpg").c_str(), GL_REPEAT);
		mapTextures["sky"] = sky;

		Texture2D* ground = new Texture2D();
		ground->Load2D((textureLoc + "park.jpg").c_str(), GL_REPEAT);
		mapTextures["ground"] = ground;

		Texture2D* lamp = new Texture2D();
		lamp->Load2D((textureLoc + "wood1.jpg").c_str(), GL_REPEAT);
		mapTextures["lamp"] = lamp;

		Texture2D* pavement = new Texture2D();
		pavement->Load2D((textureLoc + "grainy-pavement.jpg").c_str(), GL_REPEAT);
		mapTextures["pavement"] = pavement;

		Texture2D* metal = new Texture2D();
		metal->Load2D((textureLoc + "metal2.jpg").c_str(), GL_REPEAT);
		mapTextures["metal"] = metal;

		Texture2D* leaf = new Texture2D();
		leaf->Load2D((textureLoc + "leaf.jpg").c_str(), GL_REPEAT);
		mapTextures["leaf"] = leaf;
	}

	// load meshes
	{
		Mesh* box = new Mesh("box");
		box->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[box->GetMeshID()] = box;

		Mesh* sphere = new Mesh("sphere");
		sphere->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[sphere->GetMeshID()] = sphere;

		Mesh* fuelBar = ParkBallAux::CreateFuelbar("fuelBar");
		meshes[fuelBar->GetMeshID()] = fuelBar;

		Mesh* life = ParkBallAux::CreateLife("life");
		meshes[life->GetMeshID()] = life;

		Mesh* leaf = ParkBallAux::CreateLeaf("leaf");
		meshes[leaf->GetMeshID()] = leaf;

		Mesh* cone = ParkBallAux::CreateCone("cone");
		meshes[cone->GetMeshID()] = cone;

		Mesh* roundPole = ParkBallAux::CreateRoundPole("roundPole", 0.25, 0.25, 8.25);
		meshes[roundPole->GetMeshID()] = roundPole;

		Mesh* poleArm = ParkBallAux::CreateRoundPole("poleArm", 0.25, 0.25, 2.5);
		meshes[poleArm->GetMeshID()] = poleArm;

		Mesh* poleStump = ParkBallAux::CreateSquarePole("poleStump", 0.25, 0.5, 1);
		meshes[poleStump->GetMeshID()] = poleStump;

		Mesh* roundBase = ParkBallAux::CreateRoundPole("roundBase", 2, 0, 2);
		meshes[roundBase->GetMeshID()] = roundBase;

		Mesh* squareBase = ParkBallAux::CreateSquarePole("squareBase", 2, 0, 2);
		meshes[squareBase->GetMeshID()] = squareBase;

		Mesh* lampBase = ParkBallAux::CreateSquarePole("lampBase", 2, 1, 1.5);
		meshes[lampBase->GetMeshID()] = lampBase;

		Mesh* lampBase2 = ParkBallAux::CreateSquarePole("lampBase2", 2, 1, 1);
		meshes[lampBase2->GetMeshID()] = lampBase2;
	}

	Shader* shader = new Shader("ShaderTema3");
	shader->AddShader("Source/ParkBall/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/ParkBall/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	// initial lane of platforms
	{
		float lenFirstRow = -platforms->NewLength();
		platforms->logList.push_back(ParkBallAux::PlarformLog(CENTER, camera->position[2], lenFirstRow, TOUCHED, true));
		platforms->farthestZ = std::min(platforms->logList[platforms->logList.size() - 1].endZ, platforms->farthestZ);

		lenFirstRow = -platforms->NewLength();
		platforms->logList.push_back(ParkBallAux::PlarformLog(RIGHTLANE, camera->position[2], lenFirstRow, NORMAL, true));
		platforms->farthestZ = std::min(platforms->logList[platforms->logList.size() - 1].endZ, platforms->farthestZ);

		lenFirstRow = -platforms->NewLength();
		platforms->logList.push_back(ParkBallAux::PlarformLog(LEFTLANE, camera->position[2], lenFirstRow, NORMAL, true));
		platforms->farthestZ = std::min(platforms->logList[platforms->logList.size() - 1].endZ, platforms->farthestZ);

		platforms->farthestAuxZ = platforms->farthestZ;
	}

	// lightsources
	{
		int swichLane = 1;
		glm::vec3 rgb = glm::vec3(1);

		for (int i = 0; i < FIELD_LENGHT; i += distBetweenLights)
		{
			// non spot
			lights.push_back(ParkBallAux::LightLog({ swichLane * distFromCenterLane, hightNonSpot, -i },
												glm::vec3(0), rgb, 0, 0));
			lightPositions.push_back({ swichLane * distFromCenterLane, hightNonSpot, -i });
			lightDirections.push_back(glm::vec3(0));
			lightColors.push_back(rgb);
			cutOffAngles.push_back(0);
			lightIsSpot.push_back(0);

			LightRGB(rgb);

			swichLane *= -1;

			// spot
			lights.push_back(ParkBallAux::LightLog({ swichLane * distFromCenterLane, hightSpot, -i },
												{ swichLane * spotDirection.x, swichLane * spotDirection.y, spotDirection.z },
												rgb, cutoffAng, 1));
			lightPositions.push_back({ swichLane * distFromCenterLane, hightSpot, -i });
			lightDirections.push_back({ swichLane * spotDirection.x, swichLane * spotDirection.y, spotDirection.z });
			lightColors.push_back(rgb);
			cutOffAngles.push_back(cutoffAng);
			lightIsSpot.push_back(1);

			LightRGB(rgb);
		}
		lastLight = lights[lights.size() - 1].position.z;
	}
}

void ParkBall::LightRGB(glm::vec3& rgb)
{
	if (rgb.r > 0.5)
	{
		if (rgb.b > 0.5)
		{
			rgb.b -= 0.1;
		}
		else if (rgb.g > 0.5)
		{
			rgb.g -= 0.1;
		}
		else
		{
			rgb.r -= 0.1;
		}
	}
	else
	{
		if (rgb.b < 1)
		{
			rgb.b += 0.1;
		}
		else if (rgb.g < 1)
		{
			rgb.g += 0.1;
		}
		else
		{
			rgb.r += 0.1;
		}
	}
}

void ParkBall::FrameStart()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void ParkBall::Update(float deltaTimeSeconds)
{
	if (!gameEnd)
	{
		AdvanceForward(deltaTimeSeconds);
		playerAngle += deltaTimeSeconds * speed;
		leafAngle += deltaTimeSeconds;

		{ // background
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, (horisont / 2 + POV3Z + PLAYER_HIT_RANGE)));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(FIELD_LENGHT, FIELD_LENGHT, FIELD_LENGHT));
			RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema3"], modelMatrix, mapTextures["sky"], noNeed, NORMAL_DISPLAY,
				noNeed, noNeed, 30, 0.5, 0.5);

			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, fallStop * 2, (horisont / 2 + POV3Z + PLAYER_HIT_RANGE)));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(FIELD_LENGHT, 1, FIELD_LENGHT));
			RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, mapTextures["ground"], noNeed, NORMAL_DISPLAY,
				noNeed, noNeed, 30, 0.5, 0.5);
		}

		RenderLights();

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
				playerStatus = GOGO_GHOST;
			}
			else if (stuckOnMaxSpeed && (Engine::GetElapsedTime() < maxSpeedStopTime)) // orange still in effect
			{
				playerStatus = GOGO_EFFECT; // return to orange deformation
			}
			else if (goGhost && (Engine::GetElapsedTime() < goGhostStopTime)) // ghost still in effect
			{
				playerStatus = GHOST_EFFECT; // return to ghost deformation
			}
			else
			{
				playerStatus = PLAYER_ROLL;
			}
		}
		if (stuckOnMaxSpeed && (Engine::GetElapsedTime() >= maxSpeedStopTime))
		{
			stuckOnMaxSpeed = false;
			playerStatus = PLAYER_ROLL;
		}
		if (goGhost && (Engine::GetElapsedTime() >= goGhostStopTime))
		{
			goGhost = false;
			playerStatus = PLAYER_ROLL;
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
			modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
			if (playerStatus == PLAYER_ROLL)
			{
				modelMatrix = glm::rotate(modelMatrix, -playerAngle, { 1, 0, 0 }); // Ox
			}
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
			RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema3"], modelMatrix, mapTextures["ball"], { 0, 1, 1 }, playerStatus,
				camera->GetTargetPosition(), noNeed, 30, 0.5, 0.5);
		}

		// fill distance to horisont
		while (platforms->farthestZ > horisont) // smallest number = farthest away
		{
			platforms->AddPlatforms(lives, wallTextures.size() );

			int lane = rand() % platforms->mainLanesCode.size();
			for (int i = 0; i < platforms->mainLanes.size(); i++)
			{
				if (lane % 10 == 0)
				{
					leaves.push_back(glm::vec3(platforms->mainLanes[i], 0.75, platforms->farthestZ));
				}
				lane = lane / 10;
			}
		}

		{ // render fuel bar; texture will be ignored
			RenderSimpleMesh(meshes["fuelBar"], shaders["ShaderTema3"], glm::mat4(1), mapTextures["ball"],
				{ 1, 1, 1 }, MONITOR, whiteBarPos, whiteBarSize, 0, 0, 0);

			fuelSize[0] = fuel / 10;
			float fuelLeft = fuel / maxFuel;
			RenderSimpleMesh(meshes["fuelBar"], shaders["ShaderTema3"], glm::mat4(1), mapTextures["ball"],
				{ 1 - fuelLeft, fuelLeft, 0 }, MONITOR, fuelPos, fuelSize, 0, 0, 0);
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
					RenderSimpleMesh(meshes["life"], shaders["ShaderTema3"], glm::mat4(1), mapTextures["ball"],
						{ 1, 0, 0 }, MONITOR, { livesPos[0] + float(i) / 10, livesPos[1], livesPos[2] }, livesSize, 0, 0, 0);
				}
				else // player can gather one extra life
				{
					RenderSimpleMesh(meshes["life"], shaders["ShaderTema3"], glm::mat4(1), mapTextures["ball"],
						{ 1, 0.5, 0.5 }, MONITOR, { livesPos[0] + float(i - 1) / 10, livesPos[1] - 0.1, livesPos[2] }, livesSize, 0, 0, 0);
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
						( platforms->walls[i].x >= ( (camera->GetTargetPosition())[0] - LANE_WIDTH / 2) )
						&& ( platforms->walls[i].x <= ( (camera->GetTargetPosition())[0] + LANE_WIDTH / 2) )
					)

					&& (
						(
							renderPlayer
							&& ( platforms->walls[i].startZ - WALLTHICKNESS ) > ( (camera->GetTargetPosition())[2] + PLAYER_HIT_RANGE )
						)
						|| (
							(!renderPlayer)
							&& ( ( platforms->walls[i].startZ - WALLTHICKNESS ) > ( camera->position[2] - POV1Y + PLAYER_HIT_RANGE ) )
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

		for (int i = 0; i < leaves.size(); i++)
		{
			if (leaves[i].z > camera->position[2]) // offscreen; don't render it anymore
			{
				leaves.erase(leaves.begin() + i);
				i--; // no leaf is skipped
			}
			else if (HasHitLeaf(i)) // offscreen; don't render it anymore
			{
				score++; printf("Score: %d\n", score);
				leaves.erase(leaves.begin() + i);
				i--; // no leaf is skipped
			}
			else // render leaf
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, leaves[i]);
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1));
				modelMatrix = glm::rotate(modelMatrix, leafAngle, { 0, 1, 0 }); // OY
				RenderSimpleMesh(meshes["leaf"], shaders["ShaderTema3"], modelMatrix, mapTextures["leaf"],
					{ 0, 0.5, 0 }, LEAF_DISPLAY, noNeed, noNeed, 0, 0, 0);
			}
		}
	}
	else
	{
		if (!showed)
		{
			if (highScore < score)
			{
				highScore = score;
			}
			printf("\n\nGAME END!\n\nFINAL SCORE: %d\nHIGHSCORE: %d\n\nPRESS R TO RESTART\n\n", score, highScore);
			showed = true;
		}
	}
}

void ParkBall::FrameEnd()
{
}

void ParkBall::Scoot(float deltaTimeSeconds)
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

void ParkBall::Jump(float deltaTimeSeconds)
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

void ParkBall::AdvanceForward(float deltaTimeSeconds)
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

bool ParkBall::IsOnPlatform(int i) // not airborn, on platform (maintaining course on crossing during scoot)
{
	return (
			( (!jump) && (groundLevel == camera->position[1]) ) // not airborn

			&& ( // on the platform lane
				( platforms->logList[i].x >= ( (camera->GetTargetPosition())[0] - LANE_WIDTH / 2 ) )
				&& ( platforms->logList[i].x <= ( (camera->GetTargetPosition())[0] + LANE_WIDTH / 2 ) )
				)

			&& ( // between platform sartZ and endZ
				(
					renderPlayer // player sphere is between platform bounds
					&& ( // startZ >= back edge of sphere;  endZ >= front edge of sphere
						( platforms->logList[i].startZ >= ( (camera->GetTargetPosition())[2] - PLAYER_HIT_RANGE ) )
						&& ( platforms->logList[i].endZ <= ( (camera->GetTargetPosition())[2] + PLAYER_HIT_RANGE ) )
						)
				)

				|| ( // screen edge between platform sartZ and endZ
					(!renderPlayer)
					&& ( // camera was set at POV3Z, translated with POV1Z
						( platforms->logList[i].startZ >= (camera->position[2] - POV1Y + PLAYER_HIT_RANGE) )
						&& ( platforms->logList[i].endZ <= (camera->position[2] - POV1Y + PLAYER_HIT_RANGE) )
						)
					)
				)
			);
}

bool ParkBall::HasHitWall(int i) // in wall (maintaining course or crossing during scoot)
{
	return (
			( // on same lane
				( platforms->walls[i].x >= ( (camera->GetTargetPosition())[0] - LANE_WIDTH / 2 ) )
				&& ( platforms->walls[i].x <= ( (camera->GetTargetPosition())[0] + LANE_WIDTH / 2 ) )
			)

			&& ( // intersects with player on z axis
				(
					renderPlayer
					&& (
						( // front side in wall
							( platforms->walls[i].startZ >= ( (camera->GetTargetPosition())[2] - PLAYER_HIT_RANGE ) )
							&& ( platforms->walls[i].startZ <= ((camera->GetTargetPosition())[2] + PLAYER_HIT_RANGE ) )
						)
						|| ( // backside in wall
							( ( platforms->walls[i].startZ - WALLTHICKNESS ) >= ( (camera->GetTargetPosition())[2] - PLAYER_HIT_RANGE ) )
							&& ( ( platforms->walls[i].startZ - WALLTHICKNESS ) <= ( (camera->GetTargetPosition())[2] + PLAYER_HIT_RANGE ) )
							)
						|| ( // wall inside sphere
							( platforms->walls[i].startZ <= ( (camera->GetTargetPosition())[2] + PLAYER_HIT_RANGE ) )
							&& ( ( platforms->walls[i].startZ - WALLTHICKNESS ) >= ( (camera->GetTargetPosition())[2] - PLAYER_HIT_RANGE ) )
							)
					   )
				)

				|| (
					(!renderPlayer)
					&& (
						( platforms->walls[i].startZ >= (camera->position[2] - POV1Y + PLAYER_HIT_RANGE) )
						&& ( ( platforms->walls[i].startZ - WALLTHICKNESS ) <= (camera->position[2] - POV1Y + PLAYER_HIT_RANGE) )
						)
					)
			   )

			&& ( // intersects with player on y axis
				(
					renderPlayer
					&& ( ( platforms->walls[i].hight - PLATFORM_THICKNESS ) >= ( (camera->GetTargetPosition())[1] - PLAYER_HIT_RANGE ) )
				)

				|| (
					(!renderPlayer)
					&& ( ( platforms->walls[i].hight - PLATFORM_THICKNESS ) >= (camera->position[1] - POV1Y + PLAYER_HIT_RANGE) )
					)
			   )
		  );
}

bool ParkBall::HasHitLeaf(int i) // in wall (maintaining course or crossing during scoot)
{
	return (
		( // on same lane
			(leaves[i].x >= ((camera->GetTargetPosition())[0] - LANE_WIDTH / 2))
			&& (leaves[i].x <= ((camera->GetTargetPosition())[0] + LANE_WIDTH / 2))
			)

		&& ( // intersects with player on z axis
			(
				renderPlayer
				&& (
					( // front side in wall
						(leaves[i].z >= ((camera->GetTargetPosition())[2] - PLAYER_HIT_RANGE))
						&& (leaves[i].z <= ((camera->GetTargetPosition())[2] + PLAYER_HIT_RANGE))
						)
					|| ( // backside in wall
						((leaves[i].z) >= ((camera->GetTargetPosition())[2] - PLAYER_HIT_RANGE))
						&& ((leaves[i].z) <= ((camera->GetTargetPosition())[2] + PLAYER_HIT_RANGE))
						)
					|| ( // wall inside sphere
						(leaves[i].z <= ((camera->GetTargetPosition())[2] + PLAYER_HIT_RANGE))
						&& ((leaves[i].z) >= ((camera->GetTargetPosition())[2] - PLAYER_HIT_RANGE))
						)
					)
				)

			|| (
				(!renderPlayer)
				&& (
					(leaves[i].z >= (camera->position[2] - POV1Y + PLAYER_HIT_RANGE))
					&& ((leaves[i].z) <= (camera->position[2] - POV1Y + PLAYER_HIT_RANGE))
					)
				)
			)

		&& ( // intersects with player on y axis
			(
				renderPlayer
				&& ((leaves[i].y) >= ((camera->GetTargetPosition())[1] - PLAYER_HIT_RANGE))
				)

			|| (
				(!renderPlayer)
				&& ((leaves[i].y) >= (camera->position[1] - POV1Y + PLAYER_HIT_RANGE))
				)
			)
		);
}

void ParkBall::AnimationSetup() // for powerups that take effect instantenuously
{
	effectAnimation = true;
	powerUpAnimationStartTime = Engine::GetElapsedTime();
	powerUpAnimationStopTime = powerUpAnimationStartTime + powerUpAnimationTime;
}

void ParkBall::RenderLights()
{
	for (int i = 0; i < lights.size(); i++)
	{
		if (lights[i].position.z > camera->position[2])
		{
			lights[i].position.z = lastLight - distBetweenLights;
			lastLight = lights[i].position.z;
		}
		if (lights[i].isSpot == 1)
		{
			RenderLamppost(i);
		}
		else
		{
			RenderGroundLamp(i);
		}
	}
}

void ParkBall::RenderLamppost(int i)
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, lights[i].position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25));
	RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema3"], modelMatrix, mapTextures["lamp"],
		lights[i].color, LIGHT_DISPLAY, noNeed, noNeed, 0, 0, 0);

	float side = 1;
	if (lights[i].position.x < 0)
	{
		side = -1;
	}
	modelMatrix = glm::translate(modelMatrix, { side * 0.5, 0.5, 0 });
	modelMatrix = glm::rotate(modelMatrix, side * RADIANS(180) * 0.75f, { 0, 0, 1 }); // OZ
	RenderSimpleMesh(meshes["cone"], shaders["ShaderTema3"], modelMatrix, mapTextures["metal"], noNeed,
		NORMAL_DISPLAY, noNeed, noNeed, 30, 0.5, 0.5);

	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, lights[i].position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25));
	modelMatrix = glm::translate(modelMatrix, { side * 1.25, -0.1, 0 });
	modelMatrix = glm::rotate(modelMatrix, side * RADIANS(180) * 0.25f, { 0, 0, 1 }); // OZ
	RenderSimpleMesh(meshes["poleArm"], shaders["ShaderTema3"], modelMatrix, mapTextures["lamp"], noNeed,
		NORMAL_DISPLAY, noNeed, noNeed, 30, 0.5, 0.5);

	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, lights[i].position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25));
	modelMatrix = glm::translate(modelMatrix, { side * 2.1, -5, 0 });
	RenderSimpleMesh(meshes["roundPole"], shaders["ShaderTema3"], modelMatrix, mapTextures["lamp"], noNeed,
		NORMAL_DISPLAY, noNeed, noNeed, 30, 0.5, 0.5);

	modelMatrix = glm::translate(modelMatrix, { 0, -4.5, 0 });
	RenderSimpleMesh(meshes["poleStump"], shaders["ShaderTema3"], modelMatrix, mapTextures["lamp"], noNeed,
		NORMAL_DISPLAY, noNeed, noNeed, 30, 0.5, 0.5);

	modelMatrix = glm::translate(modelMatrix, { 0, -1.5, 0 });
	RenderSimpleMesh(meshes["roundBase"], shaders["ShaderTema3"], modelMatrix, mapTextures["pavement"], noNeed,
		NORMAL_DISPLAY, noNeed, noNeed, 30, 0.5, 0.5);
}

void ParkBall::RenderGroundLamp(int i)
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, lights[i].position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25));
	RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema3"], modelMatrix, mapTextures["lamp"],
		lights[i].color, LIGHT_DISPLAY, noNeed, noNeed, 0, 0, 0);

	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, lights[i].position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2));
	modelMatrix = glm::translate(modelMatrix, { 0, 1.3, 0 });
	modelMatrix = glm::rotate(modelMatrix, RADIANS(180), { 0, 0, 1 }); // OZ
	RenderSimpleMesh(meshes["squareBase"], shaders["ShaderTema3"], modelMatrix, mapTextures["lamp"], noNeed,
		NORMAL_DISPLAY, noNeed, noNeed, 30, 0.5, 0.5);

	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, lights[i].position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2));
	modelMatrix = glm::translate(modelMatrix, { 0, -1.3, 0 });
	RenderSimpleMesh(meshes["lampBase"], shaders["ShaderTema3"], modelMatrix, mapTextures["lamp"], noNeed,
		NORMAL_DISPLAY, noNeed, noNeed, 30, 0.5, 0.5);

	modelMatrix = glm::translate(modelMatrix, { 0, -1.2, 0 });
	modelMatrix = glm::rotate(modelMatrix, RADIANS(180), { 0, 0, 1 }); // OZ
	RenderSimpleMesh(meshes["lampBase2"], shaders["ShaderTema3"], modelMatrix, mapTextures["lamp"], noNeed,
		NORMAL_DISPLAY, noNeed, noNeed, 30, 0.5, 0.5);

	modelMatrix = glm::rotate(modelMatrix, RADIANS(180), { 0, 0, 1 }); // OZ
	modelMatrix = glm::translate(modelMatrix, { 0, -1.5, 0 });
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5));
	RenderSimpleMesh(meshes["roundBase"], shaders["ShaderTema3"], modelMatrix, mapTextures["pavement"], noNeed,
		NORMAL_DISPLAY, noNeed, noNeed, 30, 0.5, 0.5);
}

void ParkBall::RenderPlatform(int i)
{
	// Translate needs coordonates for the center of the object
	float length = platforms->logList[i].endZ - platforms->logList[i].startZ;
	float offset = platforms->logList[i].startZ - abs(length / 2);

	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, { platforms->logList[i].x, PLATFORM_BOTTOM, offset });
	modelMatrix = glm::scale(modelMatrix, { LANE_WIDTH, PLATFORM_THICKNESS, abs(length) });

	switch (platforms->logList[i].colorCode)
	{
	case TOUCHED:
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, mapTextures["road-purple"], noNeed,
			NORMAL_DISPLAY, noNeed, noNeed, 30, 0.5, 0.5);
		break;

	case NORMAL:
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, mapTextures["road-blue"], noNeed,
			NORMAL_DISPLAY, noNeed, noNeed, 30, 0.5, 0.5);
		break;

	case PLUSFUEL:
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, mapTextures["road-green"], noNeed,
			NORMAL_DISPLAY, noNeed, noNeed, 30, 0.5, 0.5);
		break;

	case GOGO:
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, mapTextures["road-orange"], noNeed,
			NORMAL_DISPLAY, noNeed, noNeed, 30, 0.5, 0.5);
		break;

	case MINUSFUEL:
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, mapTextures["road-yellow"], noNeed,
			NORMAL_DISPLAY, noNeed, noNeed, 30, 0.5, 0.5);
		break;

	case DEATH:
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, mapTextures["road-red"], noNeed,
			NORMAL_DISPLAY, noNeed, noNeed, 30, 0.5, 0.5);
		break;

	case MINUSLIFE:
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, mapTextures["road-minuslife"], noNeed,
			NORMAL_DISPLAY, noNeed, noNeed, 30, 0.5, 0.5);
		break;

	case PLUSLIFE:
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, mapTextures["road-pluslife"], noNeed,
			NORMAL_DISPLAY, noNeed, noNeed, 30, 0.5, 0.5);
		break;

	case GHOST:
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, mapTextures["road-ghost"], noNeed,
			NORMAL_DISPLAY, noNeed, noNeed, 30, 0.5, 0.5);
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
		if (platforms->logList[i].active)
		{
			switch (platforms->logList[i].colorCode)
			{
			case PLUSFUEL:
				fuel += fuelGain;
				if (fuel > maxFuel)
				{
					fuel = maxFuel;
				}
				AnimationSetup();
				playerStatus = PLUS_FUEL_EFFECT;
				break;

			case MINUSFUEL:
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
				playerStatus = MINUS_FUEL_EFFECT;
				break;

			case GOGO:
				stuckOnMaxSpeed = true;
				speed = speedMax;
				maxSpeedStartTime = Engine::GetElapsedTime();
				maxSpeedStopTime = maxSpeedStartTime + orangeActionTime;

				if (goGhost) // ghost also active
				{
					playerStatus = GOGO_GHOST;
				}
				else
				{
					playerStatus = GOGO_EFFECT;
				}
				break;

			case DEATH:
				gameEnd = true;
				break;

			case MINUSLIFE:
				lives--;
				if (lives == 0)
				{
					gameEnd = true;
				}
				AnimationSetup();
				playerStatus = MINUS_LIFE_EFFECT;
				break;

			case PLUSLIFE:
				if (lives < 4)
				{
					lives++;
				}
				AnimationSetup();
				playerStatus = PLUS_LIFE_EFFECT;
				break;

			case GHOST:
				goGhost = true;
				goGhostStartTime = Engine::GetElapsedTime();
				goGhostStopTime = goGhostStartTime + ghostActionTime;

				if (stuckOnMaxSpeed) // orange also active
				{
					playerStatus = GOGO_GHOST;
				}
				else
				{
					playerStatus = GHOST_EFFECT;
				}
				break;

			default:
				break;
			}
			platforms->logList[i].active = false;
		}
	}
}

void ParkBall::RenderWall(float deltaTimeSeconds, int i)
{
	// walls rise from within platform
	// find center of wall
	float offset = PLATFORM_BOTTOM + platforms->walls[i].hight / 2;

	// width wall < width lane so no ugly glitching at the bottom of the wall where it intersects with the platform
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, { platforms->walls[i].x, offset, platforms->walls[i].startZ });
	modelMatrix = glm::scale(modelMatrix, { LANE_WIDTH - 0.05, platforms->walls[i].hight, WALLTHICKNESS });

	// render wall with random texture from wall texure pool
	int texCode = platforms->walls[i].texture;
	string tex = wallTextures[texCode];
	RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, mapTextures[tex], noNeed, 1, noNeed, noNeed, 30, 0.5, 0.5);

	if ((((camera->GetTargetPosition())[2] - SPRING_DISTANCE * speed) <= platforms->walls[i].startZ)
		&& platforms->walls[i].spring
		&& (platforms->walls[i].hight != platforms->walls[i].finalHight))
		// player aproaches spring wall
	{
		if (platforms->walls[i].hight != platforms->walls[i].finalHight)
		{
			platforms->walls[i].hight += deltaTimeSeconds * WALL_RISE_SPEED;
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
			playerStatus = HIT_EFFECT;

			// delete hit wall
			platforms->walls.erase(platforms->walls.begin() + i);
		}
	}
}

void ParkBall::OnInputUpdate(float deltaTime, int mods)
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

void ParkBall::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture, const glm::vec3& color,
	const int objectCode, const glm::vec3& objectPos, const glm::vec3& objectSize,
	float materialShininess, float materialKd, float materialKs)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	glUseProgram(shader->program);

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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
	glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);

	GLint object_color_location = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(object_color_location, 1, glm::value_ptr(color));

	GLint object_position_location = glGetUniformLocation(shader->program, "object_pos");
	glUniform3fv(object_position_location, 1, glm::value_ptr(objectPos));

	GLint object_size_location = glGetUniformLocation(shader->program, "object_size");
	glUniform3fv(object_size_location, 1, glm::value_ptr(objectSize));

	int loc_object_code = glGetUniformLocation(shader->program, "object_code");
	glUniform1i(loc_object_code, objectCode);

	int loc_time = glGetUniformLocation(shader->program, "ElapsedTime");
	float elapsedTime = Engine::GetElapsedTime();
	glUniform1f(loc_time, elapsedTime);

	int loc_speed = glGetUniformLocation(shader->program, "speed");
	glUniform1f(loc_speed, speed);

	// light sources
/*	std::string aux;
	std::string currentLight;

	for (int i = 0; i < lights.size(); i++)
	{
		currentLight = std::to_string(i);

		aux = "light_position[" + currentLight + "]";
		glUniform3fv(glGetUniformLocation(shader->program, (const GLchar*)aux.c_str()), 1, glm::value_ptr(lights[i].position));

		aux = "light_direction[" + currentLight + "]";
		glUniform3fv(glGetUniformLocation(shader->program, (const GLchar*)aux.c_str()), 1, glm::value_ptr(lights[i].direction));

		aux = "spotlight[" + currentLight + "]";
		glUniform1i(glGetUniformLocation(shader->program, (const GLchar*)aux.c_str()), lights[i].isSpot);

		aux = "cut_off_angle[" + currentLight + "]";
		glUniform1f(glGetUniformLocation(shader->program, (const GLchar*)aux.c_str()), lights[i].cut_off_angle);

		aux = "light_color[" + currentLight + "]";
		glUniform3fv(glGetUniformLocation(shader->program, (const GLchar*)aux.c_str()), 1, glm::value_ptr(lights[i].color));
	}*/

	glUniform3fv(glGetUniformLocation(shader->program, "light_position"), lights.size(), glm::value_ptr(lightPositions[0]));
	glUniform3fv(glGetUniformLocation(shader->program, "light_direction"), lights.size(), glm::value_ptr(lightDirections[0]));
	glUniform3fv(glGetUniformLocation(shader->program, "light_color"), lights.size(), glm::value_ptr(lightColors[0]));
	glUniform1fv(glGetUniformLocation(shader->program, "cut_off_angle"), lights.size(), &cutOffAngles[0]);
	glUniform1iv(glGetUniformLocation(shader->program, "spotlight"), lights.size(), &lightIsSpot[0]);

	int lights_nr_location = glGetUniformLocation(shader->program, "lights_nr");
	glUniform1i(lights_nr_location, lights.size());

	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void ParkBall::OnKeyPress(int key, int mods)
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
			scootEndX = camera->position[0] - LANE_WIDTH; // enough to go one lane over
		}
		else if (scootEndX > (-LANE_WIDTH * 4))
		{
			// A was pressed while player was still scooting
			// can go up to max 4 lanes to the left

			scootEndX -= LANE_WIDTH; // longer scoot

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
			scootEndX = camera->position[0] + LANE_WIDTH; // enough to go one lane over
		}
		else if (scootEndX < (LANE_WIDTH * 4))
		{
			// D was pressed while player was still scooting
			// can go up to max 4 lanes to the right

			scootEndX += LANE_WIDTH; // longer scoot

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

void ParkBall::OnKeyRelease(int key, int mods)
{
}

void ParkBall::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void ParkBall::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void ParkBall::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void ParkBall::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void ParkBall::OnWindowResize(int width, int height)
{
}