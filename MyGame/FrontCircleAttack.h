#pragma once
#include"Texture.h"
#include"Object3d.h"
#include"Model.h"
#include<memory>
#include"CollisionPrimitive.h"
#include"BossAttackActionManager.h"
#include<array>

#include "Particle.h"

class FrontCircleAttack : public BossAttackActionManager
{
public:
	static FrontCircleAttack* GetIns();
	void Init() override;
	void Upda() override;
	void Draw() override;
	void Finalize();

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
	//釘数
	static constexpr int NailSize = 9;

	struct NailObjs
	{
		//ダメージ範囲
		std::unique_ptr<Texture> AOETexs;
		//画像アルファ値
		float TexAlpha;
		//釘オブジェ
		std::unique_ptr<Object3d>Obj;
		//パーティクル
		std::unique_ptr<Particle>PireEffect;
		//突き出しのイージング用
		float PosYMovingEaseT;
		//座標周り
		XMFLOAT3 ObjPos;
		XMFLOAT3 TexPos;
	};

	//釘位置の最大と最小値
	static constexpr float NailMaxPosY = 32.f;
	static constexpr float NailMinPosY = -20.f;

	//釘インス
	std::array<NailObjs, NailSize>nail_objses_;
	//攻撃のフェーズ
	Phase phase;
	static void (FrontCircleAttack::* actionTable[])();
public:
	//フェーズセット
	void SetAttackPhase(bool f) { if (f && phase != PHASEONE) { phase = PHASEONE; } }
	Phase GetPhaseEnd() { return phase; }

private:
	void NonInit();
	//範囲設定
	void SetDamageArea();
	//釘出てくる
	void PireNail();
	//釘引っ込む
	void DestNail();

	void AttackEnd();
};
