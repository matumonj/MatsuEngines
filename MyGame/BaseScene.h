#pragma once
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")

#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include "LightGroup.h"
class SceneManager;

class BaseScene
{
public:
	BaseScene(SceneManager* sceneManager);
	//~BaseScene();
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;

protected:
	LightGroup* lightGroup = nullptr;
	float ambientColor0[3] = { 1.0f, 1.0f, 1.0f };
	// åıê¸ï˚å¸èâä˙íl
	float lightDir0[3] = { 1.0f, 0.0f, 1.0f };
	float lightColor0[3] = { 1.0f, 0.0f, 0.0f };

	float lightDir1[3] = { 1.0f, 1.0f, 0.0f };
	float lightColor1[3] = { 0.0f, 1.0f, 0.0f };

	float lightDir2[3] = { 1.0f, 0.0f, 0.0f };
	float lightColor2[3] = { 0.0f, 0.0f, 1.0f };


	float pointLightPos[3] = { 0, 0, 0 };
	float pointLightColor[3] = { 1, 1, 1 };
	float pointLightAtten[3] = { 0.003f, 0.001f, 0.003f };

	float circleShadowDir[3] = { 0.0f, -1.0f, 0.0f };
	float circleShadowPos[3] = { 1.0f, 2.0f, 0.0f };
	float circleShadowAtten[3] = { 0.2f, 10.0f, 10.0f };
	float circleShadowFactorAngle[2] = { 1.0f, 3.0f };
	float circleShadowFactorAngle2[2] = { 1.0f, 6.0f };
	//virtual void SpriteDraw(ID3D12GraphicsCommandList* cmdList) = 0;
protected:
	SceneManager* sceneManager_ = nullptr;
};
