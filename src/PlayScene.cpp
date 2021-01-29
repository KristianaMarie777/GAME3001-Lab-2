#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

#include <cstdlib>
#include <ctime>

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();

	if (m_pBat2->isEnabled())
	{

		m_pBat2->setDestination(m_pBat->getTransform()->position);
	}

	CollisionManager::AABBCheck(m_pBat, m_pObstacle);
	if (CollisionManager::lineRectCheck(m_pBat->getTransform()->position,m_pTarget->getTransform()->position, m_pObstacle->getTransform()->position, m_pObstacle->getWidth(), m_pObstacle->getHeight()))
	{
		m_pBat->Fleeing(true);
	}
	else
		m_pBat->Fleeing(false);
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		m_pTarget->setEnabled(true);
		m_pBat->setEnabled(true);
		m_pObstacle->setEnabled(false);
		m_pBat2->setEnabled(false);
		m_pTarget->getTransform()->position.y = rand() % 601;
		m_pTarget->getTransform()->position.x = rand() % 801;

		m_pBat->Seeking(true);
		m_pBat->Fleeing(false);
		m_pBat->setDestination(m_pTarget->getTransform()->position);


		
	}
	else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		float xValue = rand() % 80 - 40;
		float yValue = rand() % 80 - 40;
		m_pTarget->setEnabled(true);
		m_pBat->setEnabled(true);
		m_pObstacle->setEnabled(false);
		m_pBat2->setEnabled(false);
		m_pTarget->getTransform()->position.y = rand() % 601;
		m_pTarget->getTransform()->position.x = rand() % 801;

		if (xValue >= 0)
			m_pBat->getTransform()->position.x = m_pTarget->getTransform()->position.x + xValue + 30;
		else
			m_pBat->getTransform()->position.x = m_pTarget->getTransform()->position.x + xValue - 30;

		if (yValue >= 0)
			m_pBat->getTransform()->position.y = m_pTarget->getTransform()->position.y + yValue + 30;
		else
			m_pBat->getTransform()->position.y = m_pTarget->getTransform()->position.y + yValue - 30;

		m_pBat->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pBat->setRotation(0.0f);
		m_pBat->Fleeing(true);
		m_pBat->Seeking(false);
		m_pBat->setAvoidDestination(m_pTarget->getTransform()->position);

	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_3))
	{
		m_pTarget->setEnabled(true);
		m_pBat->setEnabled(true);
		m_pBat2->setEnabled(true);
		m_pObstacle->setEnabled(true);
		m_pTarget->getTransform()->position.y = rand() % 601;
		m_pTarget->getTransform()->position.x = rand() % 801;
		m_pBat->Seeking(true);
		m_pBat2->Seeking(true);
		m_pBat->Fleeing(false);
		m_pBat->setDestination(m_pTarget->getTransform()->position);
		m_pBat2->setDestination(m_pBat->getTransform()->position);

	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_4))
	{
		m_pTarget->setEnabled(true);
		m_pBat->setEnabled(true);
		m_pBat2->setEnabled(false);
		m_pObstacle->setEnabled(true);
		m_pTarget->getTransform()->position.y = rand() % 601;
		m_pTarget->getTransform()->position.x = rand() % 801;
		m_pBat->Seeking(true);
		m_pBat->Fleeing(false);
		m_pBat->setAvoidDestination(m_pObstacle->getTransform()->position);
		m_pBat->setDestination(m_pTarget->getTransform()->position);

	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W) && m_pTarget->getTransform()->position.y > 0)
	{
		m_pTarget->getTransform()->position.y -= 6.0f;
		m_pBat->setDestination(m_pTarget->getTransform()->position);
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_S) && m_pTarget->getTransform()->position.y < 600)
	{
		m_pTarget->getTransform()->position.y += 6.0f;
		m_pBat->setDestination(m_pTarget->getTransform()->position);
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A) && m_pTarget->getTransform()->position.x > 0)
	{
		m_pTarget->getTransform()->position.x -= 6.0f;
		m_pBat->setDestination(m_pTarget->getTransform()->position);
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D) && m_pTarget->getTransform()->position.x < 800)
	{
		m_pTarget->getTransform()->position.x += 6.0f;
		m_pBat->setDestination(m_pTarget->getTransform()->position);
	}
}

void PlayScene::start()
{

	const SDL_Color black = { 0, 0, 0, 255 };
	srand(time(NULL));

	// Set GUI Title
	m_guiTitle = "Play Scene";


	m_pInstructionLabel = new Label("Press 1 for Seeking", "Consolas", 20, black, glm::vec2(400.0f, 40.0f));;
	m_pInstructionLabel->setParent(this);
	addChild(m_pInstructionLabel);

	m_pInstructionLabel = new Label("Press 2 for Fleeing", "Consolas", 20, black, glm::vec2(400.0f, 70.0f));;
	m_pInstructionLabel->setParent(this);
	addChild(m_pInstructionLabel);

	m_pInstructionLabel = new Label("Press 3 for Arrival", "Consolas", 20, black, glm::vec2(400.0f, 100.0f));;
	m_pInstructionLabel->setParent(this);
	addChild(m_pInstructionLabel);

	m_pInstructionLabel = new Label("Press 4 for Avoidance", "Consolas", 20, black, glm::vec2(400.0f, 130.0f));;
	m_pInstructionLabel->setParent(this);
	addChild(m_pInstructionLabel);

	m_pInstructionLabel = new Label("Use WASD", "Consolas", 20, black, glm::vec2(100.0f, 40.0f));;
	m_pInstructionLabel->setParent(this);
	addChild(m_pInstructionLabel);

	m_pInstructionLabel = new Label("to move Target", "Consolas", 20, black, glm::vec2(100.0f, 70.0f));;
	m_pInstructionLabel->setParent(this);
	addChild(m_pInstructionLabel);


	m_pTarget = new Target();
	m_pTarget->getTransform()->position = glm::vec2(500.0f, 300.0f);
	m_pTarget->setEnabled(true);
	addChild(m_pTarget);

	m_pObstacle = new Obstacle();
	m_pObstacle->getTransform()->position = glm::vec2(300.0f, 300.0f);
	m_pObstacle->setEnabled(true);
	addChild(m_pObstacle);

	//declare bat properties OR instagating bat
	
	m_pBat = new Bat();
	m_pBat->getTransform()->position = glm::vec2(100.0f, 100.0f);
	m_pBat->setEnabled(false);
	m_pBat->setDestination(m_pTarget->getTransform()->position);
	addChild(m_pBat);

	m_pBat2 = new Bat();
	m_pBat2->getTransform()->position = glm::vec2(100.0f, 100.0f);
	m_pBat2->setEnabled(false);
	m_pBat2->setDestination(m_pTarget->getTransform()->position);
	addChild(m_pBat2);
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 - Lab 2", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static float batSpeed = 10.0f;
	if (ImGui::SliderFloat("MaxSpeed", &batSpeed, 0.0f, 100.0f))
	{
		m_pBat->setMaxSpeed(batSpeed);
		m_pBat2->setMaxSpeed(batSpeed);
	}

	static float acceleration_rate = 2.0f;
	if(ImGui::SliderFloat("Acceleration rate", &acceleration_rate, 0.0f, 50.0f))
	{
		m_pBat->setAccelerationRate(acceleration_rate);
		m_pBat2->setAccelerationRate(acceleration_rate);
	}

	static float angleRadians = 0.0f;
	if(ImGui::SliderAngle("Bat's Orientation", &angleRadians))
	{
		m_pBat->setRotation(angleRadians * Util::Rad2Deg);
		m_pBat2->setRotation(angleRadians * Util::Rad2Deg);
	}

	static float turn_rate = 5.0f;
	if (ImGui::SliderFloat("Turn Rate", &turn_rate,0.0f,20.0f))
	{
		m_pBat->setTurnRate(turn_rate);
		m_pBat2->setTurnRate(turn_rate);
	}
	
	if(ImGui::Button("START"))
	{
		m_pBat->setEnabled(true);
		m_pBat2->setEnabled(true);
	}

	ImGui::SameLine();

	if (ImGui::Button("RESET"))
	{
		m_pBat->getTransform()->position = glm::vec2(100.0f, 100.0f);
		m_pBat->setEnabled(false);
		m_pBat->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pBat->setRotation(0.0f);//set angle to 0 degrees

		m_pBat2->getTransform()->position = glm::vec2(100.0f, 100.0f);
		m_pBat2->setEnabled(false);
		m_pBat2->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pBat2->setRotation(0.0f);//set angle to 0 degrees
		turn_rate = 5.0f;
		acceleration_rate = 2.0f;
		batSpeed = 10.0f;
		angleRadians = m_pBat->getRotation();
	}

	
	ImGui::Separator();

	static float targetingPos[2] = { m_pTarget->getTransform()->position.x,m_pTarget->getTransform()->position.y };
	if (ImGui::SliderFloat2("Target", targetingPos, 0.0f, 800.0f))
	{
		m_pTarget->getTransform()->position = glm::vec2(targetingPos[0], targetingPos[1]);
		m_pBat->setDestination(m_pTarget->getTransform()->position);
		m_pBat2->setDestination(m_pTarget->getTransform()->position);
	}


	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
