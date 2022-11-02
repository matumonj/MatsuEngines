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

