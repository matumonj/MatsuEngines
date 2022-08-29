#pragma once
#include "AreaFence.h"
#include"Player.h"

AreaFence::~AreaFence()
{
	//delete  m_Model;
	delete ps0;
}
void AreaFence::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();

	m_Model = Model::CreateFromOBJ("Fence");

	//フィールドにモデル割り当て
	m_Object->Initialize(camera);
	m_Object->SetModel(m_Model);
	ps0 = new OBBCollision();

	Scale = { 10,10,10 };

}

void AreaFence::Update(DebugCamera* camera)
{
	CollideAreaFence();
	//フィールド
	ParameterSet_Obj(camera);
}

void AreaFence::Draw()
{
	Draw_Obj();
}

bool AreaFence::CollideAreaFence()
{
	return true;
}

void AreaFence::FenceOpenCondition(bool condition)
{
	const float FenceDownSpeed = 0.1f;
	if (condition) {
		Position.y -=FenceDownSpeed;
		if (Position.y < FencePosY_Min) {
			
		}
	}
	else {
		FencePosY_Min = Position.y - 30;
	}
	Position.y = max(Position.y, FencePosY_Min);

	
}