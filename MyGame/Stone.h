
#pragma once
#include"Object3d.h"
#include"Model.h"
#include"DebugCamera.h"

#include"Player.h"
#include<memory>
#include"CollisionPrimitive.h"
class Stone :public ObjectManager
{

public:
	Stone() {};
	~Stone();

protected:
	OBB playerOBB;
	OBB StoneOBB;// = nullptr;
	bool f;
	float t;
	float alpha;
public:
	virtual void Initialize(DebugCamera* camera)override;
	virtual void Update(DebugCamera* camera)override;
	virtual void Draw()override;
	virtual bool CollideStone();
};

class Stone_A :public Stone
{
public:
	void Initialize(DebugCamera* camera)override;
	void Update(DebugCamera* camera)override;
	void Draw()override;
	bool CollideStone()override;
};

class Stone_B :public Stone
{
public:
	void Initialize(DebugCamera* camera)override;
	void Update(DebugCamera* camera)override;
	void Draw()override;
	bool CollideStone()override;
};

