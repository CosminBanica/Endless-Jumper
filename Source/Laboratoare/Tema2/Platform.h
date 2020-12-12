#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>

enum Type { blue, red, yellow, orange, green, darkGreen};

class Platform
{
public:
	Platform(glm::vec3 centre, int column, glm::vec3 color, Type type, float lengthScaling);
	~Platform();
	
	static Mesh* CreateMesh(std::string name, glm::vec3 centre, float lengthX, float lengthY, float lengthZ, glm::vec3 color);
	glm::vec3 GetPosition();
	void MovePlatform(float deltaZ);
	int GetColumn();
	void SetColor(glm::vec3 newColor);
	glm::vec3 GetColor();
	Type GetType();
	void UseAbility();
	bool SpentAbility();
	float GetLengthScaling();

private:
	glm::vec3 position;
	int column;
	glm::vec3 color;
	Type type;
	bool spentAbility = false;
	float lengthScaling;


protected:

};