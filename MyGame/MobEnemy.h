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
	void Initialize() override;

	//更新処理
	void Update() override;

	//描画処理
	void Draw() override;

	//死亡処理
	void Death() override;

	void Move() override;
	void Smoke(bool& createf) override;

	void DamageParticleSet();

	void EnemyHPDraw() override;
private:
	//攻撃後のクールタイム挙動
	void AttackCoolTime() override;

	void HPFrameInit() override;
	//Fbxのアニメーション制御
	void FbxAnimationControl() override;

	void FbxAnimeControl(const AnimationState& animestate, bool& Judg, float nowanimeTime, float nextanimetime);

	void OBBSetParam();

	void HPFrameScaling();
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

	int attackinter;
public:
	bool isendtime;
	std::unique_ptr<Object3d> Sword;
	XMFLOAT3 swordrot;
};
