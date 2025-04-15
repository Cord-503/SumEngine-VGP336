#include "GameState.h"

using namespace SumEngine;
using namespace SumEngine::Math;
using namespace SumEngine::Graphics;
using namespace SumEngine::Core;
using namespace SumEngine::Input;

const char* gDrawTypeNames[] =
{
	"Sun",
	"Mercury",
	"Venus",
	"Earth",
	"Mars",
	"Jupiter",
	"Saturn",
	"Uranus",
	"Neptune",
	"Pluto",
	"Galaxy"
};

const char* cTextureLocations[] =
{
	"planets/sun.jpg",
	"planets/mercury.jpg",
	"planets/venus.jpg",
	"planets/earth.jpg",
	"planets/mars.jpg",
	"planets/jupiter.jpg",
	"planets/saturn.jpg",
	"planets/neptune.jpg",
	"planets/uranus.jpg",
	"planets/pluto.jpg",
	"skysphere/space.jpg"
};

void GameState::Initialize()
{
	// Set Cameras
	mCamera.SetPosition({ 0.0f, 500.0f, -300.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	mRenderTargetCamera.SetPosition({ 0.0f, 0.0f, -300.0f });
	mRenderTargetCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	mRenderTargetCamera.SetAspectRatio(1.0f);


	// Create Meshes
	mObjects[(int)SolarSystem::Sun].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 109.0f));
	mObjects[(int)SolarSystem::Mercury].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 0.3f));
	mObjects[(int)SolarSystem::Venus].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 0.8f));
	mObjects[(int)SolarSystem::Earth].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 1.0f));
	mObjects[(int)SolarSystem::Mars].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 0.5f));
	mObjects[(int)SolarSystem::Jupiter].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 11.0f));
	mObjects[(int)SolarSystem::Saturn].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 10.0f));
	mObjects[(int)SolarSystem::Uranus].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 4.0f));
	mObjects[(int)SolarSystem::Neptune].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 4.0f));
	mObjects[(int)SolarSystem::Pluto].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 0.2f));
	mObjects[(int)SolarSystem::Galaxy].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSkySpherePX(100, 100, 1000.0f));

	mConstantBuffer.Initialize(sizeof(Matrix4));

	std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexture.fx";
	mVertexShader.Initialize<VertexPX>(shaderFile);

	mPixelShader.Initialize(shaderFile);

	// Create Textures
	for (int i = 0; i < (int)SolarSystem::End; i++)
	{
		mObjects[i].mDiffuseTexture.Initialize("../../Assets/Images/" + (std::string)cTextureLocations[i]);
	}

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

	constexpr uint32_t size = 512;
	mRenderTarget.Initialize(size, size, Texture::Format::RGBA_U32);

	// Set Positions
	mObjects[(int)SolarSystem::Sun].distanceFromSun = 0;
	mObjects[(int)SolarSystem::Mercury].distanceFromSun = 150;
	mObjects[(int)SolarSystem::Venus].distanceFromSun = 200;
	mObjects[(int)SolarSystem::Earth].distanceFromSun = 350;
	mObjects[(int)SolarSystem::Mars].distanceFromSun = 400;
	mObjects[(int)SolarSystem::Jupiter].distanceFromSun = 600;
	mObjects[(int)SolarSystem::Saturn].distanceFromSun = 700;
	mObjects[(int)SolarSystem::Uranus].distanceFromSun = 750;
	mObjects[(int)SolarSystem::Neptune].distanceFromSun = 800;
	mObjects[(int)SolarSystem::Pluto].distanceFromSun = 900;
	mObjects[(int)SolarSystem::Galaxy].distanceFromSun = 0;

	// Set Rotations
	mObjects[(int)SolarSystem::Sun].rotationSpeed = 1;
	mObjects[(int)SolarSystem::Mercury].rotationSpeed = 10;
	mObjects[(int)SolarSystem::Venus].rotationSpeed = 15;
	mObjects[(int)SolarSystem::Earth].rotationSpeed = 20;
	mObjects[(int)SolarSystem::Mars].rotationSpeed = 25;
	mObjects[(int)SolarSystem::Jupiter].rotationSpeed = 30;
	mObjects[(int)SolarSystem::Saturn].rotationSpeed = 35;
	mObjects[(int)SolarSystem::Uranus].rotationSpeed = 40;
	mObjects[(int)SolarSystem::Neptune].rotationSpeed = 45;
	mObjects[(int)SolarSystem::Pluto].rotationSpeed = 50;
	mObjects[(int)SolarSystem::Galaxy].rotationSpeed = 0;

	// Set Revolutions
	mObjects[(int)SolarSystem::Sun].orbitSpeed = 0.0f;
	mObjects[(int)SolarSystem::Mercury].orbitSpeed = 2.0f;
	mObjects[(int)SolarSystem::Venus].orbitSpeed = 2.5f;
	mObjects[(int)SolarSystem::Earth].orbitSpeed = 3.0f;
	mObjects[(int)SolarSystem::Mars].orbitSpeed = 3.5f;
	mObjects[(int)SolarSystem::Jupiter].orbitSpeed = 4.0f;
	mObjects[(int)SolarSystem::Saturn].orbitSpeed = 4.5f;
	mObjects[(int)SolarSystem::Uranus].orbitSpeed = 5.0f;
	mObjects[(int)SolarSystem::Neptune].orbitSpeed = 5.5f;
	mObjects[(int)SolarSystem::Pluto].orbitSpeed = 6.0f;
	mObjects[(int)SolarSystem::Galaxy].orbitSpeed = 0.0f;

}

void GameState::Terminate()
{
	mRenderTarget.Terminate();
	mSampler.Terminate();

	for (int i = (int)SolarSystem::End - 1; i >= 0; i--)
	{
		mObjects[i].mDiffuseTexture.Terminate();
	}

	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mConstantBuffer.Terminate();

	for (int i = (int)SolarSystem::End - 1; i >= 0; i--)
	{
		mObjects[i].mMeshBuffer.Terminate();
	}
}

void GameState::UpdateCamera(float deltaTime)
{
	auto input = InputSystem::Get();
	const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 1.0f) * deltaTime;
	const float turnSpeed = 0.1f * deltaTime;
	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed);
	}
	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed);
	}
	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed);
	}
	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed);
	}
}

float totalTime = 0.0f;
void GameState::Update(float deltaTime)
{
	totalTime += deltaTime / 10.0f;
	UpdateCamera(deltaTime);
}

int currentRenderTarget = 0;
float renderTargetDistance = 0.0f;
bool ringsToggle = true;
void GameState::Render()
{
	// Render Orbit Rings
	if (ringsToggle)
	{
		for (int i = 1; i < (int)SolarSystem::Galaxy; i++)
		{
			SimpleDraw::AddGroundCircle(100, mObjects[i].distanceFromSun, { 0,0,0 }, Colors::Gray);
		}
		// Saturn Ring
		{
			Matrix4 ringWorldPosition = Matrix4::RotationY(mObjects[(int)SolarSystem::Saturn].rotationSpeed * totalTime) * Matrix4::Translation(Vector3::ZAxis * mObjects[(int)SolarSystem::Saturn].distanceFromSun) * Matrix4::RotationY(mObjects[(int)SolarSystem::Saturn].orbitSpeed * totalTime);
			Vector3 ringPosition = { ringWorldPosition._41,ringWorldPosition._42,ringWorldPosition._43 };
			SimpleDraw::AddGroundCircle(100, 25, ringPosition, Colors::White);
		}

		SimpleDraw::Render(mCamera);
	}

	// Render objects
	for (TexturedObject& object : mObjects)
	{
		mVertexShader.Bind();
		mPixelShader.Bind();
		object.mDiffuseTexture.BindPS(0);
		mSampler.BindPS(0);

		// constant buffer
		Matrix4 matWorld = Matrix4::RotationY(object.rotationSpeed * totalTime) * Matrix4::Translation(Vector3::ZAxis * object.distanceFromSun) * Matrix4::RotationY(object.orbitSpeed * totalTime);
		Matrix4 matView = mCamera.GetViewMatrix();
		Matrix4 matProj = mCamera.GetProjectionMatrix();
		Matrix4 matFinal = matWorld * matView * matProj;
		Matrix4 wvp = Transpose(matFinal);
		mConstantBuffer.Update(&wvp);
		mConstantBuffer.BindVS(0);
		object.mMeshBuffer.Render();
	}

	mVertexShader.Bind();
	mPixelShader.Bind();
	mObjects[currentRenderTarget].mDiffuseTexture.BindPS(0);
	mSampler.BindPS(0);

	Matrix4 matWorld = Matrix4::RotationY(mObjects[currentRenderTarget].rotationSpeed * totalTime);
	Matrix4 matView = mRenderTargetCamera.GetViewMatrix();
	Matrix4 matProj = mRenderTargetCamera.GetProjectionMatrix();
	Matrix4 matFinal = matWorld * matView * matProj;
	Matrix4 wvp = Transpose(matFinal);
	mConstantBuffer.Update(&wvp);
	mConstantBuffer.BindVS(0);

	mRenderTargetCamera.SetPosition({ 0.0f, -renderTargetDistance, renderTargetDistance});
	mRenderTargetCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	mRenderTarget.BeginRender();
		mObjects[currentRenderTarget].mMeshBuffer.Render();
		SimpleDraw::AddGroundCircle(100, 50, Vector3::Zero, Colors::White);
		SimpleDraw::Render(mRenderTargetCamera);
	mRenderTarget.EndRender();
}

bool buttonValue = false;
void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Checkbox("OrbitRings", &ringsToggle);

	int currentDrawType = static_cast<int>(mCurrentTarget);
	if (ImGui::Combo("Objects", &currentRenderTarget, gDrawTypeNames, static_cast<int>(std::size(gDrawTypeNames))))
	{
		mCurrentTarget = (SolarSystem)currentRenderTarget;
	}

	ImGui::DragFloat("RenderTargetDistance", &renderTargetDistance, 1.0f, -300.0f, 0.0f);
	ImGui::DragFloat("OrbitSpeed", &mObjects[currentDrawType].orbitSpeed);
	ImGui::DragFloat("RotationSpeed", &mObjects[currentDrawType].rotationSpeed);

	ImGui::Image(
		mRenderTarget.GetRawData(),
		{ 256, 256 },
		{ 0, 0 },	//uv0
		{ 1, 1 },	//uv1
		{ 1, 1, 1, 1 },
		{ 1, 1, 1, 1 });


	ImGui::End();
}
