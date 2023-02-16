#pragma once
#include"Texture.h"
#include"Object3d.h"
#include"Model.h"
#include<memory>
#include"CollisionPrimitive.h"
#include"BossAttackActionManager.h"
#include<array>
class FrontCircleAttack : public BossAttackActionManager
{
public:
	static FrontCircleAttack* GetIns();
	void Init() override;
	void Upda() override;
	void Draw() override;
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
	static constexpr int NailSize = 9;

	struct NailObjs
	{
		std::unique_ptr<Texture> AOETexs;
		float TexAlpha;
		std::unique_ptr<Object3d>Obj;

		float PosYMovingEaseT;
		XMFLOAT3 ObjPos;
		XMFLOAT3 TexPos;
	};
	//�B�ʒu�̍ő�ƍŏ��l
	static constexpr float NailMaxPosY = 32.f;
	static constexpr float NailMinPosY = -20.f;

	//�B�C���X
	std::array<NailObjs, NailSize>nail_objses_;
	Phase phase;
	float TexAlpha;
public:
	void SetAttackPhase(bool f) { if (f && phase != PHASEONE) { phase = PHASEONE; } }
	Phase GetPhaseEnd() { return phase; }

private:
	//�͈͐ݒ�
	void SetDamageArea();
	//�B�o�Ă���
	void PireNail();
	//�B��������
	void DestNail();

	void AttackEnd();
};
