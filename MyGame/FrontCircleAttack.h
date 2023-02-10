#pragma once
#include"Texture.h"
#include"Object3d.h"
#include"Model.h"
#include<memory>
#include"CollisionPrimitive.h"
#include"BossAttackActionManager.h"
class FrontCircleAttack: public BossAttackActionManager
{
public:
	static FrontCircleAttack* GetIns();
	void Init()override;
	void Upda()override;
	void Draw()override;
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
	bool SlamAttackF;
	Texture* FissureTex;
	Texture* DamageAreaTex;

	Phase phase;
	float TexAlpha;
public:
	void SetAttackPhase(bool f) { if (f && phase != PHASEONE) { phase = PHASEONE; } }
	Phase GetPhaseEnd() { return phase; }
};
