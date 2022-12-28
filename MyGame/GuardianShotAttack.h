#pragma once
#include "Object3d.h"
#include<memory>
#include<array>
#include"Particle.h"
#include"Sprite.h"

class GuardianShotAttack
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	static GuardianShotAttack* GetIns();
	void TexSet();
	void Upda();
	void Draw();

	void SetAction(bool f) { if (phase == NON) { phase = AREASET; } }

	bool GetPhaseEnd()
	{
		if (phase == END) { return true; }
		return false;
	}

	void SetActionNon() { phase = NON; }

	bool GetisEndAttack() { return isEndAttack; }
	void SetisEndAttack(bool f) { isEndAttack = f; }
private:
	static constexpr int ArmObjNum = 5;

	enum Phase
	{
		NON,
		AREASET,
		BOM,
		END
	} phase;

	void Phase_AreaSet();
	void Phase_Bom();
	void Phase_End();

	void ArmShot();
	void ColPlayer();
	void DestroyEffect();
private:
	std::array<std::unique_ptr<Particle>, ArmObjNum> ArmEffect;
	std::array<std::unique_ptr<Object3d>, ArmObjNum> ArmObj;
	std::array<XMFLOAT3, ArmObjNum> ArmRot;
	std::array<XMFLOAT3, ArmObjNum> ArmPos;
	std::array<bool, ArmObjNum> ArmShotF;
	std::array<float, ArmObjNum> ArmShotSpeed;
	std::array<float, ArmObjNum> ArmAlpha;
	std::unique_ptr<Texture> DamageTex;
	std::unique_ptr<Particle> PlayerDamageEffect;
	int ShotCount = 0;
	XMFLOAT2 TexScl;
	float TexAlpha;

	XMFLOAT3 BossRot;
	XMFLOAT3 BossPos;
	XMFLOAT3 BossColor;

	float ColorT;
	//攻撃終了
	bool isEndAttack;
	//爆発パーティクル
	bool DestroyEffectF;
	//当たったミサイルの座標保存用
	XMFLOAT3 ColMissilePos;
};
