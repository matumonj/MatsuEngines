#pragma once
#include "Texture.h"
#include<memory>
#include"Particle.h"
#include"Object3d.h"
#include"BossAttackActionManager.h"
#include<array>
class BronzeAttack:public BossAttackActionManager
{
public:
	static BronzeAttack* GetIns();
	void Init()override;
	void Upda()override;
	void Draw()override;

	void Finalize();

public:
	//�U������
	enum Direction
	{
		HEIGHT,
		WIDTH
	}AttackDir;

	//�U���t�F�[�Y�������p
	void SetAction(bool f,Direction dir) { if (phase != AREASET) { phase = AREASET; AttackDir=dir;} }

	//�t�F�[�Y�̏I������
	bool GetPhaseEnd()
	{
		if (phase == END) { return true; }
		return false;
	}

	bool GetisEndAttack() { return isEndAttack; }
	void SetisEndAttack(bool f) { isEndAttack = f; }

public:
	enum Phase
	{
		NON,
		AREASET,
		BOM,
		BEAMSMALL,
		TEXFADE,
		END
	} phase;

	Phase GetPhase() { return phase; }
private:
	

	void Phase_AreaSet();
	void Phase_Bom();
	void Phase_MakeSmall();
	void Phase_TexFade();
	void Phase_End();

	void ObjUpda();
	void SphereMoving();
private:
	std::array<std::unique_ptr<Texture>,2> MagicTex;
	std::unique_ptr<Particle>ChargeCenter;
	std::array<std::unique_ptr<Object3d>,5> BeamObj;
	//�с[�ނ̊e�p�����[�^
	std::array < XMFLOAT3, 5> BeamObjPos;
	std::array < XMFLOAT3, 5> BeamObjScl;
	std::array < XMFLOAT3, 5> BeamMaxScl;

	static constexpr int spheresize = 30;
	std::array<std::unique_ptr<Object3d>, spheresize>chargesphere;
	std::array<float, spheresize>chargesphereMovingE;
	std::array<bool, spheresize>chargespheremoveF;
	std::array<XMFLOAT3, spheresize>chargespherepos;
	std::array<float,5> scalingETime;
	XMFLOAT2 TexScl;
	std::array<XMFLOAT3,2> TexPos;
	float TexAlpha;
	float TexRotZ;
	//boss�̍��W
	XMFLOAT3 BossRot;
	XMFLOAT3 BossPos;
	XMFLOAT3 BossColor;

	//�A���t�@�l
	float ObjAlpha;
	float ColorT;

	bool isEndAttack;
	
};

