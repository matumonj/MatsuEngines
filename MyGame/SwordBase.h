#pragma once
#include"CollisionPrimitive.h"
#include "ObjectManager.h"

class SwordBase :
	public ObjectManager
{
public:
	SwordBase();
	~SwordBase() override;
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	XMMATRIX GetMatrot() { return m_Object->ExtractRotationMat(); }
	XMMATRIX GetMatWorld() { return m_Object->GetMatWorld(); }
	float GetAnimationTime() { return animetiontime; }
	float GetKnockPower() { return KnockPower; }

	XMFLOAT3 GetSwordObbScl() { return SwordObbScl; }
protected:
	int Damage;
	int CoolTime;
	float animetiontime;
	float KnockPower;
	XMFLOAT3 SwordObbScl;

public:
	int GetDamage() { return Damage; }
	int GetCoolTime() { return CoolTime; }
};
