#pragma once
#include"Sprite.h"

#include"Object3d.h"
#include"Model.h"
#include"Collision.h"
#include<array>
#include"LightGroup.h"
#include"DebugCamera.h"
#include"DirectXCommon.h"
#include "BaseScene.h"
class ClearScene :
    public BaseScene
{

public:
	ClearScene(SceneManager* sceneManager);
private:
	Sprite* titlesprite = nullptr;

	std::array<std::unique_ptr<Sprite>,9> cleartex;
	std::array<float, 9> texangle;
	std::array<float, 9> texalpha;
	std::array<XMFLOAT2, 9> texpos;


	std::unique_ptr<Sprite>backgroundtex;
	LightGroup* lightGroup = nullptr;

	bool menujudg_Play = false;
	bool menujudg_Edit = false;
	bool feedf = false;
	bool BackCam = false;

	float Cangle = 0.0f;
	float FieldRotY = 0.0f;
	XMFLOAT2 MenuScale[2] = { {0.0f, 0.0f} };
	XMFLOAT2 CameraPos = { 0.0f, 0.0f };


	XMFLOAT3 FogPos = { 0.0f, 0.0f, 0.0f };

	//DirectXCommon* dxcomn;
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void SpriteDraw();
	void Finalize() override;

private:
	void TitleTexInit();
	void TitleTexUpda();
	void TitleTexDraw();

	bool ChangeScene();
};

