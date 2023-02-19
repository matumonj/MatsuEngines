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


private:
	//�B��
	static constexpr int NailSize = 10;

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
		float TexSclingEaseT;
		//���W����
		XMFLOAT3 ObjPos;
		XMFLOAT3 TexPos;
		XMFLOAT3 TexScl;

		float TexRotY;
	};

	//�B�ʒu�̍ő�ƍŏ��l
	static constexpr float NailMaxPosY = 32.f;
	static constexpr float NailMinPosY = -20.f;

	//�B�C���X
	std::array<NailObjs, NailSize>nail_objses_;

	//�U���̃t�F�[�Y�̊֐��e�[�u��
	static void (FrontCircleAttack::* actionTable[])();

	//�_���[�W
	static constexpr int Damage = 8;
private:
	void NonInit();
	//�͈͐ݒ�
	void SetDamageArea();
	//�B�o�Ă���
	void PireNail();
	//�B��������
	void DestNail();

	void AttackEnd();

	//�B�Ƃ̓����蔻��
	bool ColNail_Player();
};
