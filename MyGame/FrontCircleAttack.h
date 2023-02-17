#pragma once
#include"Texture.h"
#include"Object3d.h"
#include"Model.h"
#include<memory>
#include"CollisionPrimitive.h"
#include"BossAttackActionManager.h"
#include<array>

#include "Particle.h"

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
	//�B��
	static constexpr int NailSize = 9;

	struct NailObjs
	{
		//�_���[�W�͈�
		std::unique_ptr<Texture> AOETexs;
		//�摜�A���t�@�l
		float TexAlpha;
		//�B�I�u�W�F
		std::unique_ptr<Object3d>Obj;
		//�p�[�e�B�N��
		std::unique_ptr<Particle>PireEffect;
		//�˂��o���̃C�[�W���O�p
		float PosYMovingEaseT;
		//���W����
		XMFLOAT3 ObjPos;
		XMFLOAT3 TexPos;
	};

	//�B�ʒu�̍ő�ƍŏ��l
	static constexpr float NailMaxPosY = 32.f;
	static constexpr float NailMinPosY = -20.f;

	//�B�C���X
	std::array<NailObjs, NailSize>nail_objses_;
	//�U���̃t�F�[�Y
	Phase phase;
	static void (FrontCircleAttack::* actionTable[])();
public:
	//�t�F�[�Y�Z�b�g
	void SetAttackPhase(bool f) { if (f && phase != PHASEONE) { phase = PHASEONE; } }
	Phase GetPhaseEnd() { return phase; }

private:
	void NonInit();
	//�͈͐ݒ�
	void SetDamageArea();
	//�B�o�Ă���
	void PireNail();
	//�B��������
	void DestNail();

	void AttackEnd();
};
