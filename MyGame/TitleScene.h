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
	void TitleTexInit();
	void TitleTexUpda();
	void TitleTexDraw();

	bool ChangeScene();
	void TitleFieldInit();
	void TitleFieldUpda();
};
