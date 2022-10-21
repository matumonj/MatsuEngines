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

public:
	//受攻撃
	void RecvDamage(int Damage);
	//
	void RecvDamage_Cool();
	
private:
	//fbxtime制御
	void FbxAnimationControl();
	void ReturnGround();
	void Jump();
	void Move();
	bool evasionF;
	float evaTime;
	XMFLOAT3 EvaPos;
	void Evasion();
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
	void SetStopFlag(bool f) { StopFlag = f; }
private:
	//プレイヤーが持つ剣
	SwordBase* sword;
	XMFLOAT3 SwordPos;
	XMFLOAT3 SwordRot;
	//前座標
	XMFLOAT3 oldpos;

	XMFLOAT3 onGroundPos;
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
	XMMATRIX rot2;
	XMVECTOR v;

	RotationPrm rotate;
	bool StopFlag;
private:
	float vel;
	XMFLOAT3 MoveVECTOR(DirectX::XMVECTOR v, float angle);

	Input* input = Input::GetInstance();
	f_Model* fbxmodel = nullptr;
	f_Object3d* object1 = nullptr;

private:
	int nogroundtime;
	bool jumpflag;
	float movespeed = 9.0f;
	XMVECTOR Gmove;
public:
	void RotationStatus();
	
	void SetCharaRotation(float angle) { Rotation.y = angle; rotate = RotationPrm::FRONT; }
	
	int GetHP() { return HP; }
	
	int GetMaxHP() { return MaxHP; }

	float GetFbxTime() { return f_time; }

	bool GetNowAttack() { return nowattack; }
	enum AttackMotion {
		NON,
		RUN,
		FIRST,
		SECOND
	};
	AttackMotion GetAttackType() { return attackMotion; }
private:
	
	AttackMotion attackMotion=NON;
	bool SecAttack;
	bool nowSecAttack;
	float sectime=1.8f;
	float f_time;
	float AttackTime = 0.571f;
	float DeathTime =20.1f;
	bool AttackFlag;
	bool nowattack;

	int hindex=15;
};

