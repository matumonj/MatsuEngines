#pragma once
#include "Sprite.h"
#include <vector>
#include <memory>
#include"Texture.h"
#include<iomanip>
#include<iostream>
#include<sstream>
#include"DebugTxt.h"

class DamageManager
{
private:
	DamageManager() = default;
	~DamageManager() = default;
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private:
public:
	static DamageManager* GetIns(); //Žæ“¾—p
	void Init();
	void Upda();
	void Draw();
	void DamageDisPlay(int damage, XMFLOAT4 color, XMFLOAT3 Position);
	void SetTexSize(float size) { TexSize.x = size; }
	void SetAlpha(float alpha) { TexAlpha = alpha; }
private:
	void TextBeha();
	XMVECTOR WDivi(const DirectX::XMVECTOR& pos, const DirectX::XMMATRIX& mat, bool s);

	XMVECTOR WDivi(const DirectX::XMVECTOR& pos, bool isWSlide);
	XMVECTOR PosDivi(const DirectX::XMVECTOR& pos, const DirectX::XMMATRIX& mat, bool s);
private:
	XMFLOAT2 TexSize = {};
	float TexAlpha = 0.0f;
	int DamageDisPlayT = 0;
	std::list<std::unique_ptr<DebugTxt>> DamageTex;
};
