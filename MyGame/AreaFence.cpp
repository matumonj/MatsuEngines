#pragma once
#include "AreaFence.h"
#include"Player.h"

AreaFence::~AreaFence()
{
	delete ps0;
}
void AreaFence::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();

	m_Model = Model::CreateFromOBJ("Fence");

	//�t�B�[���h�Ƀ��f�����蓖��
	m_Object->Initialize(camera);
	m_Object->SetModel(m_Model);
	ps0 = new OBBCollision();

	Scale = { 10,10,10 };

}

void AreaFence::Update(DebugCamera* camera)
{
	m_Object->setSetf(true);
	CollideAreaFence();
	//�t�B�[���h
	ParameterSet_Obj(camera);
}

void AreaFence::Draw()
{
	Draw_Obj();
}

bool AreaFence::CollideAreaFence()
{
	playerOBB.m_NormaDirect[0] = { Player::GetInstance()->GetMatrot().r[0].m128_f32[0],Player::GetInstance()->GetMatrot().r[0].m128_f32[1],Player::GetInstance()->GetMatrot().r[0].m128_f32[2] };
	playerOBB.m_NormaDirect[1] = { Player::GetInstance()->GetMatrot().r[1].m128_f32[0],Player::GetInstance()->GetMatrot().r[1].m128_f32[1],Player::GetInstance()->GetMatrot().r[1].m128_f32[2] };
	playerOBB.m_NormaDirect[2] = { Player::GetInstance()->GetMatrot().r[2].m128_f32[0],Player::GetInstance()->GetMatrot().r[2].m128_f32[1],Player::GetInstance()->GetMatrot().r[2].m128_f32[2] };
	playerOBB.m_fLength[0] = 1;//x�����̒���
	playerOBB.m_fLength[1] = 1;//y�����̒���
	playerOBB.m_fLength[2] = 1;//z�����̒���
	//�G��OBB ��]�x�N�g��
	AreaFenceOBB.m_NormaDirect[0] = { m_Object->GetMatrot().r[0].m128_f32[0],m_Object->GetMatrot().r[0].m128_f32[1],m_Object->GetMatrot().r[0].m128_f32[2] };
	AreaFenceOBB.m_NormaDirect[1] = { m_Object->GetMatrot().r[1].m128_f32[0], m_Object->GetMatrot().r[1].m128_f32[1], m_Object->GetMatrot().r[1].m128_f32[2] };
	AreaFenceOBB.m_NormaDirect[2] = { m_Object->GetMatrot().r[2].m128_f32[0], m_Object->GetMatrot().r[2].m128_f32[1], m_Object->GetMatrot().r[2].m128_f32[2] };
	AreaFenceOBB.m_fLength[0] = 10;//x�����̒���
	AreaFenceOBB.m_fLength[1] = 20;//y�����̒���
	AreaFenceOBB.m_fLength[2] = 10;//z�����̒���
	//OBB�̐ݒ�ʒu
	playerOBB.m_Pos = { Player::GetInstance()->GetPosition().x,Player::GetInstance()->GetPosition().y,Player::GetInstance()->GetPosition().z };
	AreaFenceOBB.m_Pos = { m_Object->GetPosition().x,  m_Object->GetPosition().y, m_Object->GetPosition().z };

	if (ps0->ColOBBs(playerOBB, AreaFenceOBB)) {
		//Collision::SetCollideOBB(true);
		Player::GetInstance()->isOldPos();
		return true;
	}
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