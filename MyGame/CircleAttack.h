#pragma once
#include"Texture.h"
#include"Object3d.h"
#include"Model.h"
#include<memory>
class CircleAttack
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
private:
	static const int DirectionNum = 4;
	XMFLOAT3 Direction[DirectionNum]; 
	enum {
		NORTH, 
		SOUTH, 
		EAST,
		WEST
	};
	std::vector<std::unique_ptr<Object3d>>NailObj;
	Model* NailModel;
	Texture* ImpactAreaTex[2];
	float TexAlpha;
	bool fase1, fase2, fase3,fase4;
	int AttackCount;
	float CircleAreaTime = 0;
	XMFLOAT2 CircleSize = { 0,0 };

	int Area1, Area2;
	bool NailAttackFlag;
public:
	static CircleAttack* GetInstance();
	void Initialize();
	void ActionJudg();
	void Draw();
	void ImpactAttack(int area1,int area2);
	int GetDamageArea1() { return Area1; }
	int GetDamageArea2() { return Area2; }
	void SetDamageArea(int area1, int area2) { Area1 = area1; Area2 = area2; };

	bool GetNailMove() { return NailAttackFlag; }
};

