#pragma once
#include"Object3d.h"
#include<array>
#include<memory>
#include"CollisionPrimitive.h"
#include"Particle.h"
#include"BossAttackActionManager.h"
class RushAttack:public BossAttackActionManager
{

private:
	//ボスの火球
	std::unique_ptr<Object3d> RushSphereObj;
	//球パラメータ
	XMFLOAT3 rushspherescl;
	XMFLOAT3 rushspherepos;
	float rushspherealpha;
	std::array<float,5> rushEtime;

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
	static RushAttack* GetIns();
	void Init()override;
	void ActionJudg();
	void Upda()override;
	void Draw()override;
	void Finalize();
private:
	std::array<XMFLOAT3 ,5>rushimpactarea;

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
	void RushMoving();
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
