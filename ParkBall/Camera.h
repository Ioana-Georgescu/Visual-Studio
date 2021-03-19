#pragma once
#include <include/glm.h>
#include <include/math.h>

namespace ParkBallAux
{
	class Camera
	{
		public:
			Camera()
			{
				position = glm::vec3(0, 0, 0);
				forward  = glm::vec3(0, 0, -1);
				up		 = glm::vec3(0, 1, 0);
				right	 = glm::vec3(1, 0, 0);
				distanceToTargetY = 1;
				distanceToTargetZ = 3;
			}

			Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
			{
				Set(position, center, up);
			}

			~Camera()
			{ }

			// Update camera
			void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
			{
				this->position = position;
				forward = glm::normalize(center-position);
				right	= glm::cross(forward, up);
				this->up = glm::cross(right,forward);
			}

			void MoveForward(float distance)
			{
				glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
				this->position += glm::normalize(dir) * distance;
			}

			void TranslateForward(float distance)
			{
				this->position += glm::normalize(forward) * distance;
			}

			void TranslateUpword(float distance)
			{
				this->position += glm::normalize(up) * distance;
			}

			void TranslateRight(float distance)
			{
				this->position += glm::normalize(right) * distance;
			}

			glm::mat4 GetViewMatrix()
			{
				// Returns the View Matrix
				return glm::lookAt(position, position + forward, up);
			}

			glm::vec3 GetTargetPosition()
			{
				return position + forward * distanceToTargetZ - up * distanceToTargetY;
			}

		public:
			float distanceToTargetY;
			float distanceToTargetZ;
			glm::vec3 position;
			glm::vec3 forward;
			glm::vec3 right;
			glm::vec3 up;
		};
}