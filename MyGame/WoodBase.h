#pragma once
#include "CollisionPrimitive.h"
#include"ObjectManager.h"

class WoodBase : public ObjectManager
{
public:
	//������
	void Initialize() override =0;
	//�X�V����
	void Update() override =0;
	//�`�揈��
	void Draw() override =0;

protected:
	OBB playerOBB = {};
	OBB woodOBB = {}; // = nullptr;
public:
	virtual void CollideWood() =0;
};
