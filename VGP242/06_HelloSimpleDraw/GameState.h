#pragma once

#include <SumEngine/Inc/SumEngine.h>

enum class DebugDrawType
{
	None,
	Cone,
	Line,
	Sphere,
	AABB,
	AABBFilled,
	Sonic,
	Oval,
	OvalFilled,
	GroundCircle,
	GroundPlane
};

class GameState : public SumEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Render() override;
	void DebugUI() override;

	void Update(float deltaTime);
protected:
	void UpdateCamera(float deltaTime);
	SumEngine::Graphics::Camera mCamera;
	DebugDrawType mDebugDrawType = DebugDrawType::None;
};
