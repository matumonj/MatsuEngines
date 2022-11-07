#pragma once
#include"Texture.h"
#include"Enemy.h"
class AttackEffect
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	static AttackEffect* GetIns();

public:
	void Init();
	void Upda();
	void Draw();

	void ParticleUpda();
	void SetParticle(XMFLOAT3 pos);
private:
	void LoadTex();
	void Effect_Non();
	void Effect_First();
	void Effect_Second();
	void Effect_Third();
	void Effect_Particle();
	void Effect_Clear();
public:
	enum EffectType {
		NON,
		SLASH_FIRST,
		SLASH_SECOND,
		SLASH_THIRD,
		PARTICLE,
		CLEAREFFECT
	};
	enum ParticleSize {//�p�e�B�N�����ݒ�
		FIRST = 10,
		SECOND = 20,
		THIRD=15
	};
	enum EffectUpda {//�֐���������
		LOAD,
		UPDA,
		CLEAR
	};
	EffectUpda eupda = LOAD;
	EffectType etype = NON;

	void SetEffectType(EffectType type) { etype = type; }

	EffectType GetEffectType() { return etype; }

private:
	//�a���p�e�N�X�`��
	std::unique_ptr<Texture>AttackTex;
	//�U�����ɏo��p�[�e�B�N��
	std::vector<std::unique_ptr<Texture>>AttackParticle;

	std::vector<Texture*> l_ptex;
	//�U���p�e�N�X�`���̃p�����[�^
	XMFLOAT3 TexPos;
	XMFLOAT3 TexRot;
	XMFLOAT3 TexScl;
	float texAlpha = 1.0f;
	//�p�[�e�B�N�������ʒu
	XMFLOAT3 ParCenterPos;
	//�e�p�[�e�B�N�����W�A�X�P�[��
	std::vector<XMFLOAT3> ParScl;
	std::vector<XMFLOAT3> ParPos;
	std::vector<XMFLOAT3> ParRot;

};

