#pragma once
#include "CollisionPrimitive.h"
#include"ObjectManager.h"
class WoodBase:public ObjectManager
{
public:
	//������
	virtual void Initialize(DebugCamera* camera) override=0;
	//�X�V����
	virtual void Update(DebugCamera* camera) override=0;
	//�`�揈��
	virtual void Draw() override=0;

protected:
	OBB playerOBB = {};
	OBB woodOBB = {}; // = nullptr;
public:
	virtual void CollideWood()=0;
};
