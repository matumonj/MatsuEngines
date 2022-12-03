#pragma once
#include "CollisionPrimitive.h"
#include"ObjectManager.h"
class WoodBase:public ObjectManager
{
public:
	//‰Šú‰»
	virtual void Initialize(DebugCamera* camera) override=0;
	//XVˆ—
	virtual void Update(DebugCamera* camera) override=0;
	//•`‰æˆ—
	virtual void Draw() override=0;

protected:
	OBB playerOBB = {};
	OBB woodOBB = {}; // = nullptr;
public:
	virtual void CollideWood()=0;
};
