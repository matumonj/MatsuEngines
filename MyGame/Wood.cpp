#include "Wood.h"
#include "Wood.h"
#include"SphereCollider.h"
#include"CollisionAttribute.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"Collision.h"
#include"PlayerControl.h"
#include"SceneManager.h"
Wood::~Wood()
{
	
}
void Wood::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();

	//フィールドにモデル割り当て
	m_Object->Initialize(camera);
	m_Object->SetModel(ModelManager::GetIns()->GetModel(ModelManager::WOOD));
	Scale = { 2,3,2 };
	radius_adjustment = -14;
	SetCollider();
	alpha = 1;
}
#include"Input.h"
void Wood::Update(DebugCamera* camera)
{
	m_Object->SetColor({ 1,1,1,1 });


	Color = { 1,1,1,alpha };
	ParameterSet_Obj(camera);
	//m_Object->Setf(TRUE);
	//フィールド
	CollideWood();
	
}

void Wood::Draw()
{
	Draw_Obj();
}
bool Wood::CollideWood()
{
	if (PlayerControl::GetInstance()->GetPlayer() == nullptr)return false;
	playerOBB.SetOBBParam_Pos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
	playerOBB.SetOBBParam_Scl({ 1,1,1 });
	playerOBB.SetOBBParam_Rot(PlayerControl::GetInstance()->GetPlayer()->GetMatrot());
	
	//OBB 回転ベクトル
	woodOBB.SetOBBParam_Pos(m_Object->GetPosition());
	woodOBB.SetOBBParam_Scl({ 2,20,3});
	woodOBB.SetOBBParam_Rot(m_Object->GetMatrot());

	if (Collision::GetLength(PlayerControl::GetInstance()->GetPlayer()->GetPosition(), Position) < 20) {
		if (Collision::CheckOBBCollision(playerOBB, woodOBB)==true) {
		PlayerControl::GetInstance()->GetPlayer()->isOldPos();
			return true;
		} else {
				return false;
		}
	}

}
