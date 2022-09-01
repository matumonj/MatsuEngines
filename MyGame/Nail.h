#pragma once
#include"Object3d.h"
#include"Model.h"
#include<memory>
class Nail
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
private:
	std::vector<std::unique_ptr<Object3d>>Nails;
	Model* Nailmodel;
	bool SetF;
	float MinY= -30 ;
	enum Fase {
		FASE_ONE,
		FASE_TWO,
		FASE_THREE,
		NON
	};
	enum HalfAttackArea {
		RIGHT,
		LEFT
	};
	struct NailParam {
		XMFLOAT3 NailPosition;
		std::vector<int>ZN;
		std::vector<int>XN;
		int WaitCount;
		bool SetF;
		Fase fase;
	};
	NailParam HAttack;
public:
	void HalfAttack(HalfAttackArea area);

	void CircleAttack(int area1,int area2);
	void NailAmountSet(int amount);
	
	void ModelSet();
	void Update();
	void Draw();
};

