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
		std::unique_ptr<Texture>BomTex;
		DirectX::XMFLOAT3 CenterPos;
		DirectX::XMFLOAT3 VelSpeed;
		float TexAlpha;
		float Angle;
	};
	//パーティクルサイズ
	static constexpr int BomParticleSize = 15;
	//
	std::array<BomParticle, BomParticleSize> bom_particle_;
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
	//飛ばす岩石の数
	static constexpr int meteoSize = 5;
	Phase _phase = Phase::NON;
	//岩石モデル
	std::array<std::unique_ptr<Object3d>, meteoSize>MeteoRock;
	//各岩石の座標
	std::array<DirectX::XMFLOAT3, meteoSize>MeteoPos;
	//ダメージエリアテクスチャ
	std::array< std::unique_ptr<Texture>, meteoSize>MeteoDamageAreaTex;

	std::array<float, meteoSize>MeteoRockAngles;

	std::array<DirectX::XMFLOAT3, meteoSize>DAreaTexSize;

	std::array<float, meteoSize>DAreaTexAlpha;
	//岩石落下スピード イージング用
	float MeteoFallT;

	enum DamageArea
	{
		LEFT,
		RIGHT,
		UP,
		BOTTOM
	};

	//各ダメージエリアの座標書き出し
	std::map<DamageArea,DirectX::XMFLOAT3>damageAreaPos;
	void Phase_One();
	void Phase_Two();
	void Phase_Three();
};