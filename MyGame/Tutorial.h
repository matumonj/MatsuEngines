#pragma once
#include"BaseScene.h"
class CollisionManager;

class Tutorial : public BaseScene
{
public:
	//シーンのコンストラクタ
	Tutorial(SceneManager* sceneManager);
	//~Tutorial();
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T>
	using ComPtr = ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = XMFLOAT2;
	using XMFLOAT3 = XMFLOAT3;
	using XMFLOAT4 = XMFLOAT4;
	using XMMATRIX = XMMATRIX;
	using XMVECTOR = XMVECTOR;

private:
	float vig_gb;
	bool scenechange = false;
	bool feedflag = false;
	float n;
	float r, l;
	bool fp;
public:
	void objUpdate();
	void LightSet();
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	bool LoadParam();
	void Finalize() override;
	void MyGameDraw() override;
	void SpriteDraw() override;

	void LightUpdate() override;
private:
	bool feedout = true;
	float lightY;
public:
	float ambientColor0[3] = {1.0f, 1.0f, 1.0f};
	// 光線方向初期値
	float lightDir0[3] = {0.0f, 0.0f, 1.0f};
	float lightColor0[3] = {1.0f, 0.0f, 0.0f};

	float lightDir1[3] = {0.0f, 1.0f, 0.0f};
	float lightColor1[3] = {0.0f, 1.0f, 0.0f};

	float lightDir2[3] = {1.0f, 0.0f, 0.0f};
	float lightColor2[3] = {0.0f, 0.0f, 1.0f};

	float circleShadowDir[3] = {0.0f, -1.0f, 0.0f};
	float circleShadowPos[3] = {1.0f, 2.0f, 0.0f};
	float circleShadowAtten[3] = {0.2f, 0.0f, 0.0f};
	float circleShadowFactorAngle[2] = {1.0f, 3.0f};
	float circleShadowFactorAngle2[2] = {1.0f, 6.0f};
	float fighterPos[3] = {1.0f, 0.0f, 0.0f};

private:
	bool Play = false;
	bool hudload = false;
	bool Load = false;
};
