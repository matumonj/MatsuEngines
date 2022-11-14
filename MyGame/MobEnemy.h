#pragma once
#include"Object3d.h"
#include"Model.h"
#include "Enemy.h"
#include"Player.h"
#include"Texture.h"
#include"Sprite.h"

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

	//攻撃後のクールタイム挙動
	void AttackCoolTime() override;

	//Fbxのアニメーション制御
	void FbxAnimationControl() override;

	void FbxAnimeControl(const AnimationState& animestate, bool& Judg, float nowanimeTime, float nextanimetime);
	void DamageParticleSet();

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

public:
	bool isendtime;
	std::unique_ptr<Object3d> Sword;
	XMFLOAT3 swordrot;
};
