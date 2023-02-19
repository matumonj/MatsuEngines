#pragma once
#include <DirectXMath.h>

class BossAttackActionManager
{
protected:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private:
public:
	virtual void Init() =0;
	virtual void Upda() =0;
	virtual void Draw() =0;


public:
	enum Phase
	{
		PHASE_NON,
		PHASE_ONE,
		PHASE_TWO,
		PHASE_THREE,
		PHASE_FOUR
	} _phase;

public:
	Phase GetPhase() { return _phase; }
	void SetAttackPhase(bool f) { if (f && _phase != PHASE_ONE) { _phase = PHASE_ONE; } }
};
