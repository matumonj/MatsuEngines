#pragma once
#include "WoodBase.h"
#pragma once
#include"Object3d.h"
#include"Model.h"
#include"DebugCamera.h"

#include"Player.h"
#include<memory>
#include"CollisionPrimitive.h"

class WoodB : public WoodBase
{
public:
	WoodB();
	~WoodB() override;

private:
	bool f = false;
	float t = false;
	float alpha = 0.0f;
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void CollideWood() override;
};
