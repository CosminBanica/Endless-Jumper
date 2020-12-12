#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>

#include "LabCamera.h"
#include "Transform3D.h"
#include "Platform.h"
#include "Player.h"
#include "FuelInterface.h"
#include "Transform2D.h"

#define gameOverRender 2
#define orangeAbilityRender 1
#define noAbilityRender 0

class Tema2 : public SimpleScene
{
public:
	Tema2();
	~Tema2();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3& color, int renderType, bool projType);
	void RenderMesh(Mesh* mesh, Shader* shader, glm::mat4 projMatrix, const glm::mat4& modelMatrix);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;
	void populatePlatforms(std::vector<Platform> &platforms, float platformsEnd, int platformSpacing);
	std::pair<Type, glm::vec3> platformTypeGenerator(int randomPlatform);

	glm::vec3 lightPosition;
	unsigned int materialShininess;
	float materialKd;
	float materialKs;
	Laborator::Camera* camera;
	glm::mat4 projectionMatrix;
	glm::mat4 orthoMatrix;
	bool renderCameraTarget;
	std::vector<Platform> platforms;

	//Platform columns: -3:-1, -1:1, 1:3
	float platformWidth = 2;
	std::vector<std::pair<int, int>> columns =
	{ std::make_pair(-3 * platformWidth / 2, -platformWidth / 2),
	  std::make_pair(-platformWidth / 2, platformWidth / 2),
	  std::make_pair(platformWidth / 2, 3 * platformWidth / 2)
	};
	std::vector<bool> columnsBelow =
	{
		false, true, false
	};

	glm::vec3 platformBlueColor = glm::vec3(0, 0, 1);
	glm::vec3 platformPurpleColor = glm::vec3(0.5, 0, 0.7);
	glm::vec3 platformRedColor = glm::vec3(1, 0, 0);
	glm::vec3 platformYellowColor = glm::vec3(1, 1, 0);
	glm::vec3 platformOrangeColor = glm::vec3(1, 0.5, 0);
	glm::vec3 platformGreenColor = glm::vec3(0, 1, 0);
	glm::vec3 platformDarkGreenColor = glm::vec3(0.02, 0.24, 0.09);

	float platformFreq = 5;
	float platformSpawnTimer = 0;
	float platformSpawnPoint = -100;
	float platformLength = 5;
	float platformHeight = 0.5f;
	int lastColumn = 1;

	float initialPlatformZ = 0;

	float minGameSpeed = 2.5f;
	float maxGameSpeed = 25.0f;
	float orangeAbilitySpeed = 20.0f;
	float gameSpeed = 5.0f;
	bool pausePlatforms = false;

	Player player;
	float playerRadius = 0.2f;

	float right = 10.f;
	float left = 0.f;
	float bottom = 0.f;
	float top = 10.f;
	float close = 0.f;
	float away = 200.f;

	float playerMaxFuel = 15;

	bool gameOver = false;
	bool firstPerson = false;
	glm::vec3 cameraInitialPos;

	int lives = 2;
	int maxLives = 2;
	int gameDifficulty = 10;
	float fuelConsumptionSpeed = 1;
	float maxFuelConsumptionSpeed = 1.75f;
};