#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Objects
{
	Mesh* CreateBow(std::string name, glm::vec3 color);
	Mesh* CreateArrowLine(std::string name, glm::vec3 color);
	Mesh* CreateArrowTip(std::string name, glm::vec3 color);
	Mesh* CreateBaloonHead(std::string name, glm::vec3 color);
	Mesh* CreateBaloonBase(std::string name, glm::vec3 color);
	Mesh* CreateBaloonString(std::string name, glm::vec3 color);
	Mesh* CreateBlade(std::string name, glm::vec3 color);
	Mesh* CreateShuriken(std::string name, glm::vec3 center, glm::vec3 color);
	Mesh* CreateSpeedBar(std::string name, glm::vec3 color);
	Mesh* CreateMesh(std::string name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);
}