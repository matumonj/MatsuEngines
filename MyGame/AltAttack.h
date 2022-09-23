#pragma once
#include"Texture.h"
#include"Object3d.h"
#include"Model.h"
#include<memory>
#include"ParticleManager.h"
class AltAttack
{
public:
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	
private:
	std::vector<Texture*>EnergieSphere;
	std::unique_ptr<Object3d>BeamObj;
	Model* BeamModel;
	ParticleManager* BeamParticle;
	bool fase1, fase2, fase3, fase4;
	int AttackCount;

public:

public:enum Fase {
	FASENON,
	FASEONE,
	FASETWO,
	FASETHREE,
	FASEFOUR
};
private:
	Fase fase;
public:
	static AltAttack* GetInstance();
	void Initialize();
	void ActionJudg();
	void Draw();

private:
	void EnergieCharge();
	void BeamAttack();
public:

	void SetAttackFase(bool f) { if (f && fase != FASEONE) { fase = FASEONE; } }
	Fase GetFaseEnd() { return fase; }

};

