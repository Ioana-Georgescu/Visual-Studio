#pragma once

#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

namespace ParkBallAux
{
	Mesh* CreateFuelbar(std::string name);
	Mesh* CreateLife(std::string name);
	Mesh* CreateCone(std::string name);
	Mesh* CreateLeaf(std::string name);
	Mesh* CreateRoundPole(std::string name, float radiusTop, float radiusBottom, float height);
	Mesh* CreateSquarePole(std::string name, float diagTop, float diagBottom, float height);
	void MakeWall(std::vector<unsigned short> &indices, int up1, int up2);
	void MakeSlices(std::vector<unsigned short>& indices, int up1, int up2, int indexCenterTop, int indexCenterBase);

	struct LightLog
	{
		LightLog() : position(glm::vec3(0)), direction(glm::vec3(0)), color(glm::vec3(0)), cut_off_angle(0), isSpot(0) {}
		LightLog(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float cut_off_angle, int isSpot)
			: position(position), direction(direction), color(color), cut_off_angle(cut_off_angle), isSpot(isSpot) {}

		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 color;
		float cut_off_angle;
		int isSpot;
	};
}