//Created 10/18/2023
//Purpose: No idea
//Dodge rocks while flying through space.
#include "raylib.h"
#include <stdlib.h>
#include <iostream>
#include <cstdlib>

#if defined(_WIN32)
const int os = 1;
#else
const int os = 0;
#endif

//rock
typedef struct {
	Vector2 position;
	Vector2 speed;
	float radius;
	Texture2D color;
	bool active;
	Rectangle bounds;
} Rock;

#define MAX_ROCKS 50

void Delay(float seconds)
{
	double startTime = GetTime();

	while (GetTime() - startTime < seconds) {}
}

int WinMain()
{
	const int width = 800;
	const int height = 600;
	InitWindow(width, height, "Space Race!");

	//Color
	Color backgroundColor = ColorFromNormalized(Vector4{ 28.0f / 255.0f, 28.0f / 255.0f, 28.0f / 255.0f, 1.0f });

	//Player
	Texture2D playerTexture[2];
	playerTexture[0] = LoadTexture("data/rocket1.png");
	playerTexture[1] = LoadTexture("data/rocket2.png");

	//Rock
	Texture2D rockTexture[2];
	rockTexture[0] = LoadTexture("data/rock1.png");
	rockTexture[1] = LoadTexture("data/rock2.png");

	Rectangle player = { 100, 100, playerTexture[0].width, playerTexture[0].height };
	Rectangle rock = { 200, 100, rockTexture[0].width, rockTexture[0].height};
	float scale = 2.0f; // Scale by 2x
	//Timer
	float currentTime = 0.0f;
	float switchTime = 0.5f;
	bool useTexture1 = true;

	bool stop = false;

	SetTargetFPS(60);

	//Creating the rocks
	Rock rocks[MAX_ROCKS];
	int rockCount = 0;

	while (!WindowShouldClose() && !stop)
	{
		// Update player's bounding box
		player.width = playerTexture[0].width * scale;
		player.height = playerTexture[0].height * scale;

		//Switch player texture
		currentTime += GetFrameTime();

		//I can do this
		\
		//with no error
		//why
		if (currentTime >= switchTime)
		{
			useTexture1 = !useTexture1;
			currentTime = 0.0f;
		}

		if (rockCount < MAX_ROCKS && GetRandomValue(0, 100) < 2) // Adjust the chance of spawning
		{
			rocks[rockCount].position.y = GetRandomValue(0, width - 50);
			rocks[rockCount].position.x = 600;
			rocks[rockCount].speed.y = 0;
			rocks[rockCount].speed.x = -70;
			rocks[rockCount].radius = 40;
			rocks[rockCount].color = rockTexture[0];
			rocks[rockCount].active = true;
			rockCount++;
		}

		//Moving the player
		\
		if (IsKeyDown(KEY_RIGHT)) player.x += 2.0f;
		if (IsKeyDown(KEY_LEFT)) player.x -= 2.0f;
		if (IsKeyDown(KEY_UP)) player.y -= 2.0f;
		if (IsKeyDown(KEY_DOWN)) player.y += 2.0f;

		// Update asteroid positions
		for (int i = 0; i < rockCount; i++) {
			if (rocks[i].active) {
				rocks[i].position.x += rocks[i].speed.x * GetFrameTime();

				// Update rock's bounding box
				rocks[i].bounds = Rectangle{ rocks[i].position.x, rocks[i].position.y, (float) rockTexture[0].width, (float) rockTexture[0].height };



				// Remove rocks that are off-screen
				if (rocks[i].position.x < -rockTexture[0].width) {
					rocks[i].active = false;
				}
			}
		}

		BeginDrawing();
		ClearBackground(backgroundColor);

		if (IsKeyPressed(KEY_APOSTROPHE))
		{
			SetTargetFPS(0);
		}

		//Draw the player
		DrawTextureEx(playerTexture[useTexture1 ? 0 : 1], Vector2{ player.x, player.y }, 0.0f, scale, WHITE);
		//Draw the rocks
		//DrawTextureEx(rockTexture[useTexture1 ? 0 : 1], Vector2{ rock.x, rock.y }, 0.0f, scale, WHITE);

		for (int i = 0; i < rockCount; i++)
		{
			if (rocks[i].active)
			{
				//DrawTextureEx(rockTexture[useTexture1 ? 0 : 1], Vector2{ player.x, player.y }, 0.0f, scale, WHITE);
				DrawTextureEx(rocks[i].color, rocks[i].position, 0.0f, 3.0, WHITE);
			}
			if (CheckCollisionRecs(player, rocks[i].bounds))
			{
				// Handle collision here (e.g., end the game, subtract health, etc.)
				// You can also mark the rock as inactive if it should be removed upon collision.
				if (os)
				{
					system("data\\stop.vbs");
					return 1;
				}
				stop = true;
			}
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
//lua is gay because it counts arrays from 1 not 0