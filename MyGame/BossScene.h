#pragma once
#include "BaseScene.h"
class CollisionManager;

class BossScene :
	public BaseScene
{
public:
	BossScene(SceneManager* sceneManager);
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = XMFLOAT2;
	using XMFLOAT3 = XMFLOAT3;
	using XMFLOAT4 = XMFLOAT4;
	using XMMATRIX = XMMATRIX;
	using XMVECTOR = XMVECTOR;

private:
	float ambientColor0[3] = {1.0f, 1.0f, 1.0f};
	// 光線方向初期値
	float lightDir0[3] = {1.0f, 0.0f, 1.0f};
	float lightColor0[3] = {1.0f, 0.0f, 0.0f};

	float lightDir1[3] = {1.0f, 1.0f, 0.0f};
	float lightColor1[3] = {0.0f, 1.0f, 0.0f};

	float lightDir2[3] = {1.0f, 0.0f, 0.0f};
	float lightColor2[3] = {0.0f, 0.0f, 1.0f};


	float circleShadowDir[3] = {0.0f, -1.0f, 0.0f};
	float circleShadowPos[3] = {1.0f, 2.0f, 0.0f};
	float circleShadowAtten[3] = { 0.56f,0.46f,0.f };
	float circleShadowAtten2[3] = { 0.f,0.07f,0.f };
	float circleShadowFactorAngle[2] = {0.56f, 2.7f};
	float circleShadowFactorAngle2[2] = {1.2f,5.3f};
	//virtual void SpriteDraw(ID3D12GraphicsCommandList* cmdList) = 0;
	float lightY=40.f;
	XMFLOAT2 PlayerCShadowAngle = { 0,0 };
	bool Play;
	bool hudload;

public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	bool LoadParam();
	void Finalize() override;
	void LightUpdate() override;
	void MyGameDraw() override;
	void SpriteDraw() override;
private:
	void ChangeScene();
private:
	bool LoadF;
	float posy = 15.f;
	XMFLOAT3 atten;
	XMFLOAT2 spotangle = {27.f, 70.f};
	float bloomval;
	float EaseT;
	enum ShadowChara
	{
		SUMMONENEY_A,
		SUMMONENEY_B,
		PLAYER,
		BOSSENEMY
	};
};
