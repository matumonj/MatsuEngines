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


private:
	//釘数
	static constexpr int NailSize = 10;

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
		float TexSclingEaseT;
		//座標周り
		XMFLOAT3 ObjPos;
		XMFLOAT3 TexPos;
		XMFLOAT3 TexScl;

		float TexRotY;
	};

	//釘位置の最大と最小値
	static constexpr float NailMaxPosY = 32.f;
	static constexpr float NailMinPosY = -20.f;

	//釘インス
	std::array<NailObjs, NailSize>nail_objses_;

	//攻撃のフェーズの関数テーブル
	static void (FrontCircleAttack::* actionTable[])();

	//ダメージ
	static constexpr int Damage = 8;
private:
	void NonInit();
	//範囲設定
	void SetDamageArea();
	//釘出てくる
	void PireNail();
	//釘引っ込む
	void DestNail();

	void AttackEnd();

	//釘との当たり判定
	bool ColNail_Player();
};
