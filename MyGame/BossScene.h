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
#include"MinimapSprite.h"
#pragma comment(lib,"winmm.lib")
class CollisionManager;
class Player;

class BossScene :
	public BaseScene
{
public:
	BossScene(SceneManager* sceneManager);
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T>
	using ComPtr = ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = XMFLOAT2;
	using XMFLOAT3 = XMFLOAT3;
	using XMFLOAT4 = XMFLOAT4;
	using XMMATRIX = XMMATRIX;
	using XMVECTOR = XMVECTOR;

public:
	Input* input;

	bool scenechange;
	bool feedflag;
	XMFLOAT4 feedcolor = {0, 0, 0, 1};
	std::unique_ptr<MinimapSprite> minimap;
	DebugCamera* dc;
public:
	//�G�t�F�N�g�p(�����v���O�����ł������̂̓v���O�����ō����������@���p�͂����Ȃ�)
	int c_postEffect = Default;
	MinimapSprite* postEffect = nullptr;
	bool Play;
	bool hudload;
private:
	enum
	{
		Blur,
		Default,
	};

public:
	void MyGameDraw();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	bool LoadParam(DebugCamera* camera);
	void Finalize() override;

private:
	bool LoadEnemy;
	std::vector<ControlBase*> AllObjectControl;
};
