#pragma once
#ifndef ENEMY_H_
#define ENMEY_H_

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>
#include"DirectXCommon.h"
#include"DebugCamera.h"
#include"Input.h"
#include"Player.h"
#include"EnemyState.h"
#include"Texture.h"
#include"Destroy.h"
class BehaviorTree;
#include"BehaviorDatas.h"
#include"ObjectManager.h"
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
	Enemy(BehaviorTree* ai_tree, float max_hp, float max_mp, float attack, float deffence);

	void init(BehaviorTree* ai_tree, float max_hp, float max_mp, float attack, float deffence);
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Enemy();

	static Enemy* Create(Model* model, DebugCamera* camera);

private:
	EnemyState* _state;
protected:
	Texture* SearchTex;
	float EnemyHP;
	float rotx;
	float rotz;
	XMVECTOR positionA;
	XMVECTOR positionB;
	XMVECTOR SubVector;
	bool Alive = true;
	int RandMove;
	int RandMovement;
protected:
	bool MoveFlag;
	int Number;
	bool animeflag;
private:
	
	int stime = 0;
	bool searchFlag;
	bool SearchNowFlag;
	bool searchMarkDrawFlag;
	bool RecvDamagef;
	float OldHP;
protected:
	bool FollowFlag;
	bool RecvDamageJudg;
public:

	bool GetRecvDamageJudg() { return RecvDamageJudg; }
	bool GetRecvDamage() { return RecvDamagef; }
	void SetRecvDamage(bool f) { RecvDamagef = f; }
	void SetSearchNow(bool f) { SearchNowFlag = f; }
	void SearchInit();
	void SearchAction(DebugCamera*camera);
	void SearchDraw();
	void SetRotTime(float t) { RotTime = t; }

	float GetRotTime() { return RotTime; }
	void SetMoveFlag(bool f) { MoveFlag = f; }
	bool GetMoveFlag() { return MoveFlag; }
	virtual bool GetDead() { if (EnemyHP <= 0) { return true; } }
	void RecvDamage(int Damage); 
	bool GetonFlag() { return onGround; }
	bool GetAlive() { return Alive; };
	float GetHP() { return EnemyHP; };
	void SetHP(float hp) { EnemyHP = hp; }
	float getrotc() { return rotx; }
	//void SetRandMove(int randMove, int movement) { RandMove = randMove; Movement = movement; }
	int GetRandMove() { return RandMove; }
	int GetRandMoveMent() { return RandMovement; }

	void Turn_toPlayer();
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize( DebugCamera* camera)override;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update(DebugCamera* camera)override;

	virtual void Death()=0;
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw()override;

	virtual void Action();
	void SetMovement(int movement) { this->movement = movement; }
	int GetMovement() { return movement; }
	void EnemyPop(int HP);
	void SetStartPosition(XMFLOAT3 position) {
		StartPosition = { 0,0,0}; }
	/// <summary>
	/// 解放処理
	/// </summary>
	virtual void Finalize();

	float distance;

	float GetDistance() { return distance; }

	void Getposition(float* x, float* y, float* z) {
		*x = this->Position.x;
		*y = this->Position.y;
		*z = this->Position.z;
	};

	XMFLOAT3 GetScale() { return Scale; }

	void SetScale(XMFLOAT3 scale) { Scale = scale; }

	bool GetDeath() { return DeathFlag; }
	int GetTime() { return time; }
	void Walk();
	void Stop();
	void Follow();
	bool GetSearchFlag() { return searchFlag; }
	bool endsearch;
	bool getendsearch() { return endsearch; }
	bool GetEndSearch() { return endsearch; }
	void Attack();
	void AttackCoolTime();
	int GetAttackCoolTime() { return cooltime; }
private:
	bool followFlag;
	int cooltime=0;
	bool AttackFlag;
	bool AfterAttack;
	XMVECTOR move;

	XMMATRIX matRot;
	bool searchTexSet;
	int onGroundTime = 0;
	int time=0;
	float RotY;
	float tempx, tempz;
	int movement;
protected:
	
	XMFLOAT3 StartPosition;
	int PopCount;
	private:

	bool wf = true;
	bool sf = false;
	float BeforeRot;
	float rot;

	float AfterRot;
	float RotTime = 0;
	int StayCount = 0;

	protected:

	bool DeathFlag;
protected:
	BehaviorTree* m_AiTree;			// ビヘイビアツリー
	BehaviorDatas* m_BehaviorData;	// ビヘイビアデータ
	NodeBase* m_ActiveNode;			// 実行中ノード

	public:
		enum EnemyStates {
			ALIVE,
			DEAD
		};
		EnemyStates GetState() { return state; }
	protected:
		EnemyStates state;
};


#endif