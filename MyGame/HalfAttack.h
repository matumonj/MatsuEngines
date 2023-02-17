#pragma once
#include"Texture.h"
#include<array>

#include "Enemy.h"
#include"BossAttackActionManager.h"

class HalfAttack : public BossAttackActionManager
{
public:
	~HalfAttack();
private:
	bool ShieldCreate;
	bool ShieldLost;
	std::unique_ptr<Texture> ShieldTex[2];

	std::array<XMFLOAT3, 2> ShieldTexPos;
	float ShieldRot;
	float ShieldTexAlpha;
private:
	static constexpr int EnemySize = 2;
	std::array<std::unique_ptr<Enemy>, EnemySize> SummonEnemys = {};
	bool summonEnemyCreate = false;
	XMFLOAT3 SummonEPos = {1, 1, 1};
	bool SummonEnemysDeath;
	bool SummonEnemysApper;
	float Shieldalpha;
public:
	bool GetSummonEnemysDeath() { return SummonEnemysDeath; }

	bool GetSummonEnemysApper() { return SummonEnemysApper; }
	Enemy* GetSummonEnemy(int index) { return SummonEnemys[index].get(); }

public:
	enum Phase
	{
		PHASENON,
		PHASEONE,
		PHASETWO,
		PHASETHREE,
		PHASEFOUR
	};

private:
	Phase phase;

	void SummonInit();
	void BossLeaveGround();
	void SummonUpdate();
	void BossReturnGround();
	void SummonAttackEnd();
public:
	static HalfAttack* GetIns();

	void Init() override;
	void Draw() override;
	void Upda() override;
public:
	void Draw_SummonEnemyHP();
	bool SummonEnemy();
	void SummonEnemyResetParam();
	void SetAttackPhase(bool f) { if (f && phase != PHASEONE) { } }
	Phase GetPhase() { return phase; }

	static void (HalfAttack::* actionTable[])();
	void SetSummonF(bool f) { summonEnemyCreate = f; }
};
