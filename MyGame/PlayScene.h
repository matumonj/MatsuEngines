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
#include"DirectXCommon.h"
#include "FbxLoader.h"
#include "f_Object3d.h"
#include"Texture.h"
#include"PostEffect.h"
#include"Enemy.h"
#include"Effects.h"
#include"TargetMarker.h"
#include"GameUI.h"
#include"ControlBase.h"

#pragma comment(lib,"winmm.lib")
class CollisionManager;
class Player;
class PlayScene :public BaseScene
{
public:
	//�V�[���̃R���X�g���N�^
	PlayScene(SceneManager* sceneManager);
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private:
	const float window_width = 1900;
	const float window_height = 1020;
	static const int debugTextTexNumber = 0;
	// �f�o�C�X
	static ID3D12Device* device;

	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
private:
	Input* input;
	int obbf = 0;
	unsigned long prev_time = timeGetTime();
	int count_frame = 0;
	TargetMarker* targetm = nullptr;
	GameUI* gameui = nullptr;
	float ty;
	float cameraAngle = -90;
	float charaAngle = 0;
	XMFLOAT3 CameraPosition;
	bool cameraMove;
	XMFLOAT3 cameraTargetPoint;
	bool playFeed;
	bool feedout;
	PostEffect* postEffect = nullptr;
	DebugCamera* camera;
private:
	float u;
	XMFLOAT3 Player_Pos;// = player->GetPosition();
	XMFLOAT3 Player_Rot;// = player->GetRotation();

	float distance = 30;

	float dy;
	float dx;
	float dz;

public:
void MyGameDraw();
	void objUpdate( DebugCamera* camera);

public:
	XMFLOAT3 texpo = { 0,0,0 };
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void LoadParam(DebugCamera* camera);
	void SpriteDraw();
	void ImGuiDraw();
	void Finalize()override;
public:
	XMFLOAT3 efkposition = { -50,-10,90 };
	//�G�t�F�N�g�p(�����v���O�����ł������̂̓v���O�����ō����������@���p�͂����Ȃ�)
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
		//�f�o�b�O�p�ϐ� ��X�������
		bool LoadEnemy;
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

