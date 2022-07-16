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
class NodeBase;
/// <summary>
/// 敵キャラの基底クラス
/// </summary>
/// 
class Enemy :public Object3d
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

protected:
	//Microosoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
private:
	EnemyState* _state;
protected:
	Texture* SearchTex;
	float EnemyHP;
	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	XMFLOAT3 Scale;
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
	//接地フラグ
	bool onGround = true;
	//落下ベクトル
	XMVECTOR fallV;
	int stime = 0;
	bool searchFlag;
	bool SearchNowFlag;
	bool searchMarkDrawFlag;
	bool RecvDamagef;
	float OldHP;
	
public:
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
	virtual void EnemySearchPlayer(Player* player);

	/// <summary>
	/// 初期化
	/// </summary>
	virtual bool Initialize( DebugCamera* camera)override;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update(XMFLOAT4 color,DebugCamera* camera)override;

	
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw();
	virtual void InitAction();
	virtual void Action();
	void SetMovement(int movement) { this->movement = movement; }
	int GetMovement() { return movement; }
	void EnemyPop(int HP);
	void SetStartPosition(XMFLOAT3 position) { StartPosition = position; }
	/// <summary>
	/// 解放処理
	/// </summary>
	virtual void Finalize();

	float distance;

	float GetDistance() { return distance; }

	virtual float Distance(Player* player);

	void SetEnemyPosition(XMFLOAT3 position) { this->position = position; }
	XMFLOAT3 GetEnemyPosition() { return position; }
	void Getposition(float* x, float* y, float* z) {
		*x = this->Position.x;
		*y = this->Position.y;
		*z = this->Position.z;
	};

	
	void SetPosition(XMFLOAT3 position) { Position = position; }

	XMFLOAT3 GetRotation() { return Rotation; }

	void SetRotation(XMFLOAT3 rotation) { Rotation = rotation; }

	XMFLOAT3 GetScale() { return Scale; }

	void SetScale(XMFLOAT3 scale) { Scale = scale; }

	virtual void CollisionField(DebugCamera* camera);
	virtual void ChangeState(EnemyState* newState);

	int GetTime() { return time; }
	void Walk();
	void Stop();
	void Follow();
	bool GetSearchFlag() { return searchFlag; }
	bool endsearch;
	bool getendsearch() { return endsearch; }
	bool State_Dead() {
		if (state == DEAD) { return true; }
		return false;
	}
	bool GetSearchPlayer_Distance();
	void Attack();
	int AttackCoolTime();
private:
	
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
	enum EnemyStates {
		ALIVE,
		DEAD
	};
	XMFLOAT3 StartPosition;
	int PopCount;
	EnemyStates state;
	private:

	bool wf = true;
	bool sf = false;
	float BeforeRot;
	float rot;

	float AfterRot;
	float RotTime = 0;
	int StayCount = 0;
protected:
	BehaviorTree* m_AiTree;			// ビヘイビアツリー
	BehaviorDatas* m_BehaviorData;	// ビヘイビアデータ
	NodeBase* m_ActiveNode;			// 実行中ノード
};


#endif