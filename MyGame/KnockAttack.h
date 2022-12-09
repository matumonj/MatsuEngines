#pragma once
#include"Texture.h"
#include"Input.h"
#include<array>
#include<memory>
#include"Object3d.h"
#include"Particle.h"
#include"CollisionPrimitive.h"
class KnockAttack
{
public:
	~KnockAttack();
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
private:
	//Texture* KnockTex;
	static constexpr int axeSize = 4;
	std::array<std::unique_ptr<Object3d>,axeSize>AxeObj;
	std::array<XMFLOAT3, axeSize>AxePos;
	std::array<XMFLOAT3, axeSize>AxeRot;
	std::array<XMFLOAT3, axeSize>SetPos;
	std::array<std::unique_ptr<Particle>, axeSize>ImpactPar;
	float AxePosDownEtime;
	int AttackCount;
	Line2D damageLine[axeSize];
	XMVECTOR move[axeSize];
	XMMATRIX matRot[axeSize];
	
public:
	static KnockAttack* GetInstance();
	void Initialize();
	void Draw();
	void ActionJudg();

public:
	enum Phase
	{
		PHASENON,
		PHASEONE,
		PHASETWO,
		PHASETHREE,
		PHASEFOUR
	};

	Phase GetPhase() { return phase; }
	void SetAttackPhase(bool f) { if (f && phase != PHASEONE) { phase = PHASEONE; } }

private:
	Phase phase;
};
