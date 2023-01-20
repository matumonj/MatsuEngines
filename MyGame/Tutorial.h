#pragma once
#include"BaseScene.h"
#include"Audio.h"
#include"Sprite.h"
#include"LightGroup.h"
#include"DebugCamera.h"
#include"PostEffect.h"
#include"ControlBase.h"
#pragma comment(lib,"winmm.lib")
#include"GrassField.h"
class CollisionManager;
class Player;

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
	bool scenechange = false;
	bool feedflag = false;
	PostEffect* postEffect = nullptr;
public:
	void objUpdate();
	void LightSet();
public:
	XMFLOAT3 texpo = {0.0f, 0.0f, 0.0f};
	void Initialize() override;
	void Update() override;
	void Draw() override;
	bool LoadParam();
	void Finalize() override;
	void MyGameDraw()override;
	void SpriteDraw()override;

	void LightUpdate()override;
private:
	bool feedout = true;
public:
	//エフェクト用(ただプログラムでつくれるものはプログラムで作る方がいい　多用はいくない)
	int c_postEffect = Default;
private:
	enum
	{
		Blur,
		Default,
	};

	bool Play = false;
	bool hudload = false;
	bool Load = false;

	std::unique_ptr<GrassField> grassfield;
};
