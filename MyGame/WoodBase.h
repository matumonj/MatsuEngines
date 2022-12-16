#pragma once
#include "CollisionPrimitive.h"
#include"ObjectManager.h"

class WoodBase : public ObjectManager
{
public:
	//初期化
	void Initialize(DebugCamera* camera) override =0;
	//更新処理
	void Update(DebugCamera* camera) override =0;
	//描画処理
	void Draw() override =0;

protected:
	OBB playerOBB = {};
	OBB woodOBB = {}; // = nullptr;
public:
	virtual void CollideWood() =0;
};
