#pragma once
#include"Object3d.h"
#include"Model.h"
#include"DebugCamera.h"

#include"Player.h"
#include<memory>
#include"CollisionPrimitive.h"

class Wood : public ObjectManager
{
public:
	Wood();
	~Wood() override;

private:
	OBB playerOBB = {};
	OBB woodOBB = {}; // = nullptr;
	bool f = false;
	float t = false;
	float alpha = 0.0f;
public:
	void Initialize(DebugCamera* camera) override;
	void Update(DebugCamera* camera) override;
	void Draw() override;
	bool CollideWood();
};
