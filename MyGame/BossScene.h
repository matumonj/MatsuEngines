#pragma once
#include "BaseScene.h"
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
#include "FbxLoader.h"
#include "f_Object3d.h"
#include"Texture.h"
#include"PostEffect.h"
#include"Enemy.h"
#include"Effects.h"
#include"AttackCollision.h"
#include"TargetMarker.h"
#include"ControlBase.h"
#include"Nail.h"
#pragma comment(lib,"winmm.lib")
class CollisionManager;
class Player;
class BossScene :
    public BaseScene
{
public:
	BossScene(SceneManager* sceneManager);
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	Input* input;
	
	bool scenechange;
	bool feedflag;
	XMFLOAT4 feedcolor = { 0,0,0,1 };
public:
	//エフェクト用(ただプログラムでつくれるものはプログラムで作る方がいい　多用はいくない)
	int c_postEffect = Default;
	PostEffect* postEffect = nullptr;
	bool Play;
	bool hudload;
private:
	enum {
		Blur,
		Default,
	};
public:
	void MyGameDraw();

	void Initialize()override;
	void Update()override;
	void Draw()override;
	bool LoadParam(DebugCamera* camera);
	void Finalize()override;

private:
	bool LoadEnemy;
	std::vector<ControlBase*>AllObjectControl;
};

