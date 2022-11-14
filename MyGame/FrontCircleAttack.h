#pragma once
#include"Texture.h"
#include"Object3d.h"
#include"Model.h"
#include<memory>
#include"CollisionPrimitive.h"

class FrontCircleAttack
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	static FrontCircleAttack* GetInstance();
	void Initialize();
	void ActionJudg();
	void Draw();
	void Finalize();

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
	bool SlamAttackF;
	Texture* FissureTex;
	Texture* DamageAreaTex;

	Fase fase;
	float TexAlpha;
public:
	void SetAttackFase(bool f) { if (f && fase != FASEONE) { fase = FASEONE; } }
	Fase GetFaseEnd() { return fase; }
};
