#include "Wood.h"
#include "Wood.h"
#include"SphereCollider.h"
#include"CollisionAttribute.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"Collision.h"
Wood::~Wood()
{
	delete ps0;
}
void Wood::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();
	m_Model = Model::CreateFromOBJ("wood");

	//フィールドにモデル割り当て
	m_Object->Initialize(camera);
	m_Object->SetModel(m_Model);
	ps0 = new OBBCollision();
	Scale = { 2,3,2 };
	radius_adjustment = -14;
	SetCollider();
}

void Wood::Update(DebugCamera* camera)
{
	m_Object->setSetf(true);
	ParameterSet_Obj(camera);
	//フィールド
	CollideWood();
	
}

void Wood::Draw()
{
	Draw_Obj();
}
bool Wood::CollideWood()
{
	playerOBB.m_NormaDirect[0] = { Player::GetInstance()->GetMatrot().r[0].m128_f32[0],Player::GetInstance()->GetMatrot().r[0].m128_f32[1],Player::GetInstance()->GetMatrot().r[0].m128_f32[2] };
	playerOBB.m_NormaDirect[1] = { Player::GetInstance()->GetMatrot().r[1].m128_f32[0],Player::GetInstance()->GetMatrot().r[1].m128_f32[1],Player::GetInstance()->GetMatrot().r[1].m128_f32[2] };
	playerOBB.m_NormaDirect[2] = { Player::GetInstance()->GetMatrot().r[2].m128_f32[0],Player::GetInstance()->GetMatrot().r[2].m128_f32[1],Player::GetInstance()->GetMatrot().r[2].m128_f32[2] };
	playerOBB.m_fLength[0] = 1;//x方向の長さ
	playerOBB.m_fLength[1] = 1;//y方向の長さ
	playerOBB.m_fLength[2] = 1;//z方向の長さ
	//OBB 回転ベクトル
	woodOBB.m_NormaDirect[0] = { m_Object->GetMatrot().r[0].m128_f32[0],m_Object->GetMatrot().r[0].m128_f32[1],m_Object->GetMatrot().r[0].m128_f32[2] };
	woodOBB.m_NormaDirect[1] = { m_Object->GetMatrot().r[1].m128_f32[0], m_Object->GetMatrot().r[1].m128_f32[1], m_Object->GetMatrot().r[1].m128_f32[2] };
	woodOBB.m_NormaDirect[2] = { m_Object->GetMatrot().r[2].m128_f32[0], m_Object->GetMatrot().r[2].m128_f32[1], m_Object->GetMatrot().r[2].m128_f32[2] };
	woodOBB.m_fLength[0] = 1;//x方向の長さ
	woodOBB.m_fLength[1] = 20;//y方向の長さ
	woodOBB.m_fLength[2] =1;//z方向の長さ
	//OBBの設定位置
	playerOBB.m_Pos = { Player::GetInstance()->GetPosition().x,Player::GetInstance()->GetPosition().y,Player::GetInstance()->GetPosition().z };
	woodOBB.m_Pos = { m_Object->GetPosition().x,  m_Object->GetPosition().y, m_Object->GetPosition().z };

	if (Collision::GetLength(Player::GetInstance()->GetPosition(), Position) < 20) {
		if (ps0->ColOBBs(playerOBB, woodOBB)) {
			Player::GetInstance()->isOldPos();
			return true;
		} else {
				return false;
		}
	}
}
