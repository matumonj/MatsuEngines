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
#include"AttackEffect.h"
class Enemy;
class TargetMarker;

class Player : public ObjectManager
{
public:
	~Player() override;

public:
	static Player* Create(Model* model, DebugCamera* camera);
	static Player* GetInstance();

public:
	//初期化
	void Initialize(DebugCamera* camera) override;
	//更新処理
	void Update(DebugCamera* camera) override;
	//描画
	void Draw() override;

public:
	//受攻撃
	void RecvDamage(int Damage);
	//受攻撃後再度攻撃食らうまで
	void RecvDamage_Cool();

private:
	Input* input = Input::GetInstance();
	//fbxtime制御
	void FbxAnimationControl();
	//落下防止
	void ReturnGround();
	//ジャンプ
	void Jump();
	//移動処理まとめ
	void Move();

private:
	//回避フラグ
	bool evasionF = false;
	//イージング用
	float evaTime = 0.0f;
	//回避後の座標
	XMFLOAT3 EvaPos = {};
	//回避処理
	void Evasion();
public:
	//OBBの当たり判定に使うプレイヤーの回転行列取得
	XMMATRIX GetMatrot();
	//SelectSwordに使うプレイヤーの手のワールド行列
	XMMATRIX GetHanMat() { return m_fbxObject->GetHandBoneMatWorld(); }
	//座標の押し戻し
	void isOldPos()
	{
		Position.x = oldpos.x;
		Position.z = oldpos.z;
	}

	//プレーヤーの動き止める
	void SetStopFlag(bool f) { StopFlag = f; }
private:
	//プレイヤーが持つ剣
	std::unique_ptr<SwordBase> sword = nullptr;
	XMFLOAT3 SwordPos = {};
	XMFLOAT3 SwordRot = {};

private:
	//前座標
	XMFLOAT3 oldpos = {};
	//接地時間
	XMFLOAT3 onGroundPos = {};
	//体力周り
	int HP = 0;
	//最大体力
	const int MaxHP = 100;
	//
	int CoolTime = 0;
	//
	float angle;
	//teisi
	bool StopFlag;
	//
	int FallGroundTime = 0;
public:
	int GetHP() { return HP; }
	int GetMaxHP() { return MaxHP; }

private:
	float vel = 0.0f;
	bool jumpflag = false;
	float movespeed = 7.0f;
	XMFLOAT3 MoveVECTOR(DirectX::XMVECTOR v, float angle);
private:
	bool noAttack;

	XMVECTOR Gmove;
public:
	XMVECTOR GetPlayerMove() { return Gmove; }
	void Setangle(float angle) { this->angle = angle; }

	float GetFbxTime() { return f_time; }

	bool GetNowAttack() { return nowattack; }

	enum AttackMotion
	{
		NON,
		RUN,
		FIRST,
		SECOND,
		THIRD
	};

	AttackMotion GetAttackType() { return attackMotion; }
	AttackMotion GetAttackTypeOld() { return OldattackMotion; }
	void FbxAnimationControls(const AttackMotion& motiiontype, float attacktime = 0, float nextAnimation = 0);
	void SetnoAttack(bool f) { noAttack = f; }
	bool GetnoAttack() { return noAttack; }
private:
	/*ここから後まとめて消す*/
	AttackMotion OldattackMotion = NON;
	AttackMotion attackMotion = NON;
	bool AnimationEndJudg_FirstAttack = false;
	bool AnimationEndJudg_SecondAttack = false;
	bool AnimationEndJudg_ThirdAttack = false;

	int ComboCoolT = 0;
	bool SecAttack = false;
	bool nowSecAttack = false;
	bool AttackFlag = false;
	bool nowattack = false;
	//各アニメーションのスタート時間
	const float AttackThiTime = 238.00f / 60.00f;
	const float AttackSecTime = 138.00f / 60.00f;
	const float AttackFirTime = 35.00f / 60.00f;

	const float EvaTime_Start = 428.00f / 60.00f;
	const float EvaTime_End = 496.00f / 60.00f;

	const float DeathTime = 20.1f;

	float f_time = 0.0f;

	//手のボーンインデックス
	int hindex = 35;

public:
	float GetFbxTime_FirstAtack() { return AttackFirTime; }
	float GetFbxTime_SecondAtack() { return AttackSecTime; }
	float GetFbxTime_ThirdAtack() { return AttackThiTime; }
};
