#include "GameState.h"

using namespace SumEngine;
using namespace SumEngine::Math;
using namespace SumEngine::Graphics;
using namespace SumEngine::Core;
using namespace SumEngine::Input;
using namespace SumEngine::Audio;

void GameState::Initialize()
{
	mGameWorld.Initialize();

	GameObject* transformGO = mGameWorld.CreateGameObject("Transform");
	TransformComponent* transformComponent = transformGO->AddComponent<TransformComponent>();

	GameObject* cameraGO = mGameWorld.CreateGameObject("Camera");
	CameraComponent* cameraComponent = cameraGO->AddComponent<CameraComponent>();
	cameraGO->AddComponent<FPSCameraComponent>();
	cameraComponent->GetCamera().SetPosition({ 0.0f, 2.0f, -3.0f });
	cameraComponent->GetCamera().SetLookAt({ 0.0f, 1.0f, 0.0f });
	cameraGO->Initialize();

	mCameraComponent = cameraComponent;
}

void GameState::Terminate()
{
	mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
	mGameWorld.Update(deltaTime);
}

void GameState::Render()
{
	mGameWorld.Render();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	mGameWorld.DebugUI();
	ImGui::End();

	SimpleDraw::Render(mCameraComponent->GetCamera());
}
