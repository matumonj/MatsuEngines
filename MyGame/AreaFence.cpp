#include "AreaFence.h"
#include "AreaFence.h"
#include "AreaFence.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"Collision.h"
#include"Player.h"
bool AreaFence::Initialize(DebugCamera* camera)
{
	AreaFenceObject = std::make_unique<Object3d>();

	//model2 = Model::CreateFromOBJ("newAreaFence");
	AreaFenceModel = Model::CreateFromOBJ("Fence");

	//フィールドにモデル割り当て
	AreaFenceObject->Initialize(camera);
	AreaFenceObject->SetModel(AreaFenceModel);
	ps0 = new OBBCollision();
	return true;
}

void AreaFence::Update(DebugCamera* camera)
{
	AreaFenceObject->setSetf(true);
	CollideAreaFence();
	//フィールド
	AreaFenceObject->SetRotation(Rotation);
	AreaFenceObject->SetScale({ 10,10,10 });
	AreaFenceObject->SetPosition(Position);
	AreaFenceObject->Update({ 0.5,0.5,1,1 }, camera);
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
	playerOBB.m_fLength[0] = 1;//x方向の長さ
	playerOBB.m_fLength[1] = 1;//y方向の長さ
	playerOBB.m_fLength[2] = 1;//z方向の長さ
	//敵のOBB 回転ベクトル
	AreaFenceOBB.m_NormaDirect[0] = { AreaFenceObject->GetMatrot().r[0].m128_f32[0],AreaFenceObject->GetMatrot().r[0].m128_f32[1],AreaFenceObject->GetMatrot().r[0].m128_f32[2] };
	AreaFenceOBB.m_NormaDirect[1] = { AreaFenceObject->GetMatrot().r[1].m128_f32[0], AreaFenceObject->GetMatrot().r[1].m128_f32[1], AreaFenceObject->GetMatrot().r[1].m128_f32[2] };
	AreaFenceOBB.m_NormaDirect[2] = { AreaFenceObject->GetMatrot().r[2].m128_f32[0], AreaFenceObject->GetMatrot().r[2].m128_f32[1], AreaFenceObject->GetMatrot().r[2].m128_f32[2] };
	AreaFenceOBB.m_fLength[0] = 10;//x方向の長さ
	AreaFenceOBB.m_fLength[1] = 20;//y方向の長さ
	AreaFenceOBB.m_fLength[2] = 10;//z方向の長さ
	//OBBの設定位置
	playerOBB.m_Pos = { Player::GetInstance()->GetPosition().x,Player::GetInstance()->GetPosition().y,Player::GetInstance()->GetPosition().z };
	AreaFenceOBB.m_Pos = { AreaFenceObject->GetPosition().x,  AreaFenceObject->GetPosition().y, AreaFenceObject->GetPosition().z };

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
	}
	else {
		FencePosY_Min = Position.y - 30;
	}
	Position.y = max(Position.y, FencePosY_Min);
}