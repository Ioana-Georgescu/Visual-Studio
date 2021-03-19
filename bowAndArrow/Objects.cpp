#include "Objects.h"

#include <Core/Engine.h>

Mesh* Objects::CreateBow(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, -1, 0), color),
		VertexFormat(glm::vec3(0, 1, 0), color)
	};
	std::vector<unsigned short> indices = { 0, 1 };
	int c = 2;
	for (float angle = 0; angle < 180 * M_PI / 180; angle += 0.2f)
	{
		vertices.push_back(VertexFormat(glm::vec3(sin(angle), cos(angle), 0), glm::vec3(0, 1, 0)));
		indices.push_back(c);
		c++;
	}
	Mesh* bow = CreateMesh(name, vertices, indices);
	bow->SetDrawMode(GL_LINE_LOOP);
	return bow;
}

Mesh* Objects::CreateArrowLine(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(1, 0, 0), color)
	};
	std::vector<unsigned short> indices = { 0, 1 };
	Mesh* arrowLine = CreateMesh(name, vertices, indices);
	arrowLine->SetDrawMode(GL_LINES);
	return arrowLine;
}

Mesh* Objects::CreateArrowTip(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0.5f, 0.25f, 0), color),
		VertexFormat(glm::vec3(0, 0.5f, 0), color),
		VertexFormat(glm::vec3(0, 0, 0), color)
	};
	std::vector<unsigned short> indices = { 0, 1, 2 };
	Mesh* arrowTip = CreateMesh(name, vertices, indices);
	arrowTip->SetDrawMode(GL_TRIANGLES);
	return arrowTip;
}

Mesh* Objects::CreateBaloonHead(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color) // center
	};
	std::vector<unsigned short> indices = { 0 };
	int c = 1;
	for (float angle = 360 * M_PI / 180; angle > -0.2f; angle -= 0.2f)
	{
		vertices.push_back(VertexFormat(glm::vec3(sin(angle), cos(angle) * 1.25f, 0), color));
		indices.push_back(c);
		c++;
	}
	Mesh* baloonHead = CreateMesh(name, vertices, indices);
	baloonHead->SetDrawMode(GL_TRIANGLE_FAN);
	return baloonHead;
}

Mesh* Objects::CreateBaloonBase(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(-0.25f, 0, 0), color),
		VertexFormat(glm::vec3(0.25f, 0, 0), color),
		VertexFormat(glm::vec3(0, 1, 0), color)
	};
	std::vector<unsigned short> indices = { 0, 1, 2 };
	Mesh* baloonBase = CreateMesh(name, vertices, indices);
	baloonBase->SetDrawMode(GL_TRIANGLES);
	return baloonBase;
}

Mesh* Objects::CreateBaloonString(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(0.1f, -0.1f, 0), color),
		VertexFormat(glm::vec3(-0.1f, -0.2f, 0), color),
		VertexFormat(glm::vec3(0.1f, -0.4f, 0), color),
		VertexFormat(glm::vec3(-0.1f, -0.5f, 0), color)
	};
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4 };
	Mesh* baloonString = CreateMesh(name, vertices, indices);
	baloonString->SetDrawMode(GL_LINE_STRIP);
	return baloonString;
}

Mesh* Objects::CreateBlade(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(0.5f, 0.5f, 0), color),
		VertexFormat(glm::vec3(1, 0, 0), color)
	};
	std::vector<unsigned short> indices =
	{
		0, 1, 2
	};
	Mesh* blade = CreateMesh(name, vertices, indices);
	blade->SetDrawMode(GL_TRIANGLES);
	return blade;
}

Mesh* Objects::CreateShuriken(std::string name, glm::vec3 center, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(center[0], center[1], 0), color),
		VertexFormat(glm::vec3(-0.5f + center[0], 0.5f + center[1], 0), color),
		VertexFormat(glm::vec3(-1 + center[0], center[1], 0), color),
		VertexFormat(glm::vec3(-0.5f + center[0], -0.5f + center[1], 0), color),
		VertexFormat(glm::vec3(center[0], -1 + center[1], 0), color),
		VertexFormat(glm::vec3(0.5f + center[0], -0.5f + center[1], 0), color),
		VertexFormat(glm::vec3(1 + center[0], center[1], 0), color),
		VertexFormat(glm::vec3(0.5f + center[0], 0.5f + center[1], 0), color),
		VertexFormat(glm::vec3(center[0], 1 + center[1], 0), color)
	};
	std::vector<unsigned short> indices =
	{
		0, 1, 2,
		0, 3, 4,
		0, 5, 6,
		0, 7, 8
	};
	Mesh* shuriken = CreateMesh(name, vertices, indices);
	shuriken->SetDrawMode(GL_TRIANGLES);
	return shuriken;
}

Mesh* Objects::CreateSpeedBar(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0.25f, 0, 0), color),
		VertexFormat(glm::vec3(0.25f, 0.25f, 0), color),
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(0, 0.25f, 0), color)
	};
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	Mesh* speedBar = CreateMesh(name, vertices, indices);
	speedBar->SetDrawMode(GL_TRIANGLE_STRIP);
	return speedBar;
}

Mesh* Objects::CreateMesh(std::string name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices)
{
	unsigned int VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	unsigned int IBO = 0;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	
	glBindVertexArray(0);

	CheckOpenGLError();

	Mesh* newMesh = new Mesh(name);
	newMesh->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	return newMesh;
}