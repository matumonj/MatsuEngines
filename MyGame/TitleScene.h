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
class TitleScene :public BaseScene
{
public:
	TitleScene(SceneManager* sceneManager);
private:
	
	Sprite* titlesprite;
	std::unique_ptr<Object3d>field;
	std::unique_ptr<Object3d>celestal;
	std::unique_ptr<Sprite>TitleMenu[2];
	float menuAlpha[2] = { 1.0f,1.0f };
	XMFLOAT2 MenuScale[2];
	bool menujudg_Play;
	bool menujudg_Edit;
	DebugCamera* camera;
	float FieldRotY;
	LightGroup* lightGroup = nullptr;
	XMFLOAT2 CameraPos;
	float Cangle;
	Sprite* titlesprite2;
	bool t, y;
	XMFLOAT3 FogPos;
	bool feedf;
	bool BackCam;
	//DirectXCommon* dxcomn;
public:
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void SpriteDraw();
	void Finalize()override;


};

