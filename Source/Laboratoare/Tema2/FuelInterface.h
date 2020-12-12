#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace FuelInterface
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, bool fill = false, bool colorFlag = false, glm::vec3 color = glm::vec3(0, 0, 0));
	Mesh* CreateHeart(std::string name, glm::vec3 center, float size, glm::vec3 color);
}