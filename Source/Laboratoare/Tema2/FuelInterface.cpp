#pragma once

#include "FuelInterface.h"

#include <Core/Engine.h>

Mesh* FuelInterface::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, bool fill, bool colorFlag, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;
	std::vector<VertexFormat> vertices;

	if (colorFlag) {
		vertices =
		{
			VertexFormat(corner, color),
			VertexFormat(corner + glm::vec3(length, 0, 0), color),
			VertexFormat(corner + glm::vec3(length, length, 0), color),
			VertexFormat(corner + glm::vec3(0, length, 0), color)
		};
	}
	else {
		vertices =
		{
			VertexFormat(corner, glm::vec3(1, 1, 0)),
			VertexFormat(corner + glm::vec3(length, 0, 0), glm::vec3(1, 0, 0)),
			VertexFormat(corner + glm::vec3(length, length, 0), glm::vec3(1, 0, 0)),
			VertexFormat(corner + glm::vec3(0, length, 0), glm::vec3(1, 1, 0))
		};
	}

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* FuelInterface::CreateHeart(std::string name, glm::vec3 center, float size, glm::vec3 color)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	Mesh* heart = new Mesh(name);

	vertices.push_back(VertexFormat(center + glm::vec3(-size, 0, 0), color));
	indices.push_back(0);
	vertices.push_back(VertexFormat(center + glm::vec3(0, -size, 0), color));
	indices.push_back(1);
	vertices.push_back(VertexFormat(center + glm::vec3(size, 0, 0), color));
	indices.push_back(2);

	const float PI = 3.1415926f;
	unsigned short indice = 3;
	float increment = 2.0f * PI / 100;

	glm::vec3 start1 = center + glm::vec3(-size / 2.0f + 2.5, 0, 0);
	glm::vec3 start2 = center + glm::vec3(size / 2.0f - 2.5, 0, 0);

	for (float currAngle = 0.0f; currAngle <= 1.0f * PI; currAngle += increment)
	{
		vertices.push_back(VertexFormat(start1, color));
		indices.push_back(indice++);
		vertices.push_back(VertexFormat(start1 + glm::vec3((size / 2.0f + 2.5) * cos(currAngle), (size / 2.0f) * sin(currAngle) * 1.3, 0), color));
		indices.push_back(indice++);
		vertices.push_back(VertexFormat(start1 + glm::vec3((size / 2.0f + 2.5) * cos(currAngle + increment), (size / 2.0f) * sin(currAngle + increment) * 1.3, 0), color));
		indices.push_back(indice++);
	}

	for (float currAngle = 0.0f; currAngle <= 1.0f * PI; currAngle += increment)
	{
		vertices.push_back(VertexFormat(start2, color));
		indices.push_back(indice++);
		vertices.push_back(VertexFormat(start2 + glm::vec3((size / 2.0f + 2.5) * cos(currAngle), (size / 2.0f) * sin(currAngle) * 1.3, 0), color));
		indices.push_back(indice++);
		vertices.push_back(VertexFormat(start2 + glm::vec3((size / 2.0f + 2.5) * cos(currAngle + increment), (size / 2.0f) * sin(currAngle + increment) * 1.3, 0), color));
		indices.push_back(indice++);
	}

	heart->SetDrawMode(GL_TRIANGLES);
	heart->InitFromData(vertices, indices);
	return heart;
}