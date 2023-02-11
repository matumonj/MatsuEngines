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
	float circleShadowAtten[3] = {0.2f, 10.0f, 10.0f};
	float circleShadowAtten2[3] = {0.2f, 10.0f, 10.0f};
	float circleShadowFactorAngle[2] = {1.0f, 3.0f};
	float circleShadowFactorAngle2[2] = {1.0f, 6.0f};
	//virtual void SpriteDraw(ID3D12GraphicsCommandList* cmdList) = 0;

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

	enum ShadowChara
	{
		SUMMONENEY_A,
		SUMMONENEY_B,
		PLAYER,
		BOSSENEMY
	};
};
