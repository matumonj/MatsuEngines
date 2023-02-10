#pragma once
#include"Object3d.h"
#include"Model.h"
#include<memory>

class Nail
{
public:
	~Nail();
	static Nail* GetIns();
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
private:
	std::vector<std::unique_ptr<Object3d>> Nails;
	Model* Nailmodel;
	float MinY = -30;

	enum Phase
	{
		PHASE_ONE,
		PHASE_TWO,
		PHASE_THREE,
		NON
	};

	struct NailParam
	{
		XMFLOAT3 NailPosition;
		std::vector<int> ZN;
		std::vector<int> XN;
		int WaitCount;
		bool EndAction;
		Phase phase;
	};

	NailParam HAttack;
	NailParam CAttack;
public:
	enum HalfAttackArea
	{
		RIGHT,
		LEFT
	};

	void HalfAttack(const HalfAttackArea& area);

	void CircleAttack(int area1, int area2);
	void NailAmountSet(int amount);

	void ModelSet();
	void Update();
	void Draw();
	bool GetEndAction_Half() { return HAttack.EndAction; }
	void SetEndAction_Half(bool f) { HAttack.EndAction = f; }

	bool GetEndAction_HalfPhase()
	{
		if (HAttack.phase == NON) { return true; }
		return false;
	}

	void SetEndAction_Circle(bool f) { CAttack.EndAction = f; }
	bool GetEndAction_Circle() { return CAttack.EndAction; }

	void Finalize();
};
