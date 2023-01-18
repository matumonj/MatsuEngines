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
	//�V�[���̃R���X�g���N�^
	Tutorial(SceneManager* sceneManager);
	//~Tutorial();
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

private:
	bool scenechange = false;
	bool feedflag = false;
	PostEffect* postEffect = nullptr;
	DebugCamera* dc = nullptr;
public:
	void MyGameDraw();
	void objUpdate();
	void LightSet();
	void LightUpdate();
public:
	XMFLOAT3 texpo = {0.0f, 0.0f, 0.0f};
	void Initialize() override;
	void Update() override;
	void Draw() override;
	bool LoadParam();
	void Finalize() override;

private:
	bool feedout = true;
	LightGroup* lightGroup = nullptr;
	float ambientColor0[3] = {1.0f, 1.0f, 1.0f};
	// �������������l
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
public:
	//�G�t�F�N�g�p(�����v���O�����ł������̂̓v���O�����ō����������@���p�͂����Ȃ�)
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

	std::vector<ControlBase*> AllObjectControl;
	std::unique_ptr<GrassField> grassfield;
};
