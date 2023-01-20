#pragma once
#include "BaseScene.h"
#include"Audio.h"
#include"Sprite.h"
#include"Input.h"
#include"Particle.h"
#include"Collision.h"
#include"LightGroup.h"
#include"PostEffect.h"
#include"ControlBase.h"
#include"CircleAttack.h"
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
	Particle* p;
	bool scenechange;
	int count_frame = 0;
	std::unique_ptr<BomAttack> bAttack;
private:
	//�G�t�F�N�g�p(�����v���O�����ł������̂̓v���O�����ō����������@���p�͂����Ȃ�)
	int c_postEffect = Default;
	PostEffect* postEffect = nullptr;
	bool Play;
	bool hudload;

	float lightangle[5];
	XMFLOAT3 LightPos[5];
	XMFLOAT3 LightScl;
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
	bool LoadParam();
	void Finalize() override;

private:
	bool feedend;
	void ChangeScene();
private:
	bool LoadEnemy;
	std::vector<ControlBase*> AllObjectControl;
};
