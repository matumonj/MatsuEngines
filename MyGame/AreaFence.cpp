#include "AreaFence.h"
#include "AreaFence.h"
#include "AreaFence.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"Player.h"
bool AreaFence::Initialize(DebugCamera* camera)
{
	AreaFenceObject = std::make_unique<Object3d>();

	//model2 = Model::CreateFromOBJ("newAreaFence");
	AreaFenceModel = Model::CreateFromOBJ("Fence");

	//�t�B�[���h�Ƀ��f�����蓖��
	AreaFenceObject->Initialize(camera);
	AreaFenceObject->SetModel(AreaFenceModel);
	ps0 = new OBBCollision();
	return true;
}

void AreaFence::Update(DebugCamera* camera)
{
	AreaFenceObject->setSetf(true);
	//�t�B�[���h
	AreaFenceObject->SetRotation({ 0,0,0 });
	AreaFenceObject->SetScale({ 10,10,10 });
	AreaFenceObject->SetPosition(Position);
	AreaFenceObject->Update({ 1,1,1,1 }, camera);
}

void AreaFence::Draw()
{
	AreaFenceObject->PreDraw();
	AreaFenceObject->Draw();
	AreaFenceObject->PostDraw();
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
	AreaFenceOBB.m_NormaDirect[0] = { AreaFenceObject->GetMatrot().r[0].m128_f32[0],AreaFenceObject->GetMatrot().r[0].m128_f32[1],AreaFenceObject->GetMatrot().r[0].m128_f32[2] };
	AreaFenceOBB.m_NormaDirect[1] = { AreaFenceObject->GetMatrot().r[1].m128_f32[0], AreaFenceObject->GetMatrot().r[1].m128_f32[1], AreaFenceObject->GetMatrot().r[1].m128_f32[2] };
	AreaFenceOBB.m_NormaDirect[2] = { AreaFenceObject->GetMatrot().r[2].m128_f32[0], AreaFenceObject->GetMatrot().r[2].m128_f32[1], AreaFenceObject->GetMatrot().r[2].m128_f32[2] };
	AreaFenceOBB.m_fLength[0] = 1;//x�����̒���
	AreaFenceOBB.m_fLength[1] = 20;//y�����̒���
	AreaFenceOBB.m_fLength[2] = 1;//z�����̒���
	//OBB�̐ݒ�ʒu
	playerOBB.m_Pos = { Player::GetInstance()->GetPosition().x,Player::GetInstance()->GetPosition().y,Player::GetInstance()->GetPosition().z };
	AreaFenceOBB.m_Pos = { AreaFenceObject->GetPosition().x,  AreaFenceObject->GetPosition().y, AreaFenceObject->GetPosition().z };

	if (ps0->ColOBBs(playerOBB, AreaFenceOBB)) {
		return true;
	} else {
		return false;
	}
}

void AreaFence::FenceOpenCondition(bool condition)
{
	if (condition) {
		Position.y -= 0.2f;
	}
	else {
		FencePosY_Min = Position.y - 30;
	}
	Position.y = max(Position.y, FencePosY_Min);
}