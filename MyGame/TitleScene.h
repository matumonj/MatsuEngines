#pragma once
#include"BaseScene.h"
#include"Audio.h"
#include"Sprite.h"
#include"Input.h"
#include"WinApp.h"

#include"Object3d.h"
#include"Model.h"
#include"Collision.h"
#include"DebugTxt.h"
#include"LightGroup.h"
#include"DebugCamera.h"
#include"DirectXCommon.h"

class TitleScene : public BaseScene
{
public:
	TitleScene(SceneManager* sceneManager);
private:
	Sprite* titlesprite = nullptr;
	std::unique_ptr<Object3d> field = nullptr;
	std::unique_ptr<Object3d> celestal = nullptr;
	std::unique_ptr<Sprite> TitleMenu[2] = {nullptr};
	DebugCamera* camera = nullptr;
	Sprite* titlesprite2 = nullptr;
	LightGroup* lightGroup = nullptr;

	bool menujudg_Play = false;
	bool menujudg_Edit = false;
	bool feedf = false;
	bool BackCam = false;

	float menuAlpha[2] = {1.0f, 1.0f};
	float Cangle = 0.0f;
	float FieldRotY = 0.0f;
	XMFLOAT2 MenuScale[2] = {{0.0f, 0.0f}};
	XMFLOAT2 CameraPos = {0.0f, 0.0f};


	XMFLOAT3 FogPos = {0.0f, 0.0f, 0.0f};

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

	void TitleFieldInit();
	void TitleFieldUpda();
};
