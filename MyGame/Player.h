#pragma once
#include"Object3d.h"
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "FbxLoader.h"
#include"f_Model.h"
#include"f_Object3d.h"
#include"ObjectManager.h"
#include"Input.h"
#include"SwordBase.h"
class Enemy;
class TargetMarker;
class Player :public ObjectManager
{
public:
	~Player();

public:
	static Player* Create(Model* model, DebugCamera* camera);
	
	static Player* GetInstance();

public:
	//初期化
	void Initialize(DebugCamera* camera)override;
	//更新処理
	void Update(DebugCamera* camera)override;
	//描画
	void Draw();
	//描画 imgui
	void ImguiDraw();
public:
	//受攻撃
	void RecvDamage(int Damage);
	//
	void RecvDamage_Cool();
	//fbxtime制御
	void FbxAnimationControl();

public:
	//OBBの当たり判定に使うプレイヤーの回転行列取得
	XMMATRIX GetMatrot();
	XMMATRIX GetHandMatRot() { return m_fbxObject->GetRotmatrix(); }
	XMFLOAT3 GetHandRotation_xyz() { return m_fbxObject->GetHandRotation(); }
	//SelectSwordに使うプレイヤーの手のワールド行列
	XMMATRIX GetHanMat() { return HandMat; };
	//座標の押し戻し
	void isOldPos() {
		Position.x = oldpos.x; Position.z = oldpos.z;
	}
private:
	//プレイヤーが持つ剣
	SwordBase* sword;
	XMFLOAT3 SwordPos;
	XMFLOAT3 SwordRot;
	//前座標
	XMFLOAT3 oldpos;
	//手行列
	XMMATRIX HandMat;
	//体力周り
	int HP;
	const int MaxHP = 100;
	//
	int CoolTime;
	//プレイヤーの回転用
	enum class RotationPrm {
		RIGHT,//右
		LEFT,//左
		FRONT,//前
		BACK,//後ろ
	};
	float angle;
	//カメラに合わせたキャラクターの回転
	float CharaRotation = 0;
	RotationPrm rotate;

private:
	Input* input = Input::GetInstance();
	f_Model* fbxmodel = nullptr;
	f_Object3d* object1 = nullptr;

private:
	bool jumpflag;
	float movespeed = 8.0f;
public:
	void RotationStatus();
	void SetCharaRotation(float angle) { Rotation.y = angle; rotate = RotationPrm::FRONT; }

	int GetHP() { return HP; }

private:
	float f_time;
	float AttackTime = 1.5f;
	float DeathTime = 4.9f;
	bool AttackFlag;
	bool nowattack;
};

