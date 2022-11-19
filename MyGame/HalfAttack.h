#pragma once
#include"Texture.h"
#include"Input.h"
#include"MobEnemy.h"
#include<array>
class HalfAttack
{
public:
	~HalfAttack();
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
private:
	enum AttackArea
	{
		LEFT,
		RIGHT
	};
	static constexpr int EnemySize = 4;
	std::array<std::unique_ptr<Texture>, EnemySize>MagicTex;
	std::array<XMFLOAT3, EnemySize>TexScl;

	AttackArea Area;

	Texture* HalfAreaTex;
	float RotY;
	bool fase1, fase2, fase3, fase4;
	int AttackCount;
	float TexAlpha = 1.0f;
	float AfterPositionZ;
	float BeforePositionZ;
	float KnockTime = 0.0f;

	XMFLOAT3 CenterPos;
	//fase1　カウントダウンと中央に戻る処理
	XMFLOAT3 PlayerPos;
	XMFLOAT3 BossEnemyPos;

	void DamageJudg_Left();
	void DamageJudg_Right();
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
	static HalfAttack* GetInstance();
	XMFLOAT3 MoveCenterPos();
	void Initialize();
	void Draw();
	void ActionJudg();
	void TurnCenter();
	bool SummonEnemy();

	void SetAttackFase(bool f) { if (f && fase != FASEONE) { fase = FASEONE; } }
	XMFLOAT3 GetTexPos(int index) { return MagicTex[index]->GetPosition(); }
	Fase GetFaseEnd() { return fase; }
};
