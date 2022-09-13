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
protected:
	float EnemyHP;
	float MaxHP;
	float OldHP;
protected:
	bool MoveFlag = false;
	float AttackTime = 0;
	bool RecvDamagef;
	bool DamageParticleCreateF;
private:
	XMFLOAT3 OldPos;
public:
	bool GetRecvDamage() { return RecvDamagef; }
	void SetRecvDamage(bool f) { RecvDamagef = f; }
	void SetMoveFlag(bool f) { MoveFlag = f; }
	bool GetMoveFlag() { return MoveFlag; }
	void RecvDamage(int Damage);
	bool GetonFlag() { return onGround; }
	float GetHP() { return EnemyHP; };
	float GetMaxHP() { return MaxHP; }

	void Turn_toPlayer();

	void EnemyPop(int HP);

	bool AfterAttack;


	XMFLOAT3 GetScale() { return Scale; }

	void SetScale(XMFLOAT3 scale) { Scale = scale; }

	bool GetDeathTime() { return DeathFlag; }

	virtual void AttackCoolTime() = 0;
	int GetAttackCoolTime() { return cooltime; }
	bool f_AttackFlag;
	void SetAttackTime(bool f) { if (f_time < AttackTime) { f_AttackFlag = f; } }
	bool GetAttackTime() { return f_AttackFlag; }

	float GetFbxTime() { return f_time; }
protected:
	bool DeathFlag;
	bool nowDeath;
	int cooltime = 0;

	int onGroundTime = 0;
protected:
	float f_time;
	float start_time;
	float end_time;
	float DeathTime;
	XMFLOAT3 StartPosition;
	int PopCount;

protected:
	struct Attack_SE {
		bool start;
		bool end;
	};
	static const int AtckNum = 5;
	Attack_SE Attack[AtckNum];

public:
	bool GetAttack_Start(int Num) { return Attack[Num].start; }
	bool GetAttack_End(int Num) { return Attack[Num].end; }

	void SetAttack_Start(int Num, bool f) { Attack[Num].start = f; }
	void SetAttack_End(int Num, bool f) { Attack[Num].end = f; }

	bool GetNowDeath() { return nowDeath; }
	int GetCoolTime() { return cooltime; }
public:
	void ChangeState_Mob(EnemyState* state);
	void ChangeState_Boss(BossEnemyState* state);

public:

	enum {
		CIRCLE_1,
		CIRCLE_2,
		KNOCK,
		HALF_1,
		HALF_2
	};
protected:
	EnemyState* state_mob;
	BossEnemyState* state_boss;
	ParticleManager* particleMan = nullptr;

};
