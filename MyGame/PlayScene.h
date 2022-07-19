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
#include"mEffekseer.h"
#include "FbxLoader.h"
#include "f_Object3d.h"
#include"Texture.h"
#include"PostEffect.h"
#include"Enemy.h"
#include"Effects.h"
#include"TargetMarker.h"
#include"GameUI.h"
#include"OBBCollision.h"
#include"HintBoard.h"
#include"BehaviorTree.h"
#include"Wood.h"
#include "PhysicsAction.h"
#include<fstream>
#include<string>
#include<sstream>
#include<ostream>
#include <iostream>
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
	const float window_width = 1900;
	const float window_height = 1020;
	static const int debugTextTexNumber = 0;
	// デバイス
	static ID3D12Device* device;

	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
private:
	int obbf = 0;
	unsigned long prev_time = timeGetTime();
	int count_frame = 0;
	TargetMarker* targetm = nullptr;
	GameUI* gameui = nullptr;
	float ty;
	float cameraAngle = -90;
	float charaAngle = 0;
	XMFLOAT3 CameraPosition;

	DirectXCommon* dxcomn;
	Sprite* background = nullptr;
	DebugTxt* debugText;
	OBB pobb;// = nullptr;
	OBB eobb;// = nullptr;
	OBBCollision* pobbcol = nullptr;
	
	//Player* player = nullptr;
	
	Object3d* yugudo3d = nullptr;


	Model* playermodel = nullptr;
	Model* model = nullptr;
	Model* model2 = nullptr;
	Model* model5;
	Model* yugudo = nullptr;
	
	PostEffect* postEffect = nullptr;
	DebugCamera* camera;
	int Enemy_Quantity;
	int Wood_Quantity;
	//XMFLOAT3 posxs;
	FILE* fp;
	//std::vector<Enemy> enemys;

	std::vector<std::unique_ptr<Enemy>>enemys;
	std::vector <XMFLOAT3>Load_EnemyPosition;
	std::vector<std::unique_ptr<Wood>>woods;
	std::vector <XMFLOAT3>Load_WoodPosition;
	Effects* effects;
private:
	float u;
	XMFLOAT3 Player_Pos;// = player->GetPosition();
	XMFLOAT3 Player_Rot;// = player->GetRotation();

	float distance = 30;

	float dy;
	float dx;
	float dz;

private://点光源
	LightGroup* lightGroup = nullptr;
	float ambientColor0[3] = { 1,1,1 };

	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	float pointLightPos[3] = { 0,0,-50 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 0.05f,0.05f,0.05f };

	float SpotLightPos[3] = {10,0,0};
	float SpotLightColor[3] = {1,1,1};
	std::vector<int> Num;
	std::vector<int> Wood_Num;
public:
	void SpriteCreate();
	void ModelCreate(DebugCamera* camera);
	void MyGameDraw();
	void SetPrm();
	void objUpdate( DebugCamera* camera);

public:
	XMFLOAT3 texpo = { 0,0,0 };
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void PreBackGround();
	void BackGround();
	void LoadParam();
	void LoadParam_Wood();
	void PostBackGround();
	void SpriteDraw();
	void ImGuiDraw();
	void Finalize()override;
	void r(size_t u);
public:
	XMFLOAT3 efkposition = { -50,-10,90 };
	//エフェクト用(ただプログラムでつくれるものはプログラムで作る方がいい　多用はいくない)

	f_Model* fbxmodel = nullptr;
	f_Object3d* object1 = nullptr;

	int c_postEffect=Default;
private:
	float staycount;
	bool stayf;
	std::vector<XMFLOAT3> stpos;
	std::vector<XMFLOAT3> woodpos;
	std::string line;
	std::stringstream popcom;
	std::string line2;
	std::stringstream popcom2;
	std::ifstream file;
	std::ifstream file2;
	float ey;// = enemys[0]->GetRotation().y;
	float ey1;// = enemys[1]->GetRotation().y;
	BehaviorTree behavior;
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
		bool LoadEnemy;
		bool turnoff_player;
		bool turnoff_enemy;

		float p_alpha=1;
		float e_alpha=1;

		bool CameraViewPoint_First=true;
		bool CameraViewPoint_Third;
		bool t, y;
};

