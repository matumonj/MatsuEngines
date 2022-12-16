#pragma once
#include"ObjectManager.h"
#include"CollisionPrimitive.h"

class AreaFence : public ObjectManager
{
public:
	AreaFence();
	~AreaFence() override;

private:
	OBB playerOBB;
	OBB AreaFenceOBB;

	float FencePosY_Min;
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;

	bool FenceYposMin();
	void FenceOpenCondition(bool Condition);
	bool CollideAreaFence();
};
