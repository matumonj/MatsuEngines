#pragma once
#include"Object3d.h"
#include"Model.h"
#include"DebugCamera.h"

#include"Player.h"
#include<memory>
#include"CollisionPrimitive.h"

class House : public ObjectManager
{
public:
	House();
	~House() override;

protected:
	OBB playerOBB = {};
	OBB HouseOBB = {}; // = nullptr;
	float alpha = 0.0f;
public:
	void Initialize(DebugCamera* camera) override;
	void Update(DebugCamera* camera) override;
	void Draw() override;
	virtual bool CollideHouse();
};

class House_A : public House
{
public:
	void Initialize(DebugCamera* camera) override;
	void Update(DebugCamera* camera) override;
	void Draw() override;
	bool CollideHouse() override;
};

class House_B : public House
{
public:
	void Initialize(DebugCamera* camera) override;
	void Update(DebugCamera* camera) override;
	void Draw() override;
	bool CollideHouse() override;
};
