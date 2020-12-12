#pragma once

#include "Player.h"

Player::Player() {
}

Player::Player(glm::vec3 centre) {
	position = centre;
}

Player::~Player() {

}

Mesh* Player::CreateMesh(std::string name, float radius, int detail, glm::vec3 color) {
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

    float x, y, z, xy;                              // vertex position

    int sectorCount = detail;
    int stackCount = detail;
    const float PI = 3.1415926f;

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back(VertexFormat(glm::vec3(x, y, z), glm::vec3(1, 0, 0)));
        }
    }

    int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (stackCount - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

	Mesh* platform = new Mesh(name);
	platform->SetDrawMode(GL_TRIANGLES);
	platform->InitFromData(vertices, indices);

	return platform;
}



glm::vec3 Player::GetPosition() {
	return position;
}

void Player::SteerPlayer(float deltaX) {
    position += glm::vec3(deltaX, 0, 0);
}

void Player::Jump()
{
    gravity = -2.0f;
    position += glm::vec3(0, 0.1f, 0);
    DisallowJump();
}

void Player::Fall(float delta) {
    position += glm::vec3(0, -delta * gravity, 0);
    gravity += delta * 2.0f;

    if (gravity > maxGravity) {
        gravity = maxGravity;
    }
}

float Player::GetGravity() {
    return gravity;
}

bool Player::CanJump() {
    return canJump;
}

void Player::AllowJump() {
    canJump = true;
}

void Player::DisallowJump() {
    canJump = false;
}

void Player::SetAltitude(float altitude) {
    position = glm::vec3(position.x, altitude, position.z);
}

void Player::RaiseAltitude(float altitude) {
    position = glm::vec3(position.x, position.y + altitude, position.z);
}

bool Player::CanMoveLeft()
{
    return canMoveLeft;
}

void Player::AllowMoveLeft()
{
    canMoveLeft = true;
}

void Player::DisallowMoveLeft()
{
    canMoveLeft = false;
}

bool Player::CanMoveRight()
{
    return canMoveRight;
}

void Player::AllowMoveRight()
{
    canMoveRight = true;
}

void Player::DisallowMoveRight()
{
    canMoveRight = false;
}

void Player::SetRenderType(int newRenderType)
{
    renderType = newRenderType;
}

int Player::GetRenderType()
{
    return renderType;
}

bool Player::IsFalling()
{
    return falling;
}

void Player::SetFalling(bool fallingState)
{
    falling = fallingState;
}

void Player::SpendFuel(float fuelspent)
{
    fuel -= fuelspent;
}

float Player::GetFuel()
{
    return fuel;
}

void Player::SetFuel(float newFuelAmount)
{
    fuel = newFuelAmount;
}

void Player::SetOrangeStuckTimer(float timer)
{
    orangeStuckTimer = timer;
}

void Player::LowerOrangeStuckTimer(float lowerAmount)
{
    orangeStuckTimer -= lowerAmount;
}

float Player::GetOrangeTimer()
{
    return orangeStuckTimer;
}

