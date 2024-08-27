#define GLM_ENABLE_EXPERIMENTAL
#include "gameLayer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "platformInput.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include "imfilebrowser.h"
#include <gl2d/gl2d.h>
#include <platformTools.h>
#include <tiledRenderer.h>


struct GameplayData
{
	glm::vec2 playerPos = { 100,100 };
};

GameplayData data;

gl2d::Renderer2D renderer;

constexpr int BACKKGROUNDS = 3;

gl2d::Texture spaceShipTexture; //Goal : How to make the planets cover the ship instead of we going above it
								// idea in mind : Make the spaceShipTexture as background texture and make it as the [2] array and make
								// the planets as the [3] array
								// or make another spaceshiptexture but the texture is just another planet and make sure
								// that the spaceship texture that moves is on array [0]

gl2d::Texture backgroundTexture[BACKKGROUNDS];
TiledRenderer tiledRenderer[BACKKGROUNDS];

bool initGame()
{
	//initializing stuff for the renderer
	gl2d::init();
	renderer.create();

	
	spaceShipTexture.loadFromFile(RESOURCES_PATH"spaceShip/ships/ship.png", true);
	
	backgroundTexture[0].loadFromFile(RESOURCES_PATH"background1.png", true);
	backgroundTexture[1].loadFromFile(RESOURCES_PATH"background2.png", true);
	backgroundTexture[2].loadFromFile(RESOURCES_PATH"background3.png", true);
	
	tiledRenderer[0].texture = backgroundTexture[0];
	tiledRenderer[1].texture = backgroundTexture[1];
	tiledRenderer[2].texture = backgroundTexture[2];

	tiledRenderer[0].paralaxStrength = 0;
	tiledRenderer[1].paralaxStrength = 0.1;
	tiledRenderer[2].paralaxStrength = 0.7;


	return true;
}



bool gameLogic(float deltaTime)
{

#pragma region init stuff
	int w = 0; int h = 0;
	w = platform::getFrameBufferSizeX(); //window w
	h = platform::getFrameBufferSizeY(); //window h
	
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT); //clear screen

	renderer.updateWindowMetrics(w, h);
#pragma endregion

#pragma region follow

	renderer.currentCamera.follow(data.playerPos, deltaTime * 150, 10, 200, w, h);

#pragma endregion

#pragma region movement

	glm::vec2 move = {};

	if (
		platform::isButtonHeld(platform::Button::W) ||
		platform::isButtonHeld(platform::Button::Up)
		)
	{
		move.y = -1;
	}
	if (
		platform::isButtonHeld(platform::Button::S) ||
		platform::isButtonHeld(platform::Button::Down)
		)
	{
		move.y = 1;
	}
	if (
		platform::isButtonHeld(platform::Button::A) ||
		platform::isButtonHeld(platform::Button::Left)
		)
	{
		move.x = -1;
	}
	if (
		platform::isButtonHeld(platform::Button::D) ||
		platform::isButtonHeld(platform::Button::Right)
		)
	{
		move.x = 1;
	}
	
	if (move.x != 0 || move.y != 0) 
	{
		move = glm::normalize(move);
		move *= deltaTime* 300; //300 pixels per second
		data.playerPos += move;
	}
	
#pragma endregion

#pragma region render background

	renderer.currentCamera.zoom = 0.7;

	for (int i = 0; i < BACKKGROUNDS; i++)
	{
		tiledRenderer[i].render(renderer);
	}
	

#pragma endregion

	//renderer.currentCamera.follow(data.playerPos, deltaTime * 150, 10, 200, w, h);

#pragma region mouse pos

	glm::vec2 mousePos = platform::getRelMousePosition();
	glm::vec2 screenCenter(w / 2.f, h / 2.f);

	glm::vec2 mouseDirection = mousePos - screenCenter;

	if (glm::length(mouseDirection) == 0.f)
	{
		mouseDirection = { 1,0 };
	}
	else
	{
		mouseDirection = normalize(mouseDirection);
	}

	float spaceShipAngle = atan2(mouseDirection.y, -mouseDirection.x);

#pragma endregion


#pragma region render ship

	renderer.renderRectangle({ data.playerPos, 200, 200 }, spaceShipTexture,
		Colors_White, {}, glm::degrees(spaceShipAngle) + 90.f);

#pragma endregion

	//renderer.renderRectangle({data.playerPos, 100, 100}, spaceShipTexture);



	renderer.flush();


	//ImGui::ShowDemoWindow();


	return true;
#pragma endregion

}

//This function might not be be called if the program is forced closed
void closeGame()
{



}
