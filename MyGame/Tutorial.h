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
#include"AttackCollision.h"
#include"TargetMarker.h"
#include"ControlBase.h"
#include"ParticleManager.h"
#pragma comment(lib,"winmm.lib")
#include"MinimapSprite.h"
#include"GrassField.h"
#include"GrassObj.h"
class CollisionManager;
class Player;
class Tutorial :public BaseScene
{
public:
	//シーンのコンストラクタ
	Tutorial(SceneManager* sceneManager);
	//~Tutorial();
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private:
	const float window_width = 1900;
	const float window_height = 1020;
	static const int DebugTextSpriteTexNumber = 0;
	
	bool scenechange;
	bool feedflag;
	MinimapSprite* postEffect = nullptr;
	DebugCamera* dc;
public:
	void MyGameDraw();
	void objUpdate(DebugCamera* camera);

public:
	XMFLOAT3 texpo = { 0,0,0 };
	void Initialize()override;
	void Update()override;
	void Draw()override;
	bool LoadParam(DebugCamera* camera);
	void Finalize()override;

private:
	LightGroup* lightGroup = nullptr;
	float ambientColor0[3] = { 1,1,1 };
	// 光線方向初期値
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowPos[3] = { 1,2,0 };
	float circleShadowAtten[3] = { 0.2f,0.0f,0.0f };
	float circleShadowFactorAngle[2] = { 1.0f, 3.0f };
	float circleShadowFactorAngle2[2] = { 1.0f, 6.0f };
	float fighterPos[3] = { 1, 0.0f, 0 };
public:
	XMFLOAT3 efkposition = { -50,-10,90 };
	//エフェクト用(ただプログラムでつくれるものはプログラムで作る方がいい　多用はいくない)
	int c_postEffect = Default;
private:
	enum {
		Blur,
		Default,
	};
	bool Play;
	bool hudload;
	bool Load;
	std::vector<ControlBase*>AllObjectControl;
	std::unique_ptr<GrassField>grassfield;
};

