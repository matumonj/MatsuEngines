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
	void Initialize(DebugCamera* camera) override;
	void Update(DebugCamera* camera) override;
	void Draw() override;
	XMMATRIX GetMatrot() { return m_Object->ExtractRotationMat(); }
	XMMATRIX GetMatWorld() { return m_Object->GetMatWorld(); }
protected:
	int Damage;
	int CoolTime;
public:
	int GetDamage() { return Damage; }
	int GetCoolTime() { return CoolTime; }
};
