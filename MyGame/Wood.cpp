#include "Wood.h"
#include "Wood.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"Collision.h"
bool Wood::Initialize(DebugCamera* camera)
{
	WoodObject = std::make_unique<Object3d>();
	WoodModel = Model::CreateFromOBJ("wood");

	//�t�B�[���h�Ƀ��f�����蓖��
	WoodObject->Initialize(camera);
	WoodObject->SetModel(WoodModel);
	ps0 = new OBBCollision();
	return true;
}

void Wood::Update(DebugCamera* camera)
{
	WoodObject->setSetf(true);
	//�t�B�[���h
	CollideWood();
	WoodObject->SetRotation({ 0,0,0 });
	WoodObject->SetScale({2,3,2});
	WoodObject->SetPosition(Position);
	WoodObject->Update({ 1,1,1,1 }, camera);
}

void Wood::Draw()
{
	WoodObject->PreDraw();
	WoodObject->Draw();
	WoodObject->PostDraw();
}

bool Wood::CollideWood()
{
	playerOBB.m_NormaDirect[0] = { Player::GetInstance()->GetMatrot().r[0].m128_f32[0],Player::GetInstance()->GetMatrot().r[0].m128_f32[1],Player::GetInstance()->GetMatrot().r[0].m128_f32[2] };
	playerOBB.m_NormaDirect[1] = { Player::GetInstance()->GetMatrot().r[1].m128_f32[0],Player::GetInstance()->GetMatrot().r[1].m128_f32[1],Player::GetInstance()->GetMatrot().r[1].m128_f32[2] };
	playerOBB.m_NormaDirect[2] = { Player::GetInstance()->GetMatrot().r[2].m128_f32[0],Player::GetInstance()->GetMatrot().r[2].m128_f32[1],Player::GetInstance()->GetMatrot().r[2].m128_f32[2] };
	playerOBB.m_fLength[0] = 1;//x�����̒���
	playerOBB.m_fLength[1] = 1;//y�����̒���
	playerOBB.m_fLength[2] = 1;//z�����̒���
	//OBB ��]�x�N�g��
	woodOBB.m_NormaDirect[0] = { WoodObject->GetMatrot().r[0].m128_f32[0],WoodObject->GetMatrot().r[0].m128_f32[1],WoodObject->GetMatrot().r[0].m128_f32[2] };
	woodOBB.m_NormaDirect[1] = { WoodObject->GetMatrot().r[1].m128_f32[0], WoodObject->GetMatrot().r[1].m128_f32[1], WoodObject->GetMatrot().r[1].m128_f32[2] };
	woodOBB.m_NormaDirect[2] = { WoodObject->GetMatrot().r[2].m128_f32[0], WoodObject->GetMatrot().r[2].m128_f32[1], WoodObject->GetMatrot().r[2].m128_f32[2] };
	woodOBB.m_fLength[0] = 1;//x�����̒���
	woodOBB.m_fLength[1] = 20;//y�����̒���
	woodOBB.m_fLength[2] =1;//z�����̒���
	//OBB�̐ݒ�ʒu
	playerOBB.m_Pos = { Player::GetInstance()->GetPosition().x,Player::GetInstance()->GetPosition().y,Player::GetInstance()->GetPosition().z };
	woodOBB.m_Pos = { WoodObject->GetPosition().x,  WoodObject->GetPosition().y, WoodObject->GetPosition().z };

	if (Collision::GetLength(Player::GetInstance()->GetPosition(), Position) < 20) {
		if (ps0->ColOBBs(playerOBB, woodOBB)) {
			Player::GetInstance()->isOldPos();
			return true;
		} else {
				return false;
		}
	}
}