#pragma once
#include "BaseScene.h"
#include"Audio.h"
#include"Sprite.h"
#include"Input.h"
#include"Particle.h"
#include"Collision.h"
#include"LightGroup.h"
#include"DirectXCommon.h"
#include"PostEffect.h"
#include"Enemy.h"
#include"Effects.h"
#include"TargetMarker.h"
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
private:
	float LightUpDownT;
	LightGroup* lightGroup = nullptr;
	float ambientColor0[3] = {1.0f, 1.0f, 1.0f};
	// �������������l
	float lightDir0[3] = {0.0f, 0.0f, 1.0f};
	float lightColor0[3] = {1.0f, 0.0f, 0.0f};

	float lightDir1[3] = {0.0f, 1.0f, 0.0f};
	float lightColor1[3] = {0.0f, 1.0f, 0.0f};

	float lightDir2[3] = {1.0f, 0.0f, 0.0f};
	float lightColor2[3] = {0.0f, 0.0f, 1.0f};


	float pointLightPos[3] = {0, 0, 0};
	float pointLightColor[3] = {1, 1, 1};
	float pointLightAtten[3] = {0.003f, 0.001f, 0.003f};

	float circleShadowDir[3] = {0.0f, -1.0f, 0.0f};
	float circleShadowPos[3] = {1.0f, 2.0f, 0.0f};
	float circleShadowAtten[3] = {0.2f, 0.0f, 0.0f};
	float circleShadowFactorAngle[2] = {1.0f, 3.0f};
	float circleShadowFactorAngle2[2] = {1.0f, 6.0f};

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
