#pragma once
#include "Enemy.h"
#include"Object3d.h"
#include "Particle.h"
#include"Texture.h"
class ThrowRockAttack
{
public:
	static ThrowRockAttack*GetInstance();
public:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private:
	std::unique_ptr<Object3d> ThrowRockObj;
	std::unique_ptr<Texture>DamageArea;

	//岩座標
	XMFLOAT3 RockPos;
	std::array<std::unique_ptr<Object3d>,5> DestRock;
	std::array<float, 5> DestRockAlpha;
	std::array<float, 5> DestRockAcc;
	std::array<XMFLOAT3, 5> DestRockPos;
	bool destF;
	bool turnoffdrawF;
	DebugCamera* camera;
public:
	void Init();
	void Action();
	void Draw();

	//投げる動作の流れ
	enum ThrowMotion
	{
		NON,
		SET,
		PICK,
		THROW,
		END
	} tmotion;
	void SetAction(bool f) { if (tmotion != SET) { tmotion = SET; } }
	ThrowMotion GetPhase() { return tmotion; }
private:
	void ThrowPhase_Set();
	void ThrowPhase_Pick();
	void ThrowPhase_Throw();
	void ThrowPhase_End();

	void DestEffect();
	void ActionUpda();
	static void (ThrowRockAttack::* actionTable[])();
};
