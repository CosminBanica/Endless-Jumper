#pragma once

#include "Platform.h"
#include <iostream>

Platform::Platform(glm::vec3 centre, int column, glm::vec3 color, Type type, float length) {
	position = centre;
	this->column = column;
	this->color = color;
	this->type = type;
	this->lengthScaling = length;
}

Platform::~Platform() {

}

Mesh* Platform::CreateMesh(std::string name, glm::vec3 centre, float lengthX, float lengthY, float lengthZ, glm::vec3 color) {
	std::vector<VertexFormat> vertices;

	vertices =
	{
		VertexFormat(centre + glm::vec3(lengthX / 2, -lengthY / 2, lengthZ / 2), color),	// fata dreapta jos
		VertexFormat(centre + glm::vec3(-lengthX / 2, -lengthY / 2, lengthZ / 2), color),	// fata stanga jos
		VertexFormat(centre + glm::vec3(lengthX / 2, lengthY / 2, lengthZ / 2), color),		// fata dreapta sus
		VertexFormat(centre + glm::vec3(-lengthX / 2, lengthY / 2, lengthZ / 2), color),	// fata stanga sus
		VertexFormat(centre + glm::vec3(lengthX / 2, -lengthY / 2, -lengthZ / 2), color),	// spate dreapta jos
		VertexFormat(centre + glm::vec3(-lengthX / 2, -lengthY / 2, -lengthZ / 2), color),	// spate stanga jos
		VertexFormat(centre + glm::vec3(lengthX / 2, lengthY / 2, -lengthZ / 2), color),	// spate dreapta sus
		VertexFormat(centre + glm::vec3(-lengthX / 2, lengthY / 2, -lengthZ / 2), color)	// spate stanga sus
	};

	std::vector<unsigned short> indices = 
	  { 0, 2, 1, 
		1, 2, 3,
		1, 7, 5,
		1, 3, 7,
		0, 6, 2,
		0, 4, 6,
		6, 7, 3,
		6, 3, 2,
		5, 7, 6,
		5, 6, 4,
		5, 0, 1,
		5, 4, 0};

	Mesh* platform = new Mesh(name);
	platform->SetDrawMode(GL_TRIANGLES);
	platform->InitFromData(vertices, indices);

	return platform;
}

glm::vec3 Platform::GetPosition() {
	return position;
}

void Platform::MovePlatform(float deltaZ) {
	position = position + glm::vec3(0, 0, deltaZ);
}

int Platform::GetColumn() {
	return column;
}

void Platform::SetColor(glm::vec3 newColor) {
	color = newColor;
}

glm::vec3 Platform::GetColor() {
	return color;
}

Type Platform::GetType() {
	return type;
}

void Platform::UseAbility()
{
	spentAbility = true;
}

bool Platform::SpentAbility()
{
	return spentAbility;
}

float Platform::GetLengthScaling() {
	return lengthScaling;
}