#pragma once
#include"BaseScene.h"
#include"Collision.h"
#include"LightGroup.h"
#include"DebugCamera.h"
#include"PostEffect.h"
#include"Enemy.h"
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
	void objUpdate();

public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void LoadParam();
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

	float fighterPos[3] = {1, 0.0f, 0};
	DebugCamera* dc;
	std::vector<ControlBase*> AllObjectControl;
};
