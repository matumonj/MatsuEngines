#pragma once
#include"CollisionPrimitive.h"
#include "ObjectManager.h"

class SwordBase :
	public ObjectManager
{
public:
	SwordBase();
	virtual ~SwordBase() override;
public:
	void Initialize(DebugCamera* camera) override;
	void Update(DebugCamera* camera) override;
	void Draw() override;
	XMMATRIX GetMatrot() { return m_Object->ExtractRotationMat(); }
	XMMATRIX GetMatWorld() { return m_Object->GetMatWorld(); }
	float GetAnimationTime() { return animetiontime; }
	float GetKnockPower() { return KnockPower; }
protected:
	int Damage;
	int CoolTime;
	float animetiontime;
	float KnockPower;
public:
	int GetDamage() { return Damage; }
	int GetCoolTime() { return CoolTime; }
};
