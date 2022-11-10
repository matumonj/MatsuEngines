#pragma once
#include "AreaFence.h"
#include"PlayerControl.h"
#include"SceneManager.h"
#include"Collision.h"
AreaFence::AreaFence()
{

}
AreaFence::~AreaFence()
{

}
void AreaFence::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();
	//m_Object->CreateGraphicsPipeline(L"Resources/Shader/Object3dVS.hlsl", L"Resources/Shader/Object3dPS.hlsl", L"Resources/Shader/BasicGS.hlsl");
	//m_Model = ModelManager::GetIns()->GetModel(ModelManager::FENCE);

	//フィールドにモデル割り当て
	m_Object->Initialize(camera);
	m_Object->SetModel(ModelManager::GetIns()->GetModel(ModelManager::FENCE));
	
	Scale = { 10,10,10 };
	SetCollider();
}

void AreaFence::Update(DebugCamera* camera)
{
	if (SceneManager::GetInstance()->GetScene() != SceneManager::MAPCREATE) {
		CollideAreaFence();
	}
	//フィールド
	ParameterSet_Obj(camera);
}

void AreaFence::Draw()
{
	Draw_Obj();
}

bool AreaFence::CollideAreaFence()
{
	playerOBB.SetOBBParam_Pos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
	playerOBB.SetOBBParam_Rot(PlayerControl::GetInstance()->GetPlayer()->GetMatrot());
	playerOBB.SetOBBParam_Scl({ 1.0f,1.0f,1.0f });

	//柵のOBB 回転ベクトル
	AreaFenceOBB.SetOBBParam_Pos(Position);
	AreaFenceOBB.SetOBBParam_Rot(m_Object->GetMatrot());
	AreaFenceOBB.SetOBBParam_Scl({ m_Object->GetMatScl().r[0].m128_f32[0] * 4,
		m_Object->GetMatScl().r[1].m128_f32[1] * 3 ,
		m_Object->GetMatScl().r[2].m128_f32[2] - 2 });

	if (Collision::CheckOBBCollision(playerOBB,AreaFenceOBB)==true) {
		Collision::SetCollideOBB(true);
		//PlayerControl::GetInstance()->GetPlayer()->isOldPos();
		return true;
	}

	return false;
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