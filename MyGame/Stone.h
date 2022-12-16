#pragma once
#include"Object3d.h"
#include"Model.h"
#include"DebugCamera.h"

#include"Player.h"
#include<memory>
#include"CollisionPrimitive.h"

class Stone : public ObjectManager
{
public:
	Stone();
	~Stone() override;

protected:
	OBB playerOBB = {};
	OBB StoneOBB = {}; // = nullptr;
	float alpha = 0.0f;
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	virtual bool CollideStone();
};

class Stone_A : public Stone
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	bool CollideStone() override;
};

class Stone_B : public Stone
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	bool CollideStone() override;
};
