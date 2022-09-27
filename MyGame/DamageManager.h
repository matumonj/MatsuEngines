#pragma once
#include "Sprite.h"
#include <vector>
#include <memory>
#include"Texture.h"
#include<iomanip>
#include<iostream>
#include<sstream>
#include"DebugTxt.h"
class DamageManager {
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
	static DamageManager* GetIns();		//Žæ“¾—p
	void Init();
	void Upda();
	void Draw();
	void DamageDisPlay(int damage, XMFLOAT4 color,XMFLOAT3 Position);
private:
	void TextBeha();
private:
	std::list<std::unique_ptr<DebugTxt>>DamageTex;
};
