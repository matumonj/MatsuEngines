#pragma once
#include"Texture.h"
#include"Object3d.h"
#include"Model.h"
#include<memory>
#include"CollisionPrimitive.h"
#include"ParticleManager.h"
#include"Particle.h"

class AltAttack
{
public:
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private:
	std::unique_ptr<Object3d> RushSphereObj;
	XMFLOAT3 rushspherescl;
	XMFLOAT3 rushspherepos;
	float rushspherealpha;
	float rushEtime[5];

	Particle* p;
private:
	Line2D beamRay[2];
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
	float RushEaseTime;
	float angleRush;
	int RushAttackCount = 0;
public:
	static AltAttack* GetInstance();
	void Initialize();
	void ActionJudg();
	void Upda();
	void Draw();
	void Finalize();
private:
	XMFLOAT3 rushimpactarea[5];

	Line2D DamageLine;
public:
	enum Area
	{
		FIR,
		SEC,
		THI,
		FIU,
		FIV,
		END
	} area;

private:
	bool atckjudg;
	XMFLOAT3 oldp[5];
	XMFLOAT3 rushpos;
	float DestTime;

	void Rush(Area& area, Area now, Area next, float& t);
	void RushAttack();
	void CollisionParamSet();

	void RushStart();
	void RushParamReset();

	void RushObjDraw();
public:
	XMFLOAT3 GetImpactArea(Area area) { return rushimpactarea[area]; }

	float GetFtime(Area area) { return rushEtime[area]; }
	Area GetNowArea() { return area; }
	void SetAttackPhase(bool f) { if (f && phase != PHASEONE) { phase = PHASEONE; } }
	Phase GetPhaseEnd() { return phase; }
};
