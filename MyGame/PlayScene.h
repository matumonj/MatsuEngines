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
#include "FbxLoader.h"
#include "f_Object3d.h"
#include"Texture.h"
#include"PostEffect.h"
#include"Enemy.h"
#include"TargetMarker.h"
#include"ControlBase.h"
#pragma comment(lib,"winmm.lib")
class CollisionManager;
class Player;

class PlayScene : public BaseScene
{
public:
	//シーンのコンストラクタ
	PlayScene(SceneManager* sceneManager);
private: // エイリアス
	//Microsoft::WRL::を省略
	template <class T>
	using ComPtr = ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = XMFLOAT2;
	using XMFLOAT3 = XMFLOAT3;
	using XMFLOAT4 = XMFLOAT4;
	using XMMATRIX = XMMATRIX;
	using XMVECTOR = XMVECTOR;

private:
	bool playFeed;
	bool PlayGame;
	PostEffect* postEffect = nullptr;

public:
	void MyGameDraw();
	void objUpdate(DebugCamera* camera);

public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void LoadParam(DebugCamera* camera);
	void Finalize() override;

private:
	void ChangeSceneJudg();
public:
	//エフェクト用(ただプログラムでつくれるものはプログラムで作る方がいい　多用はいくない)
	int c_postEffect = Default;
private:
	enum
	{
		Blur,
		Default,
	};

	bool hudload;
	bool Load;
private:
	float ambientColor0[3] = {1, 1, 1};
	// 光線方向初期値
	float lightDir0[3] = {0, 0, 1};
	float lightColor0[3] = {1, 0, 0};

	float lightDir1[3] = {0, 1, 0};
	float lightColor1[3] = {0, 1, 0};

	float lightDir2[3] = {1, 0, 0};
	float lightColor2[3] = {0, 0, 1};

	float circleShadowDir[3] = {0, -1, 0};
	float circleShadowPos[3] = {1, 2, 0};
	float circleShadowAtten[3] = {0.2f, 0.0f, 0.0f};
	float circleShadowFactorAngle[2] = {1.0f, 3.0f};
	float circleShadowFactorAngle2[2] = {1.0f, 6.0f};
	float fighterPos[3] = {1, 0.0f, 0};
	DebugCamera* dc;
	LightGroup* lightGroup;
	std::vector<ControlBase*> AllObjectControl;
};
