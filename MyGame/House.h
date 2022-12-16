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
	void Initialize() override;
	void Update() override;
	void Draw() override;
	virtual bool CollideHouse();
};

class House_A : public House
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	bool CollideHouse() override;
};

class House_B : public House
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	bool CollideHouse() override;
};
