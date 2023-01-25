#pragma once
#include"Texture.h"
#include"Object3d.h"
#include<memory>
#include"Sprite.h"
#include"BossAttackActionManager.h"
class CircleAttack:public BossAttackActionManager
{
public:
	~CircleAttack();

public:
	enum
	{
		NORTH,
		SOUTH,
		EAST,
		WEST
	};

private:
	float rotY;
	static const int DirectionNum = 4;
	XMFLOAT3 Direction[DirectionNum];

	std::vector<std::unique_ptr<Object3d>> NailObj;
	Model* NailModel;
	Texture* ImpactAreaTex;
	float TexAlpha = 1;
	bool phase1, phase2, phase3, phase4;
	int AttackCount;
	float CircleAreaTime = 0;
	XMFLOAT2 CircleSize = {0, 0};

	int Area1, Area2;
	bool NailAttackFlag;
public:
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
public:
	static CircleAttack* GetInstance();
	void Init()override;
	void Upda()override;
	void Draw()override;

private:
	void PierceNail();
	void DamageAreaTexSet();
	void ProtrudeNail();
	void ImpactAttack();
	void EndAttackAction();
public:
	int GetDamageArea1() { return Area1; }
	int GetDamageArea2() { return Area2; }

	void SetDamageArea(int area1, int area2)
	{
		Area1 = area1;
		Area2 = area2;
	};

	bool GetNailMove() { return NailAttackFlag; }

	void SetAttackPhase(bool f) { if (f && phase != PHASEONE) { phase = PHASEONE; } }
	Phase GetPhaseEnd() { return phase; }

private:
	void CollisonNailPlayer();
};
