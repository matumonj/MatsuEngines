#pragma once
#include"Object3d.h"
#include"Model.h"
#include "Enemy.h"
#include"Player.h"
#include"Texture.h"
#include"Sprite.h"
#include<array>
class MobEnemy :
	public Enemy
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	MobEnemy();
	/// <summary>
	/// デストラクタ
	/// </summary> 
	~MobEnemy() override;

public:
	//初期化
	void Initialize(DebugCamera* camera) override;

	//更新処理
	void Update(DebugCamera* camera) override;

	//描画処理
	void Draw() override;

	//死亡処理
	void Death() override;

	void DamageParticleSet();

	void EnemyHPDraw()override;
private:
	//攻撃後のクールタイム挙動
	void AttackCoolTime() override;

	//Fbxのアニメーション制御
	void FbxAnimationControl() override;

	void FbxAnimeControl(const AnimationState& animestate, bool& Judg, float nowanimeTime, float nextanimetime);

	void OBBSetParam();
private:
	enum AttackType
	{
		VERTICAL,
		SIDEAWAY
	};

	AttackType atcktype;
	float imt;
	float rand_Attacktype;
	int HandIndex;

	float NowFrameX;
	float OldFrameX;
	float FrameScalingETime;
	XMFLOAT2 FrameScl;
public:
	bool isendtime;
	std::unique_ptr<Object3d> Sword;
	std::array<std::unique_ptr<Sprite>, 3>HPFrame;
	XMFLOAT3 swordrot;
};
