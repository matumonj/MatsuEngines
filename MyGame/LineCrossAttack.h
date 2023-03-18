#pragma once
#include "BossAttackActionManager.h"

#include"Texture.h"
#include"Input.h"
#include<array>
#include<memory>
#include"Object3d.h"
#include"Particle.h"

class LineCrossAttack : public BossAttackActionManager
{
public:
	LineCrossAttack();
	~LineCrossAttack();
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
private:
	//Texture* KnockTex;
	//�ړ��x�N�g����y������̊p�x�ŉ�]
	XMVECTOR move;

	XMMATRIX matRot;
	static constexpr int axeSize = 2;
	std::array<std::unique_ptr<Texture>, axeSize> DamageTex;
	std::array<XMFLOAT3, axeSize> AxePos;
	std::array<XMFLOAT3, axeSize> AxeRot;
	std::array<XMFLOAT3, axeSize> SetPos;
	std::array<std::unique_ptr<Particle>, axeSize> ImpactPar;
	float axeDirectionTexAlpha;
	std::array<XMFLOAT3, axeSize> ImpactTexScl;
	std::array<XMFLOAT3, axeSize> ImpactTexPos;
	std::array<float, axeSize> ImpactTexAlpha;
	float corre;
	float AxePosDownEtime;
	int AttackCount;


public:
	//�������v���C���[��
	float Add_RotVal;
	static LineCrossAttack* GetIns();
	void Init() override;
	void Draw() override;
	void Upda() override;
	void Finalize();

	void NonInit();
	//�͈͐ݒ�
	void SetDamageArea();
	//�B�o�Ă���
	void CrossAttack();

	void AttackEnd();
	//�U���̃t�F�[�Y�̊֐��e�[�u��
	static void (LineCrossAttack::* actionTable[])();
};
