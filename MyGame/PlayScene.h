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
class PlayScene :public BaseScene
{
public:
	//シーンのコンストラクタ
	PlayScene(SceneManager* sceneManager);
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
	Input* input;
	bool cameraMove;
	XMFLOAT3 cameraTargetPoint;
	bool playFeed;
	bool feedout;
	bool PlayGame;
	PostEffect* postEffect = nullptr;

public:
	void MyGameDraw();
	void objUpdate( DebugCamera* camera);

public:
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void LoadParam(DebugCamera* camera);
	void Finalize()override;
public:
	//エフェクト用(ただプログラムでつくれるものはプログラムで作る方がいい　多用はいくない)
	int c_postEffect=Default;
private:
	enum {
		Blur,
		Default,
	};
	POINT p;
	CollisionManager* collisionManager = nullptr;
	bool hudload;
	float CameraDis = 25;
	float CameraHeight = 9;
	private:
		//デバッグ用変数 後々消すやつ
		bool Load;
		bool turnoff_player;
		bool turnoff_enemy;

		float p_alpha=1;
		float e_alpha=1;

		bool CameraViewPoint_First=true;
		bool CameraViewPoint_Third;
		bool t, y;

		private:
			std::vector<Enemy*>subenemy;
			std::vector<ControlBase*>AllObjectControl;
};

