#pragma once
#include"BaseScene.h"
#include"Sprite.h"
#include"Object3d.h"
#include"LightGroup.h"
#include"DebugCamera.h"
#include"DirectXCommon.h"

class TitleScene : public BaseScene
{
public:
	TitleScene(SceneManager* sceneManager);
private:
	std::unique_ptr<Sprite> titlesprite = nullptr;

	std::unique_ptr<Object3d> field = nullptr;
	std::unique_ptr<Object3d> celestal = nullptr;
	std::unique_ptr<Sprite> TitleMenu[2] = {nullptr};
	std::unique_ptr<DebugCamera> camera = nullptr;

	bool menujudg_Play = false;
	bool menujudg_Edit = false;
	bool feedf = false;
	bool CameraBackF = false;

	float menuAlpha[2] = {1.0f, 1.0f};
	XMFLOAT2 MenuScale[2] = {{0.0f, 0.0f}};


private:
	float Cangle = 0.0f;
	float FieldRotY = 0.0f;
	XMFLOAT2 CameraPos = { 0.0f, 0.0f };

public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void LightUpdate()override;
	void Finalize() override;
	void MyGameDraw()override;
	void SpriteDraw()override;

private:
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

	void TitleTexInit();
	void TitleTexUpda();
	void TitleTexDraw();

	bool ChangeScene();
	void TitleFieldInit();
	void TitleFieldUpda();
};
