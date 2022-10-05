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
class NodeBase;
/// <summary>
/// 敵キャラの基底クラス
/// </summary>
/// 
class Enemy :public ObjectManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Enemy();

	static Enemy* Create(Model* model, DebugCamera* camera);

public:
	//初期化
	virtual void Initialize(DebugCamera* camera)override;
	//更新処理
	virtual void Update(DebugCamera* camera)override;
	//描画処理
	virtual void Draw()override;
	//死亡処理
	virtual void Death() = 0;
	//Fbx制御
	virtual void FbxAnimationControl() = 0;
	//
	virtual void Action();

	virtual void DamageTexUpdate(DebugCamera* camera)=0;

	void isRespawn();
protected:
	//体力周り
	float EnemyHP;//現在の体力
	float MaxHP;//最大値(変動なし)
	float OldHP;//前フレーム時の体力(HUDのイージング用)
protected:
	float alpha = 1.0f;
	XMFLOAT3 RespawnPos;
	bool RecvDamagef;//
	bool RecvAttackHit;
	bool DamageParticleCreateF;//攻撃受けた直後パーティクル発生フラg

private:
	bool MoveFlag = false;
	XMFLOAT3 OldPos;
public:
	//攻撃受けた直後の判定用
	void RecvDamage(int Damage);
	void EnemyPop(int HP);
	virtual void AttackCoolTime() = 0;

protected:
	bool f_AttackFlag;
	bool DeathFlag;
	bool nowDeath;
	int cooltime = 0;

	int onGroundTime = 0;
	bool AfterAttack;

	int PopCount;//リスポーンカウント

protected:
	//FBXTime周りの変数
	float f_time;//現在のフレーム
	float start_time;//初期フレーム(0)
	float end_time;//最終フレーム
	float DeathTime;//死亡時のモーション開始フレーム
	float AttackTime = 0;


	/*ゲッター*/
public:
	bool GetRecvAttack() { return RecvAttackHit;}
	void SetRecvAttack(bool f) { RecvAttackHit = f; }
	//攻撃受けた直後の判定用
	bool GetRecvDamage() { return RecvDamagef; }
	//歩いているか
	bool GetMoveFlag() { return MoveFlag; }
	//地面設置状態かどうか
	bool GetonFlag() { return onGround; }
	//体力周り
	float GetHP() { return EnemyHP; };
	float GetMaxHP() { return MaxHP; }
	//FBXTime(死にモーションと攻撃モーションの開始フレーム取得)
	bool GetDeathTime() { return DeathFlag; }
	bool GetAttackTime() { return f_AttackFlag; }
	//現在の経過フレーム
	float GetFbxTime() { return f_time; }
	//回転行列取得(OBB用いらないかも)
	XMMATRIX GetMatrot() { return m_fbxObject->GetMatrot(); }
	//攻撃のクールタイム
	int GetCoolTime() { return cooltime; }

	/*セッター*/
public:
	void SetRecvDamage(bool f) { RecvDamagef = f; }
	//
	void SetMoveFlag(bool f) { MoveFlag = f; }

	void SetScale(XMFLOAT3 scale) { Scale = scale; }

	void SetAttackTime(bool f) { if (f_time < AttackTime) { f_AttackFlag = f; } }

	void SetRespawnPos(XMFLOAT3 position) { RespawnPos = position; }
/*ボス攻撃用 できれば移したいが、、*/
protected://攻撃の開始と終了判定用
	struct Attack_SE {
		bool start;
		bool end;
	};
	//攻撃数
	static const int AtckNum = 7;
	Attack_SE Attack[AtckNum];

public:
	bool GetAttack_Start(int Num) { return Attack[Num].start; }
	bool GetAttack_End(int Num) { return Attack[Num].end; }

	void SetAttack_Start(int Num, bool f) { Attack[Num].start = f; }
	void SetAttack_End(int Num, bool f) { Attack[Num].end = f; }

public://state切り替え
	void ChangeState_Mob(EnemyState* state);
	void ChangeState_Boss(BossEnemyState* state);

	public://攻撃種類列挙
		enum {
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
	Texture* SlashTex;
	std::unique_ptr<DebugTxt>Damagetxt;
	XMFLOAT3 SlashRot;
	XMFLOAT3 SlashPos;
	float SlashAlpha = 1.0f;
protected:
	EnemyState* state_mob;
	BossEnemyState* state_boss;
	ParticleManager* particleMan = nullptr;
	ParticleManager* particleMan2 = nullptr;

};
