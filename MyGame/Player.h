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
	void Initialize() override;
	//更新処理
	void Update() override;
	//描画
	void Draw() override;

	void ParticleDraw();
public:
	//受攻撃
	void RecvDamage(int Damage);
	//受攻撃後再度攻撃食らうまで
	void RecvDamage_Cool();
	void ReStartSetParam();
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


	void Death();
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

	XMFLOAT3 GetHandPos()
	{
		return {
			m_fbxObject->GetHandBoneMatWorld().r[3].m128_f32[0],
			m_fbxObject->GetHandBoneMatWorld().r[3].m128_f32[1],
			m_fbxObject->GetHandBoneMatWorld().r[3].m128_f32[2]
		};
	}

	//座標の押し戻し
	void isOldPos()
	{
		Position.x = oldpos.x;
		//Position.y = oldpos.y;
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

public:
	void SetHP(int HP) { this->HP = HP; }
	int GetHP() { return HP; }
	int GetMaxHP() { return MaxHP; }
	bool GetStopFlag() { return StopFlag; };

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

	void SetnoAttack(bool f) { noAttack = f; }
	bool GetnoAttack() { return noAttack; }
private:
	/*ここから後まとめて消す*/
	AttackMotion OldattackMotion = NON;
	AttackMotion attackMotion = NON;
	bool AnimationEndJudg_FirstAttack = false;
	bool AnimationEndJudg_SecondAttack = false;
	bool AnimationEndJudg_ThirdAttack = false;

	bool onGround_Boss;

	int ComboCoolT = 0;
	bool SecAttack = false;
	bool nowSecAttack = false;
	bool AttackFlag = false;
	bool nowattack = false;

	float f_time = 0.0f;

	//手のボーンインデックス
	int hindex = 21;
	float RotY;
	int nearindex;
	Enemy* NearEnemy;

public:
	enum AnimeName
	{
		IDLE,
		RUNNING,
		ATTACK1,
		ATTACK2,
		ATTACK3,
		EVASION,
		DEATH
	};

	AttackMotion GetAnimeType() { return attackMotion; }
	void SetIdle(bool f) { idlef = f; }
private:
	AnimeName animationName;

	bool idlef;
	void AnimationContol(AnimeName name, int animenumber, double speed, bool loop);
	void FbxAnimationControls(const AttackMotion& motiiontype, AttackMotion nextmotiontype, AnimeName name, int number);
};
