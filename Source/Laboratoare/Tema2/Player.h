#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>

class Player
{
public:
	Player();
	Player(glm::vec3 centre);
	~Player();

	static Mesh* CreateMesh(std::string name, float radius, int detail, glm::vec3 color);
	glm::vec3 GetPosition();
	void SteerPlayer(float deltaX);
	void Jump();
	void Fall(float delta);
	float GetGravity();
	bool CanJump();
	void AllowJump();
	void DisallowJump();
	void SetAltitude(float altitude);
	void RaiseAltitude(float altitude);
	bool CanMoveLeft();
	void AllowMoveLeft();
	void DisallowMoveLeft();
	bool CanMoveRight();
	void AllowMoveRight();
	void DisallowMoveRight();
	void SetRenderType(int newRenderType);
	int GetRenderType();
	bool IsFalling();
	void SetFalling(bool fallingState);
	void SpendFuel(float fuelspent);
	float GetFuel();
	void SetFuel(float newFuelAmount);
	void SetOrangeStuckTimer(float timer);
	void LowerOrangeStuckTimer(float lowerAmount);
	float GetOrangeTimer();

private:
	glm::vec3 position;
	float gravity = -1.0f;
	float maxGravity = 4.0f;
	float fuel = 15;
	int renderType = 0;

	bool canJump = false;
	bool canMoveLeft = true;
	bool canMoveRight = true;
	bool falling = true;
	float orangeStuckTimer = 0;

protected:

};