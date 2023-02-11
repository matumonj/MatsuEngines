#pragma once
#include <DirectXMath.h>
#include"Player.h"
#include"EnemyState.h"
#include"BossEnemyState.h"
#include"GuardianState.h"
#include"Destroy.h"
#include"ObjectManager.h"
#include"DebugTxt.h"
#include<list>
#include"Task.h"

#include"FbxLoader.h"
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
	void Initialize() override =0;
	//更新処理
	void Update() override =0;
	//描画処理
	void Draw() override =0;
	//死亡処理
	virtual void Death() = 0;
	//Fbx制御
	virtual void FbxAnimationControl() = 0;
	//
	virtual void Action();

	void isRespawn();

	void DestroyJudg();

	virtual void Move() = 0;

	virtual void Smoke(bool& createf) =0;

	virtual void EnemyHPDraw() =0;

public:
	void Respawn();
	int GetRespawnCount() { return RespawnCount; }
	void ResetRespawnCount() { RespawnCount = 0; }
	bool RespawnJudg(); //

protected:
	bool isAlive;
	//体力周り
	int EnemyHP = 0; //現在の体力
	int MaxHP = 0; //最大値(変動なし)
	float OldHP = 0.0f; //前フレーム時の体力(HUDのイージング用)
protected:
	//オブジェのアルファ値
	float alpha = 1.0f;
	//リスポーン地点
	XMFLOAT3 RespawnPos = {0.0f, 0.0f, 0.0f};
	bool RecvDamagef = false; //HUD用
	bool RecvDamagef2 = false; //挙動判定用
	bool RecvAttackHit = false; //
	bool DamageParticleCreateF = false; //攻撃受けた直後パーティクル発生フラg
private:
	//動けるかどうか
	bool MoveFlag = false;
	//１フレーム前の座標
	XMFLOAT3 OldPos = {0.0f, 0.0f, 0.0f};
	int DamageSize = 0;
	XMFLOAT3 DamageTexPos = {};
public:
	//攻撃受けた直後の判定用
	void RecvDamage(int Damage);

	virtual void AttackCoolTime() = 0;
	void SetMoveStop(bool f) { movestop = f; }
	bool GetMoveStop() { return movestop; }
	bool GetisAlive() { return isAlive; }
	void SetisAlive(bool f) { isAlive = f; }
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

	int RespawnCount = 0; //リスポーンカウント

	int RespawnCountMax;
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

	AnimationState GetAnimeState() { return animeState; }
	void SetAnimeState(AnimationState state) { animeState = state; }
	int GetEnemyNumber() { return ENumber; }
protected:
	/*blenderの複数アニメーションで来たら全部消す*/
	AnimationState animeState;
	//敵の回転調整用
	float addRotRadians;
	float FollowRotAngleCorrect;

	//各アニメーションの開始タイム

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
	int GetHP() { return EnemyHP; };
	//体力最大値
	int GetMaxHP() { return MaxHP; }
	//FBXTime(死にモーションと攻撃モーションの開始フレーム取得)
	bool GetDeathTime() { return nowDeath; }

	bool GetAttackTime() { return f_AttackFlag; }
	//現在の経過フレーム
	float GetFbxTime() { return f_time; }
	//回転行列取得(OBB用いらないかも)
	XMMATRIX GetMatrot() { return m_fbxObject->ExtractRotationMat(m_fbxObject->GetWorld()); }
	XMMATRIX GetMatrotObj() { return m_Object->GetMatrot(); }
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

	void SetRespawnCountMax(int count) { RespawnCountMax = count; }

	void SetEColor(XMFLOAT4 color) { Color = color; }
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
		MINIGOLEM,
		GUARDIAN
	};


	void SetAnimationNum(int number) { m_Number = number; }
protected:
	int ENumber;
	//攻撃数
	static const int AtckNum = 8;
	Attack_SE Attack[AtckNum];

public:
	bool getdie() { return DieFlag; }
	bool getdeath() { return DeathFlag; }


	void SetAnimation(int number, bool loop, double speed);
	bool GetAttack_Start(int Num) { return Attack[Num].start; }
	bool GetAttack_End(int Num) { return Attack[Num].end; }

	void SetAttack_Start(int Num, bool f) { Attack[Num].start = f; }
	void SetAttack_End(int Num, bool f) { Attack[Num].end = f; }

	void SetHP(int HP) { this->EnemyHP = HP; }
public: //state切り替え
	void ChangeState_Mob(EnemyState* state);
	void ChangeState_Boss(BossEnemyState* state);
	void ChangeState_Guardian(GuardianState* state);

	enum NowAttackMotion
	{
		SHILEDWALK_FRONT=0,
		SHIELDWALK_SIDE=1,
		BIDLE=11,
		BROAR=8,
		BNORMAL=12,
		BNORMAL2 = 15,
		MAGIC=13,
		SWING=7,
		EVASION=3,
		FALTER=10,
		BWALK=14,
		BDEATH=9,
		CHARGE=2,
		THROW=4,
		BJUMP=5
	};

	int GetNowMotion() { return m_Number; }
protected:
	NowAttackMotion attackNum = BIDLE;
	OBB ShieldOBB;
	bool GuardAction;
	int guardtime;
	int GuarPoint;
public:
	OBB GetShieldOBB() { return ShieldOBB; }
	bool GetGuardAction() { return GuardAction; }
	void SetGuardAction(bool f) { GuardAction = f; }
	int GetGuardTime() { return guardtime; }
	void SetGuardTime(int f) { guardtime = f; }
	void SetGuardPointAdd() { GuarPoint++; }

	int GetGuardPoint() { return GuarPoint; };
public: //攻撃種類列挙
	enum
	{
		CIRCLE_1,
		CIRCLE_2,
		KNOCK,
		HALF_1,
		ULT,
		Slam,
		Beam,
		BRONZEATTACK_W,
		BRONZEATTACK_H
	};

	/*/////	ここまで////////*/


protected:
	std::unique_ptr<DebugTxt> Damagetxt;

	bool SearchF;
	XMFLOAT3 EaseTime_Arrow;
protected:
	GuardianState* state_guardian;
	EnemyState* state_mob;
	BossEnemyState* state_boss;
protected:
	//両手の行列
	XMMATRIX handmat_right;
	XMMATRIX handmat_right2;
	XMMATRIX handmat_left;

	//両手の座標
	XMFLOAT3 HandPos_Right; //18
	XMFLOAT3 HandPos_Right2; //18
	XMFLOAT3 HandPos_Left; //43

	float rotadds;

	bool HelpJudg;

protected:
	std::unique_ptr<Sprite> HelpIcon;
	float HelpIconAlpha;
public:
	void HelpIconInit();
	void HelpIconShow();
	void HelpIconDraw();
	void HelpAction();
	void SetHelpIcon(bool f) { HelpJudg = f; }
	bool GetHelpJudg() { return HelpJudg; }
	float Getaddroa() { return rotadds; }
	XMFLOAT3 HandRightPos() { return HandPos_Right; }
	XMFLOAT3 HandRightPos2() { return HandPos_Right2; }
	XMFLOAT3 HandLeftPos() { return HandPos_Left; }
protected:
	std::array<std::unique_ptr<Sprite>, 4> HPFrame;
	std::unique_ptr<Sprite> EnemyName;
	float NowFrameX;
	float OldFrameX;

	float OldFrameX_Inner;
	float FrameScalingETime;


	bool InnerFrameScalingF;
	float FrameScalingETime_Inner;
	XMFLOAT2 FrameScl;
	XMFLOAT2 FrameScl_Inner;

protected:
	virtual void HPFrameInit() =0;
	void HPFrameUpda();
private:
	std::list<std::unique_ptr<DamageManager>> dMans_;
protected:
	std::unique_ptr<Object3d> ShieldObj;
	XMFLOAT3 shieldRot;

public:
	XMMATRIX GetMatWol() { return ShieldObj->ExtractRotationMat(); }
	XMFLOAT3 GetShieldPos() { return HandPos_Right; }
	XMFLOAT3 GetShieldRot() { return ShieldObj->GetRotation(); }
};
