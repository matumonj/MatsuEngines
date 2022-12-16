#pragma once
#include"Object3d.h"
#include"Model.h"
#include"DebugCamera.h"

#include"Player.h"
#include<memory>
#include"CollisionPrimitive.h"
#include "WoodBase.h"

class Wood : public WoodBase
{
public:
	Wood();
	~Wood() override;

private:
	bool f = false;
	float t = false;
	float alpha = 0.0f;
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	void CollideWood() override;
};
