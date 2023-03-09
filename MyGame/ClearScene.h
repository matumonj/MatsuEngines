#pragma once
#include"BaseScene.h"
#include"Sprite.h"
#include<array>
#include"Object3d.h"
#include"LightGroup.h"
#include"DirectXCommon.h"

class ClearScene :
	public BaseScene
{
public:
	ClearScene(SceneManager* sceneManager);
private:
	Sprite* titlesprite = nullptr;

	std::array<std::unique_ptr<Sprite>, 9> cleartex;
	std::array<float, 9> texangle;
	std::array<float, 9> texalpha;
	std::array<XMFLOAT2, 9> texpos;


	std::unique_ptr<Sprite> backgroundtex;
	LightGroup* lightGroup = nullptr;

	bool menujudg_Play = false;
	bool menujudg_Edit = false;
	bool feedf = false;
	bool BackCam = false;

	float Cangle = 0.0f;
	float FieldRotY = 0.0f;
	XMFLOAT2 MenuScale[2] = {{0.0f, 0.0f}};
	XMFLOAT2 CameraPos = {0.0f, 0.0f};
	std::unique_ptr<DebugCamera> cameras = nullptr;

	std::unique_ptr<Object3d> celestal = nullptr;

	XMFLOAT3 FogPos = {0.0f, 0.0f, 0.0f};

	//DirectXCommon* dxcomn;
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	void LightUpdate() override;
	void SpriteDraw() override;
	void MyGameDraw() override;
private:
	void TitleTexInit();
	void TitleTexUpda();
	void TitleTexDraw();

	bool ChangeScene();
};
