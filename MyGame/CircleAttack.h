#pragma once
#include"Texture.h"
#include"Object3d.h"
#include"Model.h"
#include<memory>
#include"Sprite.h"
#include<array>
#include<map>

class CircleAttack
{
public:
	~CircleAttack();
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	enum
	{
		NORTH,
		SOUTH,
		EAST,
		WEST
	};

private:
	float rotY;
	static const int DirectionNum = 4;
	XMFLOAT3 Direction[DirectionNum];

	std::vector<std::unique_ptr<Object3d>> NailObj;
	Model* NailModel;
	Texture* ImpactAreaTex[2];
	float TexAlpha = 1;
	bool fase1, fase2, fase3, fase4;
	int AttackCount;
	float CircleAreaTime = 0;
	XMFLOAT2 CircleSize = {0, 0};

	int Area1, Area2;
	bool NailAttackFlag;
public:
public:
	enum Fase
	{
		FASENON,
		FASEONE,
		FASETWO,
		FASETHREE,
		FASEFOUR
	};

private:
	Fase fase;
public:
	static CircleAttack* GetInstance();
	void Initialize();
	void ActionJudg();
	void Draw();

private:
	void PierceNail();
	void DamageAreaTexSet();
	void ProtrudeNail();
	void EndAttackAction();
public:
	int GetDamageArea1() { return Area1; }
	int GetDamageArea2() { return Area2; }

	void SetDamageArea(int area1, int area2)
	{
		Area1 = area1;
		Area2 = area2;
	};

	bool GetNailMove() { return NailAttackFlag; }

	void SetAttackFase(bool f) { if (f && fase != FASEONE) { fase = FASEONE; } }
	Fase GetFaseEnd() { return fase; }

private:
	void CollisonNailPlayer();
};

class BomAttack
{
private:
	struct BomParticle
	{
		std::unique_ptr<Texture> BomTex;
		DirectX::XMFLOAT3 CenterPos;
		DirectX::XMFLOAT3 TexPos;
		DirectX::XMFLOAT3 TexScl;
		float Speed;
		float TexAlpha;
		float Angle;
	};

	enum bParticlePhase
	{
		NON,
		SETPOSITION,
		UPDATE,
		CLEAR
	} _Bphase;

	//パーティクルサイズ
	static constexpr int BomParticleSize = 15;
	static constexpr int meteoSize = 7;
	//
	float attackangle;
	bool jf[meteoSize];
	DirectX::XMFLOAT3 BossPos;
	DirectX::XMFLOAT3 CenterPosi[meteoSize];
	std::array<std::array<BomParticle, BomParticleSize>, meteoSize> bom_particle_;
public:
	void Init();
	void Upda();
	void Setting_DamageArea();
	void Draw();

public:
	enum class Phase
	{
		NON,
		PHASE_ONE,
		PHASE_TWO,
		PHASE_THREE,
		END
	};

private:
	bool AttackJudg;
	int NextPhase_WaitC = 0;

	static constexpr float FieldBottomPosY = 10.0f;

	enum DamageArea
	{
		LEFT,
		RIGHT,
		UP,
		BOTTOM
	};

	float MteoObjAlpha;
	//各ダメージエリアの座標書き出し
	std::map<DamageArea, DirectX::XMFLOAT3> damageAreaPos;
	void Phase_One();
	void Phase_Two();
	void Phase_Three();
	void Phase_Four();
	void Phase_End(int indexMeteo, int indexPar);
	void MeteoRot(const DamageArea& area);
	void BomParticleUpda();
	void MeteoRockMove();

	void ParamSet();
};
