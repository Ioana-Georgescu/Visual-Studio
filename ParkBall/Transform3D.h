#pragma once

#include <include/glm.h>

namespace Transform3D
{
	// Translate matrix
	inline glm::mat4 Translate(glm::vec3 translate)
	{
		return glm::transpose(
			glm::mat4(
				1, 0, 0, translate[0],
				0, 1, 0, translate[1],
				0, 0, 1, translate[2],
				0, 0, 0, 1
			)
		);
	}

	// Scale matrix
	inline glm::mat4 Scale(glm::vec3 scale)
	{
		return glm::transpose(
			glm::mat4(
				scale[0], 0, 0, 0,
				0, scale[1], 0, 0,
				0, 0, scale[2], 0,
				0, 0, 0, 1
			)
		);
	}

	// Rotate matrix relative to the OZ axis
	inline glm::mat4 RotateOZ(float radians)
	{
		return glm::transpose(
			glm::mat4(
				cos(radians), -sin(radians), 0, 0,
				sin(radians), cos(radians), 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			)
		);
	}

	// Rotate matrix relative to the OY axis
	inline glm::mat4 RotateOY(float radians)
	{
		return glm::transpose(
			glm::mat4(
				cos(radians), 0, sin(radians), 0,
				0, 1, 0, 0,
				-sin(radians), 0, cos(radians), 0,
				0, 0, 0, 1
			)
		);
	}

	// Rotate matrix relative to the OX axis
	inline glm::mat4 RotateOX(float radians)
	{
		return glm::transpose(
			glm::mat4(
				1, 0, 0, 0,
				0, cos(radians), -sin(radians), 0,
				0, sin(radians), cos(radians), 0,
				0, 0, 0, 1
			)
		);
	}
}
