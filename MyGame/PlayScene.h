#pragma once
#include"BaseScene.h"
class CollisionManager;

class PlayScene : public BaseScene
{
public:
	//シーンのコンストラクタ
	PlayScene(SceneManager* sceneManager);
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = XMFLOAT2;
	using XMFLOAT3 = XMFLOAT3;
	using XMFLOAT4 = XMFLOAT4;
	using XMMATRIX = XMMATRIX;
	using XMVECTOR = XMVECTOR;

private:
	bool playFeed;
	bool PlayGame;
public:
	void objUpdate();

public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void LoadParam();
	void LightUpdate() override;
	void Finalize() override;
	void MyGameDraw() override;
	void SpriteDraw() override;
private:
	void ChangeSceneJudg();
public:
	float ambientColor0[3] = {1.0f, 1.0f, 1.0f};
	// 光線方向初期値
	float lightDir0[3] = {1.0f, 0.0f, 1.0f};
	float lightColor0[3] = {1.0f, 0.0f, 0.0f};

	float lightDir1[3] = {1.0f, 1.0f, 0.0f};
	float lightColor1[3] = {0.0f, 1.0f, 0.0f};

	float lightDir2[3] = {1.0f, 0.0f, 0.0f};
	float lightColor2[3] = {0.0f, 0.0f, 1.0f};


	float pointLightPos[3] = {0, 0, 0};
	float pointLightColor[3] = {1, 1, 1};
	float pointLightAtten[3] = {0.003f, 0.001f, 0.003f};

	float circleShadowDir[3] = {0.0f, -1.0f, 0.0f};
	float circleShadowPos[3] = {1.0f, 2.0f, 0.0f};
	float circleShadowAtten[3] = {0.2f, 10.0f, 10.0f};
	float circleShadowFactorAngle[2] = {1.0f, 3.0f};
	float circleShadowFactorAngle2[2] = {1.0f, 6.0f};
	//virtual void SpriteDraw(ID3D12GraphicsCommandList* cmdList) = 0;

private:
	bool hudload;
	bool Load;
};
