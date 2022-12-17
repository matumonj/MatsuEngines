#pragma once
#include "Texture.h"
#include<memory>
#include"Particle.h"
class GuardianBomAttack
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	void TexSet();
	void Upda();
	void Draw();

private:
	enum Phase
	{
		NON,
		AREASET,
		BOM,
		END
	}phase;

	void Phase_AreaSet();
	void Phase_Bom();
	void Phase_End();
private:
	std::unique_ptr<Texture>DamageTex;
	std::unique_ptr<Particle>BomEffect;
	XMFLOAT2 TexScl;
	float TexAlpha;
};

