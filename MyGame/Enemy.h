#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include"DebugCamera.h"
#include"Input.h"
#include"Player.h"
#include"EnemyState.h"
#include"BossEnemyState.h"
#include"Texture.h"
#include"Destroy.h"
#include"ObjectManager.h"
#include"ParticleManager.h"
#include"DebugTxt.h"
#include<list>
#include"Task.h"
#include"DamageManager.h"
class NodeBase;

/// <summary>
/// 敵キャラの基底クラス
/// </summary>
/// 
class Enemy : public ObjectManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy() override;

public:
	//初期化
	virtual void Initialize(DebugCamera* camera) override=0;
	//更新処理
	virtual void Update(DebugCamera* camera) override=0;
	//描画処理
	virtual void Draw() override=0;
	//死亡処理
	virtual void Death() = 0;
	//Fbx制御
	virtual void FbxAnimationControl() = 0;
	//
	virtual void Action();

	void isRespawn();

	void DestroyJudg();
protected:
	//体力周り
	float EnemyHP = 0.0f; //現在の体力
	float MaxHP = 0.0f; //最大値(変動なし)
	float OldHP = 0.0f; //前フレーム時の体力(HUDのイージング用)
protected:
	float alpha = 1.0f;
	XMFLOAT3 RespawnPos = { 0.0f, 0.0f, 0.0f };
	bool RecvDamagef = false; //
	bool RecvDamagef2 = false; //
	bool RecvAttackHit = false;
	bool DamageParticleCreateF = false; //攻撃受けた直後パーティクル発生フラg
private:
	bool MoveFlag = false;
	XMFLOAT3 OldPos = { 0.0f, 0.0f, 0.0f };
	int DamageSize = 0;
	XMFLOAT3 DamageTexPos = {};
public:
	//攻撃受けた直後の判定用
	void RecvDamage(int Damage);
	void EnemyPop(int HP);
	virtual void AttackCoolTime() = 0;
	void SetMoveStop(bool f) { movestop = f; }
	bool GetMoveStop() { return movestop; }
	void DamageTexDisplay();
	void DamageTexDisplay_Draw();
protected:
	bool f_AttackFlag = false;
	bool DeathFlag = false;
	bool nowDeath = false;
	bool nowAttack = false;
	bool FalterFlag = false;
	bool SwingFlag = false;
	bool DieFlag = false;
	int cooltime = 0;

	int onGroundTime = 0;
	bool AfterAttack = false;

	int PopCount = 0; //リスポーンカウント

	bool movestop = false;
protected:
	//FBXTime周りの変数
	float f_time = 0.0f; //現在のフレーム
	float start_time = 0.0f; //初期フレーム(0)
	float end_time = 0.0f; //最終フレーム
	float DeathTime = 0.0f; //死亡時のモーション開始フレーム
	float NormalAttackTime = 0.0f;

	XMMATRIX HandMat = {};
	OBB HandSiteOBB = {};
	OBB playerOBB = {};

	bool damageDisF = false;
public:
	enum AnimationState
	{
		NON,
		WALK,
		ATTACK1,
		ATTACK2,
		ATTACK3,
		DEATH,
		ROAR,
		IDLE
	};

	void SetAnimeState(AnimationState state) { animeState = state; }
	int GetEnemyNumber() { return ENumber; }
protected:
	/*blenderの複数アニメーションで来たら全部消す*/
	AnimationState animeState;
	//敵の回転調整用
	float addRotRadians;
	float FollowRotAngleCorrect;

	//各アニメーションの開始タイム
	float MagicAttackTime = 0.0f;
	float FalterTime = 0.0f;
	float FalterTime_End;
	float RoarTime;
	float RoarTime_End;
	float SwingTime;
	float SwingTime_End;
	float IdleTime;
	float IdleTime_End;
	float SideWalk_LeftTime;
	float SideWalk_LeftTime_End;
	float SideWalk_RightTime;
	float SideWalk_RightTime_End;

	float DeathMotionTime_Start;
	float DeathMotionTime_End;
	float EvaTime = 0.0f;
	float AttackTime;
	float AttackTime_End;

	bool EvaMotionStart = false;
	bool MagicMotionStart = false;
	bool RoarMotionFlag = false;
	bool IdleMotionFlag = false;
	bool DeathMotionFlag = false;
	bool SideWalk_RightMotionFlag = false;
	bool SideWalk_LeftMotionFlag = false;
	/*ゲッター*/
public:
	float GetRotCorrect() { return FollowRotAngleCorrect; }
	bool GetRecvAttack() { return RecvAttackHit; }
	void SetRecvAttack(bool f) { RecvAttackHit = f; }
	//攻撃受けた直後の判定用
	bool GetRecvDamage() { return RecvDamagef; }
	//攻撃受けた直後の判定用
	bool GetRecvDamage2() { return RecvDamagef2; }
	//歩いているか
	bool GetMoveFlag() { return MoveFlag; }
	//地面設置状態かどうか
	bool GetonFlag() { return onGround; }
	//体力周り
	float GetHP() { return EnemyHP; };
	//体力最大値
	float GetMaxHP() { return MaxHP; }
	//FBXTime(死にモーションと攻撃モーションの開始フレーム取得)
	bool GetDeathTime() { return nowDeath; }

	bool GetAttackTime() { return f_AttackFlag; }
	//現在の経過フレーム
	float GetFbxTime() { return f_time; }
	//回転行列取得(OBB用いらないかも)
	XMMATRIX GetMatrot() { return m_fbxObject->GetMatrot(); }
	//攻撃のクールタイム
	int GetCoolTime() { return cooltime; }

	XMMATRIX GetHandMat() { return HandMat; }
	OBB GetHandSiteMat() { return HandSiteOBB; }

	float GetObjAlpha() { return alpha; }
	/*セッター*/
public:
	float GetRotRadians() { return addRotRadians; }
	void SetRecvDamage(bool f) { RecvDamagef = f; }

	void SetRecvDamage2(bool f) { RecvDamagef2 = f; }
	void SetMoveFlag(bool f) { MoveFlag = f; }

	void SetScale(XMFLOAT3 scale) { Scale = scale; }

	void SetAttackTime(bool f) { if (f_time < NormalAttackTime) { f_AttackFlag = f; } }

	void SetRespawnPos(XMFLOAT3 position) { RespawnPos = position; }
	/*ボス攻撃用 できれば移したいが、、*/
protected: //攻撃の開始と終了判定用
	struct Attack_SE
	{
		bool start;
		bool end;
	};
public:
	enum EnemyNumber
	{
		GOLEM,
		FLOG,
	};
protected:
	int ENumber;
	//攻撃数
	static const int AtckNum = 7;
	Attack_SE Attack[AtckNum];

public:
	void SetSwingMotion(bool f) { if (f_time < SwingTime) { SwingFlag = f; } }

	void SetFalterMotion(bool f) { if (f_time < FalterTime) { FalterFlag = f; } }
	void SetRoarMotion(bool f) { if (f_time > RoarTime_End) { RoarMotionFlag = f; } }
	void SetDeathMotion(bool f) { if (f_time < DeathMotionTime_Start) { DieFlag = f; } }
	//
	void SetRSideWalkMotion(bool f) { if (f_time > SideWalk_RightTime) { SideWalk_RightMotionFlag = f; } }
	void SetLSideWalkMotion(bool f) { if (f_time < SideWalk_LeftTime) { SideWalk_LeftMotionFlag = f; } }

	//
	void SetIdleMotion(bool f) { if (f_time < IdleTime) { IdleMotionFlag = f; } }
	float GetFalterTime_End() { return FalterTime_End; }
	float GetRoarTime_End() { return RoarTime_End; }
	void SetMagicAttackTime(bool f) { if (f_time < MagicAttackTime) { MagicMotionStart = f; } }
	void SetEvaMotionTime(bool f) { if (f_time < EvaTime) { EvaMotionStart = f; } }

	bool GetAttack_Start(int Num) { return Attack[Num].start; }
	bool GetAttack_End(int Num) { return Attack[Num].end; }

	void SetAttack_Start(int Num, bool f) { Attack[Num].start = f; }
	void SetAttack_End(int Num, bool f) { Attack[Num].end = f; }

	void SetHP(int HP) { this->EnemyHP = HP; }
public: //state切り替え
	void ChangeState_Mob(EnemyState* state);
	void ChangeState_Boss(BossEnemyState* state);

public: //攻撃種類列挙
	enum
	{
		CIRCLE_1,
		CIRCLE_2,
		KNOCK,
		HALF_1,
		HALF_2,
		Slam,
		Beam
	};

	/*/////	ここまで////////*/


protected:
	std::unique_ptr<DebugTxt> Damagetxt;

	bool SearchF;
	XMFLOAT3 EaseTime_Arrow;
protected:
	EnemyState* state_mob;
	BossEnemyState* state_boss;

private:
	std::list<std::unique_ptr<DamageManager>> dMans_;

};
