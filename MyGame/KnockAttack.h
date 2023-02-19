#pragma once
#include"Texture.h"
#include"Input.h"
#include<array>
#include<memory>
#include"Object3d.h"
#include"Particle.h"
#include"CollisionPrimitive.h"
#include"BossAttackActionManager.h"

class KnockAttack : public BossAttackActionManager
{
public:
	KnockAttack();
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
	std::array<std::unique_ptr<Object3d>, axeSize> AxeObj;
	std::array<XMFLOAT3, axeSize> AxePos;
	std::array<XMFLOAT3, axeSize> AxeRot;
	std::array<XMFLOAT3, axeSize> SetPos;
	std::array<std::unique_ptr<Particle>, axeSize> ImpactPar;
	std::array<std::unique_ptr<Texture>, axeSize> ImpactTex;
	std::array<std::unique_ptr<Texture>, axeSize> AxeDirectionTex;
	float axeDirectionTexAlpha;
	std::array<XMFLOAT3, axeSize> ImpactTexScl;
	std::array<XMFLOAT3, axeSize> ImpactTexPos;
	std::array<float, axeSize> ImpactTexAlpha;
	float corre;
	float AxePosDownEtime;
	int AttackCount;
	Line2D damageLine[axeSize];
	XMVECTOR move[axeSize];
	XMMATRIX matRot[axeSize];

public:
	static KnockAttack* GetIns();
	void Init() override;
	void Draw() override;
	void Upda() override;
	void Finalize();
};
