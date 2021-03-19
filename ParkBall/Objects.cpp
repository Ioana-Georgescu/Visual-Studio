#include "Objects.h"

Mesh* ParkBallAux::CreateFuelbar(std::string name)
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

Mesh* ParkBallAux::CreateLife(std::string name)
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

Mesh* ParkBallAux::CreateCone(std::string name)
{
	glm::vec3 center = glm::vec3(0, 1, 0);

	std::vector<glm::vec3> vertices =
	{
		glm::vec3(0, -1, 0) // tip
	};

	std::vector<glm::vec3> normals =
	{
		glm::vec3(0, 0, 0)
	};

	std::vector<glm::vec2> textureCoords =
	{
		glm::vec2(0.5, 0.5)
	};

	std::vector<unsigned short> indices = { 0 };

	int c = 1;
	for (float angle = 360 * M_PI / 180; angle > -0.2f; angle -= 0.2f)
	{
		float x = sin(angle);
		float y = cos(angle);

		vertices.push_back(glm::vec3(x, 1, y));
		normals.push_back( normalize( glm::vec3(x, 1, y) ) );
		textureCoords.push_back(glm::vec2(0.5 + x / 2, 0.5 + y / 2));
		indices.push_back(c);
		c++;
	}
	Mesh* cone = new Mesh(name);
	cone->InitFromData(vertices, normals, textureCoords, indices);
	cone->SetDrawMode(GL_TRIANGLE_FAN);
	return cone;
}

Mesh* ParkBallAux::CreateLeaf(std::string name)
{
	glm::vec3 center = glm::vec3(0, 1, 0);

	std::vector<glm::vec3> vertices =
	{ // object center (0, 0, 0)
		glm::vec3(0, 3, 0.25), // tip front
		glm::vec3(1, 0, 0.25), // right front
		glm::vec3(-1, 0, 0.25), // left front
		glm::vec3(0, -2, 0.25), // down front
		glm::vec3(0.25, -3, 0.25), // tail right front
		glm::vec3(-0.25, -3, 0.25), // tail left front

		glm::vec3(0, 3, -0.25), // tip back
		glm::vec3(1, 0, -0.25), // right back
		glm::vec3(-1, 0, -0.25), // left back
		glm::vec3(0, -2, -0.25), // down back
		glm::vec3(0.25, -3, -0.25), // tail right back
		glm::vec3(-0.25, -3, -0.25) // tail left back
	};

	std::vector<glm::vec3> normals =
	{
		normalize(glm::vec3(0, 3, 0.25)), // tip front
		normalize(glm::vec3(1, 0, 0.25)), // right front
		normalize(glm::vec3(-1, 0, 0.25)), // left front
		normalize(glm::vec3(0, -2, 0.25)), // down front
		normalize(glm::vec3(0.25, -3, 0.25)), // tail right front
		normalize(glm::vec3(-0.25, -3, 0.25)), // tail left front

		normalize(glm::vec3(0, 3, -0.25)), // tip back
		normalize(glm::vec3(1, 0, -0.25)), // right back
		normalize(glm::vec3(-1, 0, -0.25)), // left back
		normalize(glm::vec3(0, -2, -0.25)), // down back
		normalize(glm::vec3(0.25, -3, -0.25)), // tail right back
		normalize(glm::vec3(-0.25, -3, -0.25)) // tail left back
	};

	std::vector<glm::vec2> textureCoords =
	{
		glm::vec2(0.5, 1),
		glm::vec2(0.75, 0.5),
		glm::vec2(0.25, 0.5),
		glm::vec2(0.5, 0.25),
		glm::vec2(0.3, 0),
		glm::vec2(0.7, 0),

		glm::vec2(0.5, 1),
		glm::vec2(0.75, 0.5),
		glm::vec2(0.25, 0.5),
		glm::vec2(0.5, 0.25),
		glm::vec2(0.3, 0),
		glm::vec2(0.7, 0)
	};

	std::vector<unsigned short> indices =
	{
		0, 2, 1, // front
		2, 3, 1,
		3, 5, 4,

		6, 7, 8, // back
		7, 9, 8,
		9, 10, 11,

		0, 1, 7, // right side
		0, 7, 6,
		1, 3, 9,
		1, 9, 7,
		3, 4, 10,
		3, 10, 9,

		6, 7, 2, // left side
		6, 2, 0,
		7, 9, 3,
		7, 3, 1,
		9, 10, 4,
		9, 4, 3
	};

	Mesh* leaf = new Mesh(name);
	leaf->InitFromData(vertices, normals, textureCoords, indices);
	leaf->SetDrawMode(GL_TRIANGLES);
	return leaf;
}

Mesh* ParkBallAux::CreateRoundPole(std::string name, float radiusTop, float radiusBottom, float height)
{
	float radiusMiddle = (radiusTop + radiusBottom) / 2;
	int indexCenterBase = 0;
	int indexCenterTop = 1;

	// add centers for base and top
	std::vector<glm::vec3> vertices = { glm::vec3(0, height / 2, 0), glm::vec3(0, -height / 2, 0) };
	std::vector<glm::vec3> normals = { glm::vec3(0, -1, 0), glm::vec3(0, 1, 0) };
	std::vector<glm::vec2> textureCoords = { glm::vec2(0.5, 0.5), glm::vec2(0.5, 0.5) }; // texture center
	std::vector<unsigned short> indices;

	int vertIndx = 2; // start counting indices for the other verteces
	int top, middle, bottom;

	/*
	* 2____5...
	* | \  |
	* |  \ |
	* 3____6...
	* | \  |
	* |  \ |
	* 4____7...
	*/
	for (float angle = 2 * M_PI; angle > -0.2f; angle -= 0.2f)
	{
		float x = sin(angle);
		float y = cos(angle);

		// top circle
		top = vertIndx;
		vertices.push_back(glm::vec3(x * radiusTop, height / 2, y * radiusTop));
		normals.push_back(glm::vec3(0, 1, 0));
		textureCoords.push_back(glm::vec2(0.5 + x / 2, 0.5 + y / 2));
		vertIndx++;

		// middle
		middle = vertIndx;
		vertices.push_back(glm::vec3(x * radiusMiddle, 0, y * radiusMiddle));
		normals.push_back(normalize(glm::vec3(x * radiusMiddle, 0, y * radiusMiddle)));
		textureCoords.push_back(glm::vec2(x, 0.5));
		vertIndx++;

		// base circle
		bottom = vertIndx;
		vertices.push_back(glm::vec3(x * radiusBottom, - height / 2, y * radiusBottom));
		normals.push_back(glm::vec3(0, -1, 0));
		textureCoords.push_back(glm::vec2(0.5 + x / 2, 0.5 + y / 2));
		vertIndx++;

		if (vertIndx >= 7)
		{
			MakeWall(indices, top - 3, top);
			MakeWall(indices, middle - 3, middle);
			MakeSlices(indices, top - 3, top, indexCenterTop, indexCenterBase);
		}
	}
	// close the circle
	MakeWall(indices, top, 2);
	MakeWall(indices, middle, middle);
	MakeSlices(indices, top, 2, indexCenterTop, indexCenterBase);

	Mesh* mesh = new Mesh(name);
	mesh->InitFromData(vertices, normals, textureCoords, indices);
	mesh->SetDrawMode(GL_TRIANGLES);
	return mesh;
}

Mesh* ParkBallAux::CreateSquarePole(std::string name, float diagTop, float diagBottom, float height)
{
	float diagMiddle = (diagTop + diagBottom) / 2;
	int indexCenterBase = 0;
	int indexCenterTop = 1;

	// add centers for base and top
	std::vector<glm::vec3> vertices = { glm::vec3(0, height / 2, 0), glm::vec3(0, -height / 2, 0) };
	std::vector<glm::vec3> normals = { glm::vec3(0, -1, 0), glm::vec3(0, 1, 0) };
	std::vector<glm::vec2> textureCoords = { glm::vec2(0.5, 0.5), glm::vec2(0.5, 0.5) }; // texture center
	std::vector<unsigned short> indices;

	int vertIndx = 2; // start counting indices for the other verteces
	int top, middle, bottom;

	/*
	* 2____5...7...10
	* | \  |
	* |  \ |
	* 3____6...
	* | \  |
	* |  \ |
	* 4____7...
	*/
	for (int i = 0; i < 4; i++)
	{
		float x, y;
		switch (i)
		{
		case 0:
			x = 1 / sqrt(2);
			y = 1 / sqrt(2);
			break;
		case 1:
			x = 1 / sqrt(2);
			y = -1 / sqrt(2);
			break;
		case 2:
			x = -1 / sqrt(2);
			y = -1 / sqrt(2);
			break;
		case 3:
			x = -1 / sqrt(2);
			y = 1 / sqrt(2);
			break;
		default:
			break;
		}

		// top square
		top = vertIndx;
		vertices.push_back(glm::vec3(x * diagTop, height / 2, y * diagTop));
		normals.push_back(glm::vec3(0, 1, 0));
		textureCoords.push_back(glm::vec2(0.5 + x / 2, 0.5 + y / 2));
		vertIndx++;

		// middle
		middle = vertIndx;
		vertices.push_back(glm::vec3(x * diagMiddle, 0, y * diagMiddle));
		normals.push_back(normalize(glm::vec3(x * diagMiddle, 0, y * diagMiddle)));
		textureCoords.push_back(glm::vec2(x, 0.5));
		vertIndx++;

		// base square
		bottom = vertIndx;
		vertices.push_back(glm::vec3(x * diagBottom, -height / 2, y * diagBottom));
		normals.push_back(glm::vec3(0, -1, 0));
		textureCoords.push_back(glm::vec2(0.5 + x / 2, 0.5 + y / 2));
		vertIndx++;

		if (vertIndx >= 7)
		{
			MakeWall(indices, top - 3, top);
			MakeWall(indices, middle - 3, middle);
			MakeSlices(indices, top - 3, top, indexCenterTop, indexCenterBase);
		}
	}
	// close the circle
	MakeWall(indices, top, 2);
	MakeWall(indices, middle, middle);
	MakeSlices(indices, top, 2, indexCenterTop, indexCenterBase);

	Mesh* mesh = new Mesh(name);
	mesh->InitFromData(vertices, normals, textureCoords, indices);
	mesh->SetDrawMode(GL_TRIANGLES);
	return mesh;
}

void ParkBallAux::MakeWall(std::vector<unsigned short> &indices, int up1, int up2)
{
	/*
	*   up1___up2
	*    | \    |
	*    |  \   |
	*    |   \  |
	*    |    \ |
	* up1t+1____up2+1
	*/

	indices.push_back(up1);
	indices.push_back(up1 + 1);
	indices.push_back(up2 + 1);

	indices.push_back(up1);
	indices.push_back(up2 + 1);
	indices.push_back(up2);
}

void ParkBallAux::MakeSlices(std::vector<unsigned short> &indices, int up1, int up2, int indexCenterTop, int indexCenterBase)
{
	/*
	* indexCenterTop
	*   /   \
	* up1___up2
	* 
	*   indexCenterBase
	*	 /      \
	* up1+2____up2+2
	*/
	
	indices.push_back(indexCenterTop);
	indices.push_back(up1);
	indices.push_back(up2);

	indices.push_back(indexCenterBase);
	indices.push_back(up1 + 2);
	indices.push_back(up2 + 2);
}