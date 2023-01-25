#pragma once
#include "BossAttackActionManager.h"
#include "Enemy.h"
#include"Object3d.h"
#include "Particle.h"
#include"Texture.h"
class ThrowRockAttack:public BossAttackActionManager
{
public:
	static ThrowRockAttack*GetInstance();

private:
	//岩おぶじぇ
	std::unique_ptr<Object3d> ThrowRockObj;
	//土煙
	std::unique_ptr<Particle> DestPar;
	//岩座標
	XMFLOAT3 RockPos;

	//砕ける瞬間フラグ
	bool destF;
	bool EffectCreateF;
	//砕けるエフェクトサイズ
	static constexpr int EffectSize = 5;

	std::array<std::unique_ptr<Object3d>,EffectSize> DestRock;
	std::array<float, EffectSize> DestRockAlpha;
	std::array<float, EffectSize> DestRockAcc;
	std::array<XMFLOAT3, EffectSize> DestRockPos;

	//投石描画フラグ
	bool TurnoffDrawF;
	
	DebugCamera* camera;
public:
	void Init()override;
	void Upda()override;
	void Draw()override;

	//投げる動作の流れ
	enum ThrowMotion
	{
		NON,
		SET,
		PICK,
		THROW,
		END
	} tmotion;

	//フェーズを初期状態に
	void SetAction(bool f) { if (tmotion != SET) { tmotion = SET; } }

	ThrowMotion GetPhase() { return tmotion; }
private:
	void ThrowPhase_Set();
	void ThrowPhase_Pick();
	void ThrowPhase_Throw();
	void ThrowPhase_End();

	void DestEffect();
	void ActionUpda();

private:
	static void (ThrowRockAttack::* actionTable[])();
};
