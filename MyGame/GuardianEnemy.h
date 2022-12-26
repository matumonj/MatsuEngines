#pragma once
#include"Enemy.h"
#include"GuardianBomAttack.h"

class GuardianEnemy : public Enemy
{
public:
	/// コンストラクタ
/// </summary>
	GuardianEnemy();
	/// <summary>
	/// デストラクタ
	/// </summary> 
	~GuardianEnemy() override;

public:
	//初期化
	void Initialize() override;

	//更新処理
	void Update() override;

	//描画処理
	void Draw() override;

	//死亡処理
	void Death() override;

	void DamageParticleSet();

	void EnemyHPDraw() override;
private:
	//攻撃後のクールタイム挙動
	void AttackCoolTime() override;

	//Fbxのアニメーション制御
	void FbxAnimationControl() override;

	void OBBSetParam();

	void HPFrameScaling();

	void Move()override;
private:
	std::unique_ptr<GuardianBomAttack> bomattack;

	enum AttackType
	{
		BOM,
		NORMAL,
		RUSH,
	};

	float PosYMovingT;
	AttackType atcktype;
};
