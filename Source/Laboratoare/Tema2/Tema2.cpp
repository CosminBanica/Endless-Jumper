#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

pair<Type, glm::vec3> Tema2::platformTypeGenerator(int randomPlatform) {
	Type platType;
	glm::vec3 platColor;

	if (randomPlatform == 0) {
		platType = blue;
		platColor = platformBlueColor;
	}
	if (randomPlatform == 1 || randomPlatform == 8 || randomPlatform == 15) {
		platType = red;
		platColor = platformRedColor;
	}
	if (randomPlatform == 2 || randomPlatform == 9 || randomPlatform == 11 || randomPlatform == 14 || randomPlatform == 19) {
		platType = yellow;
		platColor = platformYellowColor;
	}
	if (randomPlatform == 3 || randomPlatform == 10 || randomPlatform == 12 || randomPlatform == 16) {
		platType = orange;
		platColor = platformOrangeColor;
	}
	if (randomPlatform == 4 || randomPlatform == 5 || randomPlatform == 13 || randomPlatform == 17 || randomPlatform == 18 || randomPlatform == 7) {
		platType = green;
		platColor = platformGreenColor;
	}
	if (randomPlatform == 6) {
		platType = darkGreen;
		platColor = platformDarkGreenColor;
	}

	return make_pair(platType, platColor);
}

void Tema2::populatePlatforms(vector<Platform> &platforms, float platformsEnd, int platformSpacing) {
	int lastColumn = -1;

	platforms.push_back(Platform(glm::vec3(0, 0, -platformSpacing * 2), 1, platformBlueColor, blue, 2));
	//platforms.push_back(Platform(glm::vec3(0, 0, -platformSpacing * 2), 1, platformBlueColor, blue, 1));
	for (int i = platformSpacing * 4; i <= -platformsEnd; i += platformSpacing) {
		int columnToPick = std::rand() % 3;
		if (columnToPick == lastColumn) {
			columnToPick = (columnToPick + (std::rand() % 2 + 1)) % 3;
		}
		lastColumn = columnToPick;
		std::pair<int, int> column = columns[columnToPick];

		int platformLength = std::rand() % 150;
		float platformLengthScaling = 1.0f + (float)platformLength / 100;
		glm::vec3 platformLocation = glm::vec3(column.first + (platformWidth / 2), 0, -i);

		int randomPlatform = std::rand() % gameDifficulty;
		pair<Type, glm::vec3> platformType = platformTypeGenerator(randomPlatform);
		platforms.push_back(Platform(platformLocation, columnToPick, platformType.second, platformType.first, platformLengthScaling));
	}
}

void Tema2::Init()
{
	camera = new Laborator::Camera();
	camera->Set(glm::vec3(0, 2.7f, 3.5f), glm::vec3(0, 0.8f, 0), glm::vec3(0, 1, 0));
	cameraInitialPos = camera->GetTargetPosition();

	player = Player(glm::vec3(0, 0, 0));

	{
		glm::vec3 center = glm::vec3(0, 0, 0);
		Mesh* mesh = Player::CreateMesh("sphere", playerRadius, 100, glm::vec3(1, 0, 0));
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		glm::vec3 center = glm::vec3(0, 0, 0);
		Mesh* mesh = Platform::CreateMesh("platform", center, platformWidth, platformHeight, platformLength, glm::vec3(1, 0, 0));
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* powerBar = FuelInterface::CreateSquare("powerBar", glm::vec3(0, 0, 0), 2, 1);
		AddMeshToList(powerBar);
	}

	{
		Mesh* powerBarContur = FuelInterface::CreateSquare("powerBarContur", glm::vec3(0, 0, 0), 2, 0, 1, glm::vec3(1, 1, 1));
		AddMeshToList(powerBarContur);
	}

	{
		Mesh* powerBarSlider = FuelInterface::CreateSquare("powerBarSlider", glm::vec3(0, 0, 0), 2, 1, 1);
		AddMeshToList(powerBarSlider);
	}

	{
		Mesh* life = FuelInterface::CreateHeart("life", glm::vec3(0, 0, 0), 40, glm::vec3(0, 1, 0));
		AddMeshToList(life);
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader* shader = new Shader("ShaderLab7");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	//Light & material properties
	{
		lightPosition = glm::vec3(0, 2, -1);
		materialShininess = 100;
		materialKd = 0.5;
		materialKs = 0.5;
	}

	renderCameraTarget = true;
	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
	orthoMatrix = glm::ortho(left, right, bottom, top, close, away);
	//glEnable(GL_CULL_FACE);

	populatePlatforms(platforms, -100, 6);
}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	float playerPrevHeight = player.GetPosition().y;

	// Increase game difficulty
	if (minGameSpeed < 10.0f) {
		minGameSpeed += deltaTimeSeconds;
		if (gameSpeed < minGameSpeed) {
			gameSpeed = minGameSpeed;
		}
	}

	if (minGameSpeed >= 10.0f) {
		gameDifficulty = 20;
	}

	// Render fuel interface
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x + 0.1f, camera->GetTargetPosition().y + 6.4, camera->GetTargetPosition().z - 4));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5, 1, 1));
	RenderMesh(meshes["powerBar"], shaders["VertexColor"], orthoMatrix, modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x + 3.1f, camera->GetTargetPosition().y + 6.805, camera->GetTargetPosition().z - 3.25));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5/ playerMaxFuel * (player.GetFuel() - playerMaxFuel), 1, 1));
	RenderMesh(meshes["powerBarSlider"], shaders["VertexColor"], orthoMatrix, modelMatrix);
	
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x + 0.1f, camera->GetTargetPosition().y + 6.36, camera->GetTargetPosition().z - 4));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.51, 1.03, 1));
	RenderMesh(meshes["powerBarContur"], shaders["VertexColor"], orthoMatrix, modelMatrix);
	
	// Render lives interface
	if (lives > 0) {
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x + 9, camera->GetTargetPosition().y - 2, camera->GetTargetPosition().z - 6));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01, 0.02, 1));
		RenderMesh(meshes["life"], shaders["VertexColor"], orthoMatrix, modelMatrix);
	}
	if (lives > 1) {
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x + 8, camera->GetTargetPosition().y - 2, camera->GetTargetPosition().z - 6));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01, 0.02, 1));
		RenderMesh(meshes["life"], shaders["VertexColor"], orthoMatrix, modelMatrix);
	}

	// Render ground and check for collisions
	{
		columnsBelow = { false ,false, false };

		platformSpawnTimer += deltaTimeSeconds * gameSpeed;
		if (platformSpawnTimer > platformFreq) {
			platformSpawnTimer = 0;
			int columnToPick = std::rand() % 3;
			if (columnToPick == lastColumn) {
				columnToPick = (columnToPick + (std::rand() % 2 + 1)) % 3;
			}
			lastColumn = columnToPick;
			std::pair<int, int> column = columns[columnToPick];
			glm::vec3 platformLocation = glm::vec3(column.first + (platformWidth / 2), 0, platformSpawnPoint);

			int platformLength = std::rand() % 100;
			float platformLengthScaling = 1.0f + (float)platformLength / 100.0f;

			int randomPlatform = std::rand() % gameDifficulty;
			pair<Type, glm::vec3> platformType = platformTypeGenerator(randomPlatform);
			platforms.push_back(Platform(platformLocation, columnToPick, platformType.second, platformType.first, platformLengthScaling));
		}

		bool left = true;
		bool right = true;
		player.SetFalling(true);

		for (int i = 0; i < platforms.size(); ++i) {
			glm::mat4 modelMatrix = glm::mat4(1);

			// Only move platforms if player is moving
			if (!pausePlatforms) {
				platforms[i].MovePlatform(deltaTimeSeconds * gameSpeed);
			}
			modelMatrix = glm::translate(modelMatrix, platforms[i].GetPosition());
			modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, platforms[i].GetLengthScaling()));
			RenderSimpleMesh(meshes["platform"], shaders["ShaderLab7"], modelMatrix, platforms[i].GetColor(), noAbilityRender, 0);

			// Get rid of platforms that we can't see anymore
			if (platforms[i].GetPosition().z > 40) {
				platforms.erase(platforms.begin() + i);
			}

			float playerZ = (cameraInitialPos + player.GetPosition()).z;
			float playerX = (cameraInitialPos + player.GetPosition()).x;
			float playerY = (cameraInitialPos + player.GetPosition()).y;
			float platformEdgeLeft = platforms[i].GetPosition().x - platformWidth / 2;
			float platformEdgeRight = platforms[i].GetPosition().x + platformWidth / 2;
			float platformEdgeUp = platforms[i].GetPosition().y + platformHeight / 2;
			float platformEdgeDown = platforms[i].GetPosition().y - platformHeight / 2;

			// Player Collisions
			// Platform top check
			if ((playerZ < platforms[i].GetPosition().z + (platformLength / 2) * platforms[i].GetLengthScaling()) &&
				(playerZ > platforms[i].GetPosition().z - (platformLength / 2) * platforms[i].GetLengthScaling() - playerRadius)) {
				columnsBelow[platforms[i].GetColumn()] = true;
			}

			// Check if abilities need to be applied
			if ((playerZ < platforms[i].GetPosition().z + (platformLength / 2) * platforms[i].GetLengthScaling()) &&
				(playerZ > platforms[i].GetPosition().z - (platformLength / 2) * platforms[i].GetLengthScaling() - playerRadius) &&
				(playerX < platformEdgeRight + playerRadius && playerX > platformEdgeLeft - playerRadius) &&
				(playerY > platformEdgeUp - playerRadius && playerY < platformEdgeUp + playerRadius)) {
				platforms[i].SetColor(platformPurpleColor);
				player.SetFalling(false);
				
				Type platformType = platforms[i].GetType();
				if (!platforms[i].SpentAbility() && !gameOver) {
					if (platformType == red) {
						player.SetFuel(-1);
						lives = 0;
					}
					if (platformType == yellow) {
						player.SpendFuel(2);
					}
					if (platformType == orange) {
						player.SetRenderType(orangeAbilityRender);
						player.SetOrangeStuckTimer(3);
						gameSpeed = orangeAbilitySpeed;
					}
					if (platformType == green) {
						if (player.GetFuel() < 0) {
							player.SetFuel(0);
							pausePlatforms = false;
						}
						player.SpendFuel(-5);
					}
					if (platformType == darkGreen) {
						if (lives < maxLives) {
							lives++;
						}
					}
				}
				platforms[i].UseAbility();
			}

			// Platform front check
			if (playerX < platformEdgeRight + playerRadius && playerX > platformEdgeLeft - playerRadius &&
				playerY > platformEdgeDown - playerRadius && playerY < platformEdgeUp &&
				playerZ - playerRadius > platforms[i].GetPosition().z + (platformLength / 2) * platforms[i].GetLengthScaling() &&
				playerZ - playerRadius < platforms[i].GetPosition().z + (platformLength / 2) * platforms[i].GetLengthScaling() + 0.1f) {
				pausePlatforms = true;
			}
			else if (playerX < platformEdgeRight + playerRadius && playerX > platformEdgeLeft - playerRadius &&
				playerY > platformEdgeDown - playerRadius && playerY < platformEdgeUp + playerRadius / 1.2f &&
				playerZ > platforms[i].GetPosition().z - (platformLength / 2) * platforms[i].GetLengthScaling() &&
				playerZ < platforms[i].GetPosition().z + (platformLength / 2) * platforms[i].GetLengthScaling()) {
				player.RaiseAltitude(0.01f);
			}

			// Platform side check
			if (playerX > platformEdgeLeft - playerRadius - 0.1f && playerX < platformEdgeLeft - playerRadius &&
				playerY > platformEdgeDown - playerRadius * 1.2f && playerY < platformEdgeUp + playerRadius / 1.2f &&
				playerZ > platforms[i].GetPosition().z - (platformLength / 2) * platforms[i].GetLengthScaling() - playerRadius &&
				playerZ < platforms[i].GetPosition().z + (platformLength / 2) * platforms[i].GetLengthScaling() + playerRadius) {
				right = false;
			}
			if (playerX > platformEdgeLeft + playerRadius && playerX < platformEdgeLeft + playerRadius + 0.1f &&
				playerY > platformEdgeDown - playerRadius * 1.2f && playerY < platformEdgeUp + playerRadius / 1.2f &&
				playerZ > platforms[i].GetPosition().z - (platformLength / 2) * platforms[i].GetLengthScaling() - playerRadius &&
				playerZ < platforms[i].GetPosition().z + (platformLength / 2) * platforms[i].GetLengthScaling() + playerRadius) {
				left = false;
			}
			
		}

		if (left) {
			player.AllowMoveLeft();
		}
		else {
			player.DisallowMoveLeft();
		}
		if (right) {
			player.AllowMoveRight();
		}
		else {
			player.DisallowMoveRight();
		}
	}

	// Render player
	if (renderCameraTarget)
	{
		// Manage fuel consumption
		if (player.GetFuel() > playerMaxFuel) {
			player.SetFuel(playerMaxFuel);
		}
		fuelConsumptionSpeed += deltaTimeSeconds * 0.03;
		if (fuelConsumptionSpeed > maxFuelConsumptionSpeed) {
			fuelConsumptionSpeed = maxFuelConsumptionSpeed;
		}
		if (!pausePlatforms) {
			if (player.GetOrangeTimer() > 0) {
				player.LowerOrangeStuckTimer(deltaTimeSeconds * 1);
			}
			player.SpendFuel(deltaTimeSeconds * fuelConsumptionSpeed);
			if (player.GetFuel() < 0) {
				pausePlatforms = true;
			}
		}

		// Check which column player is on for collision
		float playerX = (player.GetPosition() + cameraInitialPos).x;
		float playerY = (player.GetPosition() + cameraInitialPos).y;
		if (!gameOver) {
			if (playerX > columns[0].first && playerX <= columns[0].second) {
				if (!columnsBelow[0] || pausePlatforms) {
					player.Fall(deltaTimeSeconds);
				}
				else if (playerY - playerRadius > platformHeight / 2) {
					player.Fall(deltaTimeSeconds);
				}
				else {
					player.AllowJump();
				}
			}
			else if (playerX > columns[1].first && playerX <= columns[1].second) {
				if (!columnsBelow[1] || pausePlatforms) {
					player.Fall(deltaTimeSeconds);
				}
				else if (playerY - playerRadius > platformHeight / 2) {
					player.Fall(deltaTimeSeconds);
				}
				else {
					player.AllowJump();
				}
			}
			else if (playerX > columns[2].first && playerX <= columns[2].second) {
				if (!columnsBelow[2] || pausePlatforms) {
					player.Fall(deltaTimeSeconds);
				}
				else if (playerY - playerRadius > platformHeight / 2) {
					player.Fall(deltaTimeSeconds);
				}
				else {
					player.AllowJump();
				}
			}
		}

		// Manage game over
		if (playerY < -platformHeight / 2 - playerRadius) {
			if (lives > 0) {
				lives--;
				cout << "You have " << lives << " lives left!!!\n";
				player.SetAltitude(3);
				player.SetFuel(playerMaxFuel);
				player.SetOrangeStuckTimer(-1);
				pausePlatforms = false;
			}
			else {
				pausePlatforms = true;
				player.SetAltitude(0.5f);
				player.SetFuel(-10);
				gameOver = true;
				player.SetOrangeStuckTimer(-1);
				firstPerson = false;
				camera->TranslateForward(-2.2);
				cout << "GAME OVER!!!\n";
			}
			
		}
		playerPrevHeight = -playerPrevHeight + player.GetPosition().y;

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, cameraInitialPos);
		modelMatrix = glm::translate(modelMatrix, player.GetPosition());
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f));
		if (player.GetOrangeTimer() <= 0) {
			player.SetRenderType(noAbilityRender);
		}
		if (gameOver) {
			player.SetRenderType(gameOverRender);
		}
		if (player.IsFalling() && !firstPerson) {
			RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab7"], modelMatrix, glm::vec3(1, 0.7, 0.7), player.GetRenderType(), 0);
		}
		else if (!gameOver && !firstPerson) {
			RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab7"], modelMatrix, glm::vec3(1, 0.2, 0.7), player.GetRenderType(), 0);
		}

		camera->TranslateUpword(playerPrevHeight);
	}
}

void Tema2::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3& color, int renderType, bool projType)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	// Set light position uniform
	GLint loc_light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3fv(loc_light_position, 1, glm::value_ptr(lightPosition));

	// Set eye position (camera position) uniform
	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	GLint loc_eye_ppsition = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(loc_eye_ppsition, 1, glm::value_ptr(eyePosition));

	// Set material property uniforms (shininess, kd, ks, object color) 
	GLint material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	GLint material_kd = glGetUniformLocation(shader->program, "material_kd");  // diffuse light
	glUniform1f(material_kd, materialKd);

	GLint material_ks = glGetUniformLocation(shader->program, "material_ks");  // specular light
	glUniform1f(material_ks, materialKs);

	GLint time = glGetUniformLocation(shader->GetProgramID(), "time");
	glUniform1f(time, (GLfloat)Engine::GetElapsedTime());

	GLint material_object_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(material_object_color, 1, glm::value_ptr(color));
	

	GLint deformate = glGetUniformLocation(shader->program, "deformation"); 
	glUniform1i(deformate, renderType);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	if (projType == 0) {
		glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	}
	else {
		glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(orthoMatrix));
	}
	
	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, glm::mat4 projMatrix, const glm::mat4& modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	float speed = 2;

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		// Control player using W, A, S, D
		if (window->KeyHold(GLFW_KEY_W) && player.GetOrangeTimer() <= 0) {
			gameSpeed += 10.0f * deltaTime;

			if (gameSpeed > maxGameSpeed) {
				gameSpeed = maxGameSpeed;
			}
		}
		if (window->KeyHold(GLFW_KEY_S) && player.GetOrangeTimer() <= 0) {
			gameSpeed -= 10.0f * deltaTime;

			if (gameSpeed < minGameSpeed) {
				gameSpeed = minGameSpeed;
			}
		}
		if (window->KeyHold(GLFW_KEY_D)) {
			if (player.CanMoveRight()) {
				if (!firstPerson) {
					player.SteerPlayer(4.0f * deltaTime);
				}
				else {
					player.SteerPlayer(4.0f * deltaTime);
					camera->TranslateRight(4.0f * deltaTime);
				}
			}
		}
		if (window->KeyHold(GLFW_KEY_A)) {
			if (player.CanMoveLeft()) {
				if (!firstPerson) {
					player.SteerPlayer(-4.0f * deltaTime);
				}
				else {
					player.SteerPlayer(-4.0f * deltaTime);
					camera->TranslateRight(-4.0f * deltaTime);
				}
			}
		}
	}
}

void Tema2::OnKeyPress(int key, int mods)
{
	// Jump
	if (key == GLFW_KEY_SPACE)
	{
		if (player.CanJump()) {
			player.Jump();
			camera->TranslateUpword(0.1f);
		}
	}

	// Switch view mode
	if (key == GLFW_KEY_LEFT_CONTROL) {
		if (firstPerson) {
			firstPerson = false;
			player.SetAltitude(0);
			camera->Set(glm::vec3(0, 2.7f, 3.5f), glm::vec3(0, 0.8f, 0), glm::vec3(0, 1, 0));
		}
		else {
			firstPerson = true;
			player.SetAltitude(0);
			camera->Set(glm::vec3(0, 2.7f, 3.5f), glm::vec3(0, 0.8f, 0), glm::vec3(0, 1, 0));
			camera->TranslateRight(player.GetPosition().x);
			camera->TranslateForward(2.2);
		}
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}